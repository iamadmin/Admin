/*
 * LogUtil.h
 *
 *  Created on: 2011-12-3
 *      Author: tomzhou
 *
 *      LogUtil log;
 *      log.setFileName("agentd");
 *      log.setBasePath("./logs/");
 *      log.setLogLevel("error"); (保存文件的level)
 *      log.setLogLevel("info","running"); (默认的level)
 *      log.setRotateInterval("day");
 *
 *      log.Write(LogUtil::ERROR,"this is a error");
 *      log.Write("this is a info");
 *
 *      log.setLogLevel("error","running");
 *      log<<log.getLineHeader()<<"String"<< i << double << bool << "\n";
        log.flushLog();
 *
 *
 *      You can use BasePath/level.conf to change the log level online.
 *
 */

#ifndef LOGUTIL_H_
#define LOGUTIL_H_

#include <string>
#include <time.h>
#include <stdio.h>
#include "StringUtil.h"

using namespace std;

namespace util {

class LogUtil {

private:
    //日志级别 (写文件的级别)
    int curLevel;
    //当前进程日志级别;
    int tmpLevel;
    //日志目录
    string basePath;
    //截断间隔:year,month,day,hour
    int interval;

    //重新加载日志级别间隔(从basePath取，时间,默认300秒)
    int reloadConfigTime;

    //当前是否可以写log
    bool logEnable;

    //文件前后缀
    string fileName;
    string fileExtName;

    //日志文件时间字段
    string lastLogName;

    //日志级别的字符串组，用于将日志级别转为字符串写入日志文件。
    string levelArray[8];
    //char* levelArray[];

    //上次读取level.conf的时间（秒）
    time_t lastTime;

    //日志文件指针
    FILE * fp;

    //初始化函数，给构造函数使用
    void init(string bPath);

    //获取文件名中的时间字段
    string getLogName(time_t t);

public:
    //默认构造函数，起始路径为./
    LogUtil();
    //带起始路径的构造函数
    LogUtil(string bPath);

    bool Write(string message, bool newline = true);
    bool Write(int level, string message, bool newline = true);

    void setLogLevel(string level, string type = "file");
    void setRotateInterval(string interval);

    string getLineHeader(int level = -1);

    void flushLog() {
        if (fp != NULL) {
            fflush(fp);
        }
    }

    virtual ~LogUtil();

    string getBasePath() const {
        return basePath;
    }

    string getFileExtName() const {
        return fileExtName;
    }

    string getFileName() const {
        return fileName;
    }

    void setBasePath(string basePath) {
        this->basePath = basePath;
    }

    void setFileExtName(string fileExtName) {
        this->fileExtName = fileExtName;
    }

    void setFileName(string fileName) {
        this->fileName = fileName;
    }

    //重定向功能
    LogUtil& operator <<(int const& value) {
        this->Write(StringUtil::FromInt(value), false);
        return *this;
    }

    //重定向功能
    LogUtil& operator <<(double const& value) {
        this->Write(StringUtil::FromDouble(value), false);
        return *this;
    }

    //重定向功能
    LogUtil& operator <<(bool const& value) {
        this->Write(StringUtil::FromBool(value), false);
        return *this;
    }

    //重定向功能
    LogUtil& operator <<(const char * value) {
        string s(value);
        this->Write(s, false);
        return *this;
    }

    //重定向功能
    LogUtil& operator <<(string const& value) {
        this->Write(value, false);
        return *this;
    }

    //日志级别
    enum LogLevel {
        FATAL, ERROR, WARNING, INFO, DEBUG
    };
    enum RotateInterval {
        YEAR, MONTH, WEEK, DAY, HOUR
    };

};

}
#endif /* LOGUTIL_H_ */
