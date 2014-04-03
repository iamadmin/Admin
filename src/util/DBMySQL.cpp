/*
 * DBMySQL.cpp
 *
 *  Created on: 2012-4-21
 *      Author: tomzhou
 */

#include "DBMySQL.h"

#include <string>
#include <mysql.h>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>

using namespace std;

DBMySQL::DBMySQL() {
    _host = "";
    _port = 3306;
    _user = "";
    _passwd = "";
    _db = "";
    _charset = "";
    _connected = false;
    _res = NULL;
    _null = "";

    mysql_init(&_conn);

}

DBMySQL::DBMySQL(string host, string user, string passwd, string db, int port, string charset) {
    _host = host;
    _port = port;
    _user = user;
    _passwd = passwd;
    _db = db;
    _charset = charset;
    _connected = false;
    _res = NULL;
    _null = "";

    mysql_init(&_conn);
}

DBMySQL::~DBMySQL() {
    if (_res != NULL) {
        mysql_free_result(_res);
    }
    if (_connected) {
        mysql_close(&_conn);
    }
    _arrRec.clear();
    _fields.clear();
}

/**
 * 连接DB
 */
bool DBMySQL::connect() {
    if (_connected) {
        return true;
    }

    if (!mysql_real_connect(&_conn, _host.c_str(), _user.c_str(), _passwd.c_str(), _db.c_str(), _port, NULL, 0)) {
        return false;
    }

    //set charset
    if (_charset.length() > 0) {
        if (mysql_set_character_set(&_conn, _charset.c_str()) != 0) {
            return false;
        }
    }

    return _connected = true;
}

/**
 * 带参数连接DB
 */
bool DBMySQL::connect(string host, string user, string passwd, string db, int port, string charset) {
    _host = host;
    _port = port;
    _user = user;
    _passwd = passwd;
    _db = db;
    _charset = charset;
    _connected = false;

    return connect();
}

/**
 * 获取Insert后的自增长ID
 */
unsigned long DBMySQL::getLastId() {
    _lastId = mysql_insert_id(&_conn);
    return _lastId;
}

/**
 * 获取select的字段数
 */
unsigned long DBMySQL::getNumFields() {
    return _numFields;
}

/**
 * 获取受影响的行数
 */
unsigned long DBMySQL::getNumRows() {
    return _numRows;
}

/**
 * 执行查询(可用于insert,select,update,delete)
 */
bool DBMySQL::query(string sql) {
    int ret = 0;

    //每次查询，都将前次结果集清空
    //可支持一次query，多次get结果
    if (_res != NULL) {
        mysql_free_result(_res);
        _res = NULL;
    }

    if (_connected) {
        ret = mysql_query(&_conn, sql.c_str());
    } else {
        //先尝试连接
        if (connect()) {
            ret = mysql_query(&_conn, sql.c_str());
        } else {
            return false;
        }
    }
    if (ret == 0) {
        _numRows = mysql_affected_rows(&_conn);
        return true;
    } else {
        return false;
    }

}

/**
 * 更改默认DB
 */
int DBMySQL::selectDb(string db) {
    return mysql_select_db(&_conn, db.c_str());
}

/**
 * 获取结果数组
 */
vector<StringMap> DBMySQL::getArray() {

    //如果未进行新的query，则使用上次的result
    //新query会重置_res=NULL
    if (_res == NULL) {
        _numRows = 0;
        _arrRec.clear();

        _res = mysql_store_result(&_conn);
        if (_res == NULL) {
            return _arrRec;
        }
        _numRows = mysql_num_rows(_res);
        //获得fields
        getFields();

        //清空当前集


        MYSQL_ROW row;
        StringMap tmp;
        unsigned int i = 0;
        while ((row = mysql_fetch_row(_res))) {
            if (row == NULL)
                break;
            for (i = 0; i < _fields.size(); i++) {
                if (row[i] == NULL) {
                    tmp[_fields[i]] = _null;
                } else {
                    tmp[_fields[i]] = (char *) row[i];
                }
            }
            _arrRec.push_back(tmp);
        }

    }

    return _arrRec;
}

/**
 * 获取结果数组
 */
vector<StringMap> DBMySQL::getArray(string sql) {
    query(sql);
    return getArray();
}

/**
 * 获取一行记录
 */
StringMap DBMySQL::getOneRow() {
    //获取所有行
    getArray();
    if (_numRows > 0) {
        return _arrRec[0];
    } else {
        StringMap tmp;
        return tmp;
    }
}

/**
 * 获取一行记录
 */
StringMap DBMySQL::getOneRow(string sql) {
    query(sql);
    return getOneRow();
}

/**
 * 获取第一行第一列的数值，通常用于count(*)
 */
int DBMySQL::getOneNum() {
    getArray();
    if (_numRows > 0) {
        string tNum = _arrRec[0][_fields[0]];

        if (tNum == _null)
            return 0;

        return ParseInt(tNum);

    } else {
        return 0;
    }
}

/**
 * 获取第一行第一列的数值，通常用于count(*)
 */
int DBMySQL::getOneNum(string sql) {
    query(sql);
    return getOneNum();
}

/**
 * 获取返回的字段列表
 */
vector<string> DBMySQL::getFields() {
    unsigned int i;
    MYSQL_FIELD * fs;

    _numFields = mysql_num_fields(_res);

    _fields.clear();

    fs = mysql_fetch_fields(_res);
    for (i = 0; i < _numFields; i++) {
        _fields.push_back(fs[i].name);
    }
    return _fields;
}

/**
 * 设置NULL返回的字符串
 */
void DBMySQL::setNullString(string n) {
    _null = n;
}

/**
 * 获取错误号(0表示没有错误)
 */
unsigned int DBMySQL::getErrorNum() {
    return mysql_errno(&_conn);
}

/**
 * 获取错误信息
 */
string DBMySQL::getError() {
    return mysql_error(&_conn);
}

/**
 * 获取客户端版本
 */
string DBMySQL::getClientInfo() {
    return mysql_get_client_info();
}

/**
 * 获取服务端版本
 */
string DBMySQL::getServerInfo() {
    return mysql_get_server_info(&_conn);
}

/**
 * 根据where数组拼接成sql语句(and连接)
 */
string DBMySQL::getSql(string bSql, vector<string> where) {
    if (where.size() < 1)
        return bSql;
    bSql = bSql + " where";
    for (size_t i = 0; i < where.size(); i++) {
        if (i == 0) {
            bSql = bSql + " " + where[i];
        } else {
            bSql = bSql + " and " + where[i];
        }
    }
    return bSql;
}

/**
 * 根据where数组拼接成sql语句(and连接)
 */
string DBMySQL::getSql(string bSql, vector<string> where, string order) {
    return getSql(bSql, where) + " " + order;
}

/**
 * 根据where数组拼接成sql语句(and连接)
 */
string DBMySQL::getSql(string bSql, vector<string> where, string order, int start, int limit) {
    return getSql(bSql, where) + " " + order + " limit " + FromInt(start) + "," + FromInt(limit);
}

/**
 * 将int转为string
 */
string DBMySQL::FromInt(int i) {
    char tmp[30];
    sprintf(tmp, "%d", i);
    return string(tmp);
}

/**
 * 将string转为int
 */
int DBMySQL::ParseInt(string s) {
    return atoi(s.c_str());
}

/**
 * 进行msyql特殊字符转义
 */
string DBMySQL::escape(string s) {
    size_t len = s.size();
    char *tStr = new char[len * 2 + 1];
    mysql_real_escape_string(&_conn, tStr, s.c_str(), len);
    string retStr(tStr);
    delete[] tStr;
    return retStr;
}
