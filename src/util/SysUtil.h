/*
 * SysUtil.h
 *
 *  Created on: 2011-12-17
 *      Author: tomzhou
 */

#ifndef SYSUTIL_H_
#define SYSUTIL_H_

#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

namespace util {

class SysUtil {
public:
    SysUtil() {

    }
    virtual ~SysUtil() {

    }

    //转为Daemon模式
    static bool getDaemon() {
        pid_t pid, sid;

        //创建新进程
        pid = fork();
        //无法创建新进程
        if (pid < 0) {
            cerr << "Fail: can't fork process." << endl;
            return false;
        }

        //父进程直接退出
        if (pid > 0) {
            exit( EXIT_SUCCESS);
        }
        //创建新会话
        sid = setsid();
        if (sid < 0) {
            cerr << "Fail: can't create new session." << endl;
            return false;
        }

        close( STDIN_FILENO);
        close( STDOUT_FILENO);

        /* Change the file mode mask */
        umask(0);

        //子进程退出不需要父进程处理
        signal(SIGCHLD, SIG_IGN);

        return true;

    }

    /**
     * 获取介于min-max间的随机数,不包含max
     */
    static int getRandNum(int min, int max) {
        srand( time(NULL) + rand());
        int tmp = rand()% (max - min) + min;

        return tmp;
    }
};

}

#endif /* SYSUTIL_H_ */
