/*
 * main.cpp
 *
 *  Created on: 2010-1-7
 *      Author: tomzhou
 */

#include <iostream>
#include <string>
#include "util/FileUtil.h"
#include "util/StringUtil.h"
#include "util/LogUtil.h"
#include "util/SysUtil.h"
#include "util/DBMySQL.h"
#include "util/IpcUtil.h"
#include "control/MProcess.h"
#include "Setting.h"
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define VERSION "1.0"

using namespace std;
using namespace util;

int main(int argc, char* argv[]) {

    //可执行文件起始路径
    string startPath = FileUtil::getExePath();
    string basePath = FileUtil::getParentDir(startPath);
    string configFile = "";

    //处理输入参数
    if (argc > 1) {
        int opt;
        while ((opt = getopt(argc, argv, "hf:c:")) != -1) {
            switch (opt) {
            case '?':
                //Ignore invalid parameter.
                continue;
            case 'h':
                cout << endl;
                cout << "masterd v" << VERSION << "      Build on " << __DATE__ << endl;
                cout << "Author: tomzhou     admin.net@163.com" << endl;
                cout << "Usage: masterd [options]" << endl;
                cout << "  -h               Print help information." << endl;
                cout << "  -c configFile    Use this config file(etc/masterd.conf)." << endl;
                cout << endl;
                return 0;
                break;
            case 'f':
            case 'c':
                configFile = optarg;
                break;
            }
        }

    }

    //默认使用./etc/agentd.conf配置文件
    if (configFile == "") {
        configFile = basePath + "/etc/masterd.conf";
    }

    ConfigFile config;
    if (!config.load(configFile.c_str())) {
        cerr << "Error: can't load configuration file(" << configFile << ")." << endl;
        return -1;
    }

    if (!SysUtil::getDaemon()) {
        cerr << "Error: can't run as daemon." << endl;
        return -1;
    }

    //打开日志文件
    config.setSection("Log");
    string logFileName = config.getValue("File");
    if (logFileName.length() > 0) {
        if (logFileName[0] != '/') {
            //相对路径
            logFileName = basePath + "/" + logFileName;
        }
    } else {
        logFileName = basePath + "/logs/masterd.log";
    }
    SYS_LOGGER.setFileName(logFileName.c_str());

    //设置截断大小
    string logMaxSize = config.getValue("MaxSize");
    if (logMaxSize.length() > 0) {
        SYS_LOGGER.setMaxFileSize(StringUtil::toByteSize(logMaxSize));
    } else {
        SYS_LOGGER.setMaxFileSize();
    }
    //设置日志保存份数
    int logMaxIndex = config.getIntValue("MaxIndex");
    if (logMaxIndex > 0) {
        SYS_LOGGER.setMaxFileIndex(logMaxIndex);
    } else {
        SYS_LOGGER.setMaxFileIndex();
    }
    //SYS_LOG(DEBUG, "xxx: %s:%d", "xxxx", i);
    //SYS_LOG(INFO, "xxx: %s:%d", "xxxx", i);

    SYS_LOG(INFO, "masterd is starting...");

    MProcess mp(config);
    mp.Start();

    return 0;
}
