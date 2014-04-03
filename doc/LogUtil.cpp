/*
 * LogUtil.cpp
 *
 *  Created on: 2011-12-3
 *      Author: tomzhou
 */

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <errno.h>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include "LogUtil.h"
#include "StringUtil.h"

using namespace std;

namespace util {

LogUtil::LogUtil() {
    init("./logs/");
}

LogUtil::LogUtil(string bPath) {
    init(bPath);
}

void LogUtil::init(string bPath) {
    //init

    //初始化日志级别字符串数组
    levelArray[0] = "FATAL";
    levelArray[1] = "ERROR";
    levelArray[2] = "WARNING";
    levelArray[3] = "INFO";
    levelArray[4] = "DEBUG";

    logEnable = true;
    interval = DAY;
    curLevel = ERROR;
    tmpLevel = INFO;

    //默认重加载日志级别的间隔60s
    reloadConfigTime = 60;
    fileName = "";
    fileExtName = "log";
    lastLogName = "";

    lastTime = time(NULL);

    //日志文件指针，默认为空
    fp = NULL;

    //prepare base path
    basePath = bPath;
    if (basePath.length() == 0) {
        basePath = "./logs/";
    }
    DIR *d = NULL;
    errno = 0;
    d = opendir(basePath.c_str());
    if (NULL == d) {
        if (errno == ENOENT) {
            //创建目录
            if (mkdir(basePath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
                cerr << "Fail to create log directory, logging is disabled.";
                logEnable = false;
                return;
            }
        } else {
            cerr << "Fail to open log directory, logging is disabled.";
            logEnable = false;
            return;
        }
    } else {
        closedir(d);
    }
}

//写日志，默认级别为tmpLevel，可通过setLogLevel设置默认级别
bool LogUtil::Write(string message, bool newline) {
    return Write(tmpLevel, message, newline);
}

//写日志，默认级别为INFO
//bool LogUtil::Write(char * message) {
//	return Write(INFO, string(message));
//}
//
//bool LogUtil::Write(int level, char * message) {
//	return Write(level, string(message));
//}

//写日志，newline=true为行模式，自动加前导字符和回行符
bool LogUtil::Write(int level, string message, bool newline) {
    //异常情况，无法打开log目录或文件
    if (logEnable == false) {
        return false;
    }

    //取当前时间
    time_t nowTime = time(NULL);
    //是否重加载log level
    if (nowTime - lastTime > reloadConfigTime) {
        lastTime = nowTime;
        //重新加载日志级别 basePath/level
        string levelFile = basePath + "/level.conf";
        FILE * tmpFp;
        char tmpLevel[50];
        tmpLevel[0] = '\0';
        tmpFp = fopen(levelFile.c_str(), "r");
        if (tmpFp != NULL) {
            fgets(tmpLevel, 50, tmpFp);
            fclose(tmpFp);
            setLogLevel(tmpLevel);
        }
    }

    //判断是否需要记日志
    if (level > curLevel) {
        return true;
    }

    //取到文件名
    string curLogName = getLogName(nowTime);
    if (curLogName != lastLogName) {
        lastLogName = curLogName;
        //关闭上次打开的log文件。
        if (fp != NULL) {
            fclose(fp);
            fp = NULL;
        }

    }

    //取得log文件名
    string fullName = basePath + "/" + fileName + lastLogName + "." + fileExtName;

    //打开文件
    if (fp == NULL) {
        fp = fopen(fullName.c_str(), "a");
        if (fp == NULL) {
            return false;
        }
    }

    //注意多进程情况

    //写日志 [TIME] [LEVEL] [PID] message
    if (newline) {
        string header=getLineHeader(level);
        fprintf(fp, "%s", header.c_str());

        //换行退格处理 replace("\n", "\n  ");
        message = StringUtil::ReplaceAll(message, "\n", "\n  ");
    }

    fprintf(fp, "%s",  message.c_str());

    if (newline) {
        //行模式，加回车并flush到文件
        fprintf(fp,"%s", "\n");
        fflush(fp);
    }


    return true;
}

string LogUtil::getLineHeader(int level) {
    //取当前时间
    time_t nowTime = time(NULL);
    struct tm * tmpTime = localtime(&nowTime);
    char strTime[30];
    sprintf(strTime, "%04d-%02d-%02d %02d:%02d:%02d", tmpTime->tm_year + 1900, tmpTime->tm_mon + 1, tmpTime->tm_mday, tmpTime->tm_hour, tmpTime->tm_min, tmpTime->tm_sec);

    //取pid
    pid_t pid = getpid();

    if (level<0) level=tmpLevel;
    char strHeader[60];
    sprintf(strHeader, "%s%s%s%s%s%d%s", "[", strTime, "] [", levelArray[level].c_str(), "] [", pid, "] ");

    string ret(strHeader);
    return ret;
}

/**
 * 设置日志级别(type=file时，设置保存文件的level)
 */
void LogUtil::setLogLevel(string level, string type) {
    int _localLevel = 0;
    if (StringUtil::CompareIgnoreCase(StringUtil::Trim(level), "FATAL")) {
        _localLevel = FATAL;
    } else if (StringUtil::CompareIgnoreCase(StringUtil::Trim(level), "ERROR")) {
        _localLevel = ERROR;
    } else if (StringUtil::CompareIgnoreCase(StringUtil::Trim(level), "WARNING")) {
        _localLevel = WARNING;
    } else if (StringUtil::CompareIgnoreCase(StringUtil::Trim(level), "DEBUG")) {
        _localLevel = DEBUG;
    } else {
        _localLevel = INFO;
    }

    if (StringUtil::CompareIgnoreCase(type, "file")) {
        curLevel = _localLevel;
    } else {
        tmpLevel = _localLevel;
    }

}

/**
 * 设置轮循间隔
 */
void LogUtil::setRotateInterval(string interval) {
    if (StringUtil::CompareIgnoreCase(StringUtil::Trim(interval), "YEAR")) {
        interval = YEAR;
    } else if (StringUtil::CompareIgnoreCase(StringUtil::Trim(interval), "MONTH")) {
        interval = MONTH;
    } else if (StringUtil::CompareIgnoreCase(StringUtil::Trim(interval), "WEEK")) {
        interval = WEEK;
    } else if (StringUtil::CompareIgnoreCase(StringUtil::Trim(interval), "HOUR")) {
        interval = HOUR;
    } else {
        interval = DAY;
    }
}

string LogUtil::getLogName(time_t t) {
    char tmp[20];
    struct tm * tt = localtime(&t);
    if (interval == YEAR) {
        sprintf(tmp, "%04d", tt->tm_year + 1900);
    } else if (interval == MONTH) {
        sprintf(tmp, "%04d%02d", tt->tm_year + 1900, tt->tm_mon + 1);
    } else if (interval == WEEK) {
        sprintf(tmp, "%04d%02d", tt->tm_year + 1900, tt->tm_yday / 7);
    } else if (interval == HOUR) {
        sprintf(tmp, "%04d%02d%02d%02d", tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour);
    } else {
        sprintf(tmp, "%04d%02d%02d", tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday);
    }
    return tmp;
}

LogUtil::~LogUtil() {
    //关闭文件指针
    if (fp != NULL) {
        fclose(fp);
    }
}

}
