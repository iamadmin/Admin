/*
 * MProcess.h
 *
 *  Created on: 2014-3-12
 *      Author: tomzhou
 */

#ifndef MPROCESS_H_
#define MPROCESS_H_

#include <iostream>
#include <string>
#include "../util/FileUtil.h"
#include "../util/StringUtil.h"
#include "../util/LogUtil.h"
#include "../Setting.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

struct client_t {
    struct bufferevent *buffEvt;
    int fd;
    int length;
    int curPos;
    char * data;
};


class MProcess {
public:
    MProcess(ConfigFile & _conf);
    virtual ~MProcess();

    void Start();

    static void doAccept(int listener, short event, void *arg);
    static void doRead(struct bufferevent *ev_buf, void *arg);
    static void doWrite(struct bufferevent *ev_buf, void *arg);
    static void doError(struct bufferevent *ev_buf, short error, void *arg);

    static void doEventError(int err);
    static void doEventLog(int level, const char *msg);

    static void doProcess(char *s);

private:
     ConfigFile config;

};

#endif /* MPROCESS_H_ */
