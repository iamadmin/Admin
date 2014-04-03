/*
 * IpcUtil.h
 *
 *  Created on: 2012-07-20
 *      Author: tomzhou
 */

#ifndef IPCUTIL_H_
#define IPCUTIL_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

using namespace std;

namespace util {

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};


/**
 * 用法
 * SemUtil lock(100);
 * lock.init(); (只能有一个进程初始化一次)
 * lock.getLock();
 * dosomething();
 * lock.unLock();
 *
 */
class SemUtil {

private:
    key_t _k;
    int _id;

public:
    //默认构造函数，必须带key
    SemUtil(key_t k) {
        _k = k;
        _id = semget(_k, 1, 0666 | IPC_CREAT);
    }

    //析构函数
    ~SemUtil() {

    }

    /**
     * 获取锁
     */
    bool getLock() {
        if (_id < 0)
            return false;
        struct sembuf tbuf;
        tbuf.sem_num = 0;
        tbuf.sem_op = -1;
        tbuf.sem_flg = SEM_UNDO;
        if (semop(_id, &tbuf, 1) == -1) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * 释放锁
     */
    bool unLock() {
        if (_id < 0)
            return false;
        struct sembuf tbuf;
        tbuf.sem_num = 0;
        tbuf.sem_op = 1;
        tbuf.sem_flg = SEM_UNDO;
        if (semop(_id, &tbuf, 1) == -1) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * 初始化信号值为1, 通常只需要初始化一次。
     */
    bool init() {
        union semun tmp;
        tmp.val = 1;
        if (semctl(_id, 0, SETVAL, tmp) == -1) {
            return false;
        } else {
            return true;
        }
    }

    /**
     *删除锁
     */
    bool delLock() {
        union semun tmp;
        tmp.val = 1;
        if (semctl(_id, 0, IPC_RMID, tmp) == -1) {
            return false;
        } else {
            return true;
        }
    }

};

}

#endif /* FILEUTIL_H_ */
