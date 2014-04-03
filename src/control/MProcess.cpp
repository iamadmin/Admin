/*
 * MProcess.cpp
 *
 *  Created on: 2014-3-12
 *      Author: tomzhou
 */

#include "MProcess.h"
#include <iostream>
#include <string>
#include "../util/FileUtil.h"
#include "../util/StringUtil.h"
#include "../util/LogUtil.h"
#include "../util/NetUtil.h"
#include "../Setting.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

MProcess::MProcess(ConfigFile & _conf) {
    config = _conf;
}

MProcess::~MProcess() {
    // TODO Auto-generated destructor stub
}

//Accept
void MProcess::doAccept(int listener, short event, void *arg) {
    struct event_base *base = (struct event_base *) arg;
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    int fd = accept(listener, (struct sockaddr*) &client_addr, &length);

    if (fd < 0) {
        SYS_LOG(ERROR, "Fail to accept socket.");
    } else if (fd > FD_SETSIZE) {
        close(fd);
        SYS_LOG(ERROR, "Fail to accept socket.");
    } else {
        evutil_make_socket_nonblocking(fd);
        //todo ...

        client_t *client = (client_t*) malloc(sizeof(client_t));

        client->buffEvt = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        client->fd = fd;
        bufferevent_setcb(client->buffEvt, doRead, doWrite, doError, client);

        //bufferevent_setwatermark(client->buffEvt, EV_READ, 0, 102400);
        bufferevent_enable(client->buffEvt, EV_READ | EV_WRITE);
    }

}

void MProcess::doRead(struct bufferevent *ev_buf, void *arg) {
    client_t *client = (client_t *) arg;
    struct evbuffer *input = bufferevent_get_input(ev_buf);
    struct evbuffer *output = bufferevent_get_output(ev_buf);

    char *data = NULL;
    size_t len;
    data = evbuffer_readln(input, &len, EVBUFFER_EOL_CRLF);

    if (data != NULL) {
        if (data[0] == '\x03' || data[0] == '\x04' || StringUtil::compareIgnoreCase(data,"quit")) {
            free(data);
            bufferevent_free(ev_buf);
            close(client->fd);
            free(client);
            return;
        }
        doProcess(data);

        evbuffer_add(output, data, strlen(data));
        evbuffer_add(output, "\n", 1);

        free(data);
    }
}

void MProcess::doWrite(struct bufferevent *ev_buf, void *arg) {

}
void MProcess::doEventLog(int level, const char *msg) {
    SYS_LOG(INFO, "libEvent2 [%d]:%s", level, msg);
}
void MProcess::doEventError(int err) {
    SYS_LOG(ERROR, "libEvent2 fatal error: %d", err);
}

void MProcess::doError(struct bufferevent *ev_buf, short error, void *arg) {
    client_t *client = (client_t *) arg;

    if (error & BEV_EVENT_EOF) {
        /* connection has been closed, do any clean up here */
        /* ... */
    } else if (error & BEV_EVENT_ERROR) {
        /* check errno to see what error occurred */
        /* ... */
    } else if (error & BEV_EVENT_TIMEOUT) {
        /* must be a timeout event handle, handle it */
        /* ... */
    }
    bufferevent_free(ev_buf);
    close(client->fd);
    free(client);

}

void MProcess::doProcess(char *s) {
    if (s == NULL)
        return;
    size_t len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 32;
        } else if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        } else {

        }
    }
    return;
}

void MProcess::Start() {
    //获取监听IP
    config.setSection("Local");
    string listenIp = config.getValue("Listen");
    uint32_t iIp = INADDR_ANY;
    if (listenIp.length() > 0) {
        if (listenIp[0] >= '0' && listenIp[0] <= '9') {
            iIp = NetUtil::getAddr(listenIp.c_str());
        } else {
            iIp = NetUtil::getLocalAddr(listenIp.c_str());
        }
    }
    int iPort = config.getIntValue("Port");
    if (iPort < 1 || iPort > 65535)
        iPort = 8100;

    //开启socket
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = iIp;
    sin.sin_port = htons(iPort);

    SYS_LOG(INFO, "Listen on [%s:%d]", NetUtil::ipToString(iIp).c_str(), iPort);

    int listener = 0;
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Can't create socket, Error(" << errno << "):" << strerror(errno);
        exit(-1);
    }
    evutil_make_socket_nonblocking(listener);

    if (bind(listener, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        cerr << "Can't bind on [" << NetUtil::ipToString(iIp) << ":" << iPort << "], Error(" << errno << "):" << strerror(errno);
        exit(-1);
    }

    if (listen(listener, 512) < 0) {
        cerr << "Can't listen on [" << NetUtil::ipToString(iIp) << ":" << iPort << "], Error(" << errno << "):" << strerror(errno);
        exit(-1);
    }

    struct event_base *base;
    struct bufferevent *bev;
    struct event *listevent;

    base = event_base_new();
    event_set_log_callback(doEventLog);
    event_set_fatal_callback(doEventError);

    listevent = event_new(base, listener, EV_READ | EV_PERSIST, doAccept, (void*) base);
    event_add(listevent, NULL);

    event_base_dispatch(base);

    close(listener);
}
