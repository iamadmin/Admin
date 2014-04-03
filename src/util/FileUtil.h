/*
 * FileUtil.h
 *
 *  Created on: 2011-11-26
 *      Author: tomzhou
 */

#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include <string>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

namespace util {

class FileUtil {
public:

    /**
     * 取得当前执行路径
     * @return
     */
    static string getWorkDir() {
        char curDir[1025];
        string strRes;
        if (getcwd(curDir, sizeof(curDir)) != NULL) {
            strRes = strRes.insert(0, curDir);
            return strRes;
        } else {
            return "";
        }
    }

    /**
     * 取到执行文件全路径
     * @return
     */
    static string getExeName() {
        char exePath[1025];
        string strRes;
        unsigned int tmpLen = 0;
        tmpLen = readlink("/proc/self/exe", exePath, sizeof(exePath));
        if (tmpLen > 0 && tmpLen < sizeof(exePath)) {
            exePath[tmpLen] = '\0';
            strRes = strRes.insert(0, exePath);
            return strRes;
        } else {
            return "";
        }
    }

    /**
     * 取到执行文件路径，不包文件名
     * @return
     */
    static string getExePath() {
        string exeName;
        size_t lastPos;
        exeName = FileUtil::getExeName();
        if (exeName.empty()) {
            return "";
        } else {
            lastPos = exeName.find_last_of("/\\");
            if (lastPos == string::npos) {
                return "/";
            } else {
                exeName = exeName.substr(0, lastPos);
                return exeName;
            }
        }
    }

    /**
     * 文件或目录是否存在
     * @param fName
     * @return
     */
    static bool isExist(string fName) {
        struct stat buf;
        int ret = stat(fName.c_str(), &buf);
        if (ret == 0)
            return true;
        if (errno == ENOENT)
            return false;
        return true;
    }

    /**
     * 是否是普通文件
     * @param fName
     * @return
     */
    static bool isFile(string fName) {
        struct stat buf;
        int ret = stat(fName.c_str(), &buf);
        if (ret == 0) {
            return buf.st_mode & S_IFREG;
        } else {
            return false;
        }
    }

    /**
     * 是否是目录
     * @param fName
     * @return
     */
    static bool isDirectory(string fName) {
        struct stat buf;
        int ret = stat(fName.c_str(), &buf);
        if (ret == 0) {
            return buf.st_mode & S_IFDIR;
        } else {
            return false;
        }
    }

    /**
     * 获取真实路径，去除软链,和..之类，如果路径不存在，则返回空字符串
     * @param fName
     * @return
     */
    static string getRealPath(string fName) {
        char rPath[PATH_MAX];
        char *pPath;
        pPath = realpath(fName.c_str(), rPath);
        if (pPath != NULL) {
            string ret(rPath);
            return ret;
        } else {
            return "";
        }
    }

    /**
     * 创建多级目录
     */
    static bool makeDirs(char *szDirPath) {
        struct stat stats;
        if (lstat(szDirPath, &stats) == 0 && S_ISDIR (stats.st_mode))
            return true;

        mode_t umask_value = umask(0);
        umask(umask_value);
        mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;

        char *slash = szDirPath;
        while (*slash == '/')
            slash++;

        while (1) {
            slash = strchr(slash, '/');
            if (slash == NULL)
                break;

            *slash = '\0';
            int ret = mkdir(szDirPath, mode);
            *slash++ = '/';
            if (ret && errno != EEXIST) {
                return false;
            }

            while (*slash == '/')
                slash++;
        }
        if (mkdir(szDirPath, mode)) {
            return false;
        }
        return true;
    }

    /**
     * 取文件名目录的父级目标，不核对目录是否存在
     * @param dir
     * @return
     */
    static string getParentDir(string dir) {

        //为空直接返回./
        if (dir.empty())
            return "./";

        //根目录直接返回
        if (dir == "/" || dir == "\\")
            return "/";

        //忽略最后的/
        size_t lastPos = dir.size() - 1;

        if (dir[lastPos] == '/' || dir[lastPos] == '\\') {
            dir = dir.substr(0, lastPos);
        }

        lastPos = dir.find_last_of("/\\");
        if (lastPos == string::npos) {
            return "./";
        } else {
            dir = dir.substr(0, lastPos);
            return dir;
        }

    }

};

}

#endif /* FILEUTIL_H_ */
