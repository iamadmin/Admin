/*
 * LogUtil.h
 * Author: taobao, tomzhou
 * Usage:
 *      SYS_LOGGER.setFileName("./agentd.log");
 *      SYS_LOGGER.setMaxFileSize();
 *      SYS_LOGGER.setMaxFileIndex();
 *      SYS_LOG(DEBUG, "xxx: %s:%d", "xxxx", i);
 *      SYS_LOG(INFO, "xxx: %s:%d", "xxxx", i);
 *
 */

#ifndef LOGUTIL_H_
#define LOGUTIL_H_

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <deque>
#include <string>
#include <pthread.h>
#include <sys/time.h>

#define SYS_LOG_LEVEL_ERROR 0
#define SYS_LOG_LEVEL_WARN  1
#define SYS_LOG_LEVEL_INFO  2
#define SYS_LOG_LEVEL_DEBUG 3
#define SYS_LOG_LEVEL(level) SYS_LOG_LEVEL_##level, __FILE__, __LINE__, __FUNCTION__
#define SYS_LOG_NUM_LEVEL(level) level, __FILE__, __LINE__, __FUNCTION__
#define SYS_LOGGER util::CLogger::_logger
#define SYS_PRINT(level, ...) SYS_LOGGER.logMessage(SYS_LOG_LEVEL(level), __VA_ARGS__)
#define SYS_LOG_BASE(level, ...) (SYS_LOG_LEVEL_##level>SYS_LOGGER._level) ? (void)0 : SYS_PRINT(level, __VA_ARGS__) 
#define SYS_LOG(level, _fmt_, args...) ((SYS_LOG_LEVEL_##level>SYS_LOGGER._level) ? (void)0 : SYS_LOG_BASE(level, "[%ld] " _fmt_, getpid(), ##args))
#define SYS_LOG_US(level, _fmt_, args...) \
  ((SYS_LOG_LEVEL_##level>SYS_LOGGER._level) ? (void)0 : SYS_LOG_BASE(level, "[%ld][%ld][%ld] " _fmt_, \
                                                            getpid(), util::CLogger::get_cur_tv().tv_sec, \
                                                            util::CLogger::get_cur_tv().tv_usec, ##args))

namespace util {
using std::deque;
using std::string;

/** 
* @brief 简单的日志系统 
*/
class           CLogger {
public:
    CLogger();
    ~CLogger();
    /** 
     * @brief 
     * 
     * @param filename
     * @param fmt
     */
    void rotateLog(const char *filename, const char *fmt = NULL);
    /** 
     * @brief 将日志内容写入文件
     * 
     * @param level 日志的级别
     * @param file  日志内容所在的文件
     * @param line  日志内容所在的文件的行号
     * @param function 写入日志内容的函数名称
     * @param fmt
     * @param ...
     */
    void logMessage(int level, const char *file, int line, const char *function, const char *fmt, ...);
    /** 
     * @brief 设置日志的级别
     * 
     * @param level DEBUG|WARN|INFO|ERROR
     */
    void setLogLevel(const char *level);
    /** 
     * @brief 设置日志文件的名称
     * 
     * @param filename 日志文件的名称
     */
    void setFileName(const char *filename, bool flag = false);
    /** 
     * @brief 检测文件是否已经打开,标准输出,错误输出重定向
     */
    void checkFile();
    void setCheck(int v) {_check = v;}
    /** 
     * @brief 设置日志文件文件的大小,达到maxFileSize就新打开一个文件
     * 如果不设置此项，日志系统会忽略日志滚动
     * 
     * @param maxFileSize 日志文件的大小
     */
    void setMaxFileSize( int64_t maxFileSize=0x40000000);
    /** 
     * @brief 保留最近maxFileIndex个日志文件，超出maxFileIndex个日志文件
     * 会按时间先后删除,但进程重启后日志系统会按时间先后重新统计
     * 
     * @param maxFileIndex 保留文件的最大个数
     */
    void setMaxFileIndex( int maxFileIndex= 0x0F);

    static inline struct timeval get_cur_tv()
    {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      return tv;
    };

private:
    int _fd;
    char *_name;
    int _check;
    size_t _maxFileIndex;
    int64_t _maxFileSize;
    bool _flag;

    //记录上一次写日志的分钟数，用于每分钟自动检测是否需要动态更新日志级别
    int _lastMin;

public:
    static CLogger _logger;
    int _level;

private:
    std::deque<std::string> _fileList;
    static const char *const _errstr[];   
    pthread_mutex_t _fileSizeMutex;
    pthread_mutex_t _fileIndexMutex; 
};

}
#endif
