/*
 * Setting.h
 *
 *  Created on: 2011-11-27
 *      Author: tomzhou
 */

#ifndef SETTING_H_
#define SETTING_H_
#include <string>
#include "util/config/configfile.h"

using namespace std;
using namespace util;

class Setting {
private:
	//配置文件
	string configFile;
	//服务器IP&Port
	string serverIp;
	int serverPort;
	//本地IP&Port
	string localIp;
	int localPort;

	string logLevel;
	string logRotate;
	string logPath;

	string dbHost;
	int dbPort;
	string dbUser;
	string dbPassword;
	string dbName;


	ConfigFile cf;

public:
	Setting() {
		logLevel="error";
		logRotate="day";
		logPath="./agentdlogs/";

		localIp="0.0.0.0";
		localPort=8020;

		serverIp="0.0.0.0";
		serverPort=8040;
	}
	virtual ~Setting() {

	}
    string getConfigFile() const
    {
        return configFile;
    }

    string getLocalIp() const
    {
        return localIp;
    }

    int getLocalPort() const
    {
        return localPort;
    }

    string getServerIp() const
    {
        return serverIp;
    }

    int getServerPort() const
    {
        return serverPort;
    }

    void setConfigFile(string configFile)
    {
        this->configFile = configFile;
    }

    void setLocalIp(string localIp)
    {
        this->localIp = localIp;
    }

    void setLocalPort(int localPort)
    {
        this->localPort = localPort;
    }

    void setServerIp(string serverIp)
    {
        this->serverIp = serverIp;
    }

    void setServerPort(int serverPort)
    {
        this->serverPort = serverPort;
    }

    string getLogLevel() const
    {
        return logLevel;
    }

    string getLogPath() const
    {
        return logPath;
    }

    string getLogRotate() const
    {
        return logRotate;
    }

    void setLogLevel(string logLevel)
    {
        this->logLevel = logLevel;
    }

    void setLogPath(string logPath)
    {
        this->logPath = logPath;
    }

    void setLogRotate(string logRotate)
    {
        this->logRotate = logRotate;
    }

    //从配置文件加载配置
    bool loadConfigFile(string fname) {
    	configFile=fname;
    	if (access(configFile.c_str(),R_OK|W_OK)!=0) {
			cerr<<"Error: config file \""<<configFile<<"\" doesn't exist or is not writable."<<endl;
			return false;
		}

    	cf.clear();
    	if (!cf.load(configFile.c_str())) {
    		cerr<<"Error: can't read the config file."<<endl;
    		cerr<<"Msg: " << cf.getError() <<endl;
    		return false;
    	}
    	cf.setSection("Server");
    	serverIp=cf.getStringValue("serverIp");
    	serverPort=cf.getIntValue("serverPort");

    	cf.setSection("Client");
    	localIp=cf.getStringValue("localIp");
    	localPort=cf.getIntValue("localPort");

    	cf.setSection("Log");
    	logLevel=cf.getStringValue("logLevel");
    	logRotate=cf.getStringValue("logRotate");
    	logPath=cf.getStringValue("logPath");

    	cf.setSection("Db");
    	dbHost=cf.getStringValue("dbHost");
    	dbUser=cf.getStringValue("dbUser");
    	dbPassword=cf.getStringValue("dbPassword");
    	dbName=cf.getStringValue("dbName");
    	dbPort=cf.getIntValue("dbPort");

    	if (logLevel=="") logLevel="error";
    	if (logRotate=="") logRotate="day";
    	//默认使用 ./logs/目录存放日志
    	if (logPath=="") logPath=FileUtil::getExePath()+"/logs/";

    	if (localIp=="") localIp="0.0.0.0";
    	if (localPort<1) localPort=8020;

    	if (serverIp=="") serverIp="0.0.0.0";
    	if (serverPort<1) serverPort=8020;

    	if (dbPort<1) dbPort=3306;

		return true;
    }

    bool saveConfigFile() {

    	//有些配置可能从服务端下发过来，需要写回文件
    	cf.setSection("Server");
    	cf.setStringValue("serverIp",serverIp.c_str());
    	cf.setIntValue("serverPort",serverPort);

    	return cf.save();
    }

    string getDbHost() const
    {
        return dbHost;
    }

    string getDbName() const
    {
        return dbName;
    }

    string getDbPassword() const
    {
        return dbPassword;
    }

    int getDbPort() const
    {
        return dbPort;
    }

    string getDbUser() const
    {
        return dbUser;
    }

    void setDbHost(string dbHost)
    {
        this->dbHost = dbHost;
    }

    void setDbName(string dbName)
    {
        this->dbName = dbName;
    }

    void setDbPassword(string dbPassword)
    {
        this->dbPassword = dbPassword;
    }

    void setDbPort(int dbPort)
    {
        this->dbPort = dbPort;
    }

    void setDbUser(string dbUser)
    {
        this->dbUser = dbUser;
    }

};

#endif /* SETTING_H_ */
