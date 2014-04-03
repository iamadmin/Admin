/*
 * DBMySQL.h
 *
 *  Created on: 2012-4-21
 *      Author: tomzhou
 */

#ifndef DBMYSQL_H_
#define DBMYSQL_H_

#include <string>
#include <mysql.h>
#include <map>
#include <vector>

using namespace std;

typedef map<string, string> StringMap;
typedef map<int, string> IntMap;

class DBMySQL {
public:
    DBMySQL();
    DBMySQL(string host, string user, string passwd, string db, int port = 3306, string charset = "");
    virtual ~DBMySQL();

    bool connect();
    bool connect(string host, string user, string passwd, string db, int port = 3306, string charset = "");

    //获取insert后的自增长id
    unsigned long getLastId();
    //获取列数量
    unsigned long getNumFields();
    //获取受影响的行数量
    unsigned long getNumRows();

    //执行查询(可用于insert,select,update,delete)
    bool query(string sql);

    //获取结果数组
    vector<StringMap> getArray();
    vector<StringMap> getArray(string sql);

    //获取一行记录
    StringMap getOneRow();
    StringMap getOneRow(string sql);

    //获取第一行第一列的数值，通常用于count(*)
    int getOneNum();
    int getOneNum(string sql);

    //获取返回的字段列表
    vector<string> getFields();

    //更换DB
    int selectDb(string db);

    //设置NULL返回的字符串
    void setNullString(string n);

    //获取错误或信息
    unsigned int getErrorNum();
    string getError();
    string getClientInfo();
    string getServerInfo();

    //进行msyql特殊字符转义
    string escape(string s);

    //数字与字符串转换工具
    static string FromInt(int i);
    static int ParseInt(string s);

    //拼接SQL
    static string getSql(string bSql, vector<string> where);
    static string getSql(string bSql, vector<string> where, string order);
    static string getSql(string bSql, vector<string> where, string order, int start, int limit);

private:

    string _host;
    int _port;
    string _user;
    string _passwd;
    string _db;
    string _charset;

    bool _connected;

    MYSQL _conn;
    MYSQL_RES * _res;

    unsigned long _lastId;
    unsigned int _numFields;
    unsigned long _numRows;

    vector<StringMap> _arrRec; //多行记录
    vector<string> _fields; //数据列集合

    string _null;

};

#endif /* DBMYSQL_H_ */
