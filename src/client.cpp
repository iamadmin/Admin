/*
 * client.cpp
 *
 *  Created on: 2013-6-29
 *      Author: tomzhou
 */


#include <iostream>
#include <string>
#include "util/config/configfile.h"
#include "util/FileUtil.h"
#include "util/StringUtil.h"
#include "util/LogUtil.h"
#include "util/SysUtil.h"
#include "util/DBMySQL.h"
#include "util/IpcUtil.h"
#include "util/NetUtil.h"
#include "util/TimeUtil.h"
#include "Setting.h"

#include "model/Person.pb.h"

#include <fstream>
#include <sys/types.h>
#include <unistd.h>


#define VERSION "1.0"

using namespace std;
using namespace util;
using namespace model;

int main(int argc, char* argv[]) {

    cout << "I am client" << endl;

//    LogUtil log;
//    log.setFileName("sem");
//    log.setBasePath("./logs/");
//    log.setLogLevel("debug");
//    log.setLogLevel("error","running");
//
//
//
//    int pTime=0;
//    for (int i=0;i<200;i++) {
//
//        log<<log.getLineHeader()<<"Agent ("<< i << ") Begin.... \n";
//        log.flushLog();
//        pTime = SysUtil::getRandNum(0,4);
//        sleep(pTime);
//        log<<log.getLineHeader()<<"Agent ("<< i << ") End. \n";
//        log.flushLog();
//
//    }

    return 0;
}


