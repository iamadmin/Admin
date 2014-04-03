/*
 * StringUtil.h
 *
 *  Created on: 2011-12-4
 *      Author: tomzhou
 */

#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <vector>

using namespace std;

namespace util {

class StringUtil {
public:

    /**
     * 将字符串转为全大写
     */
    static string toUpper(const string &s) {
        string tmp = s;
        int len = tmp.length();
        for (int i = 0; i < len; i++) {
            tmp[i] = toupper((unsigned char) tmp[i]);
        }
        return tmp;
    }

    /**
     * 字符串是否是数字
     */
    static int isInt(const char *p) {
        if (p == NULL || (*p) == '\0')
            return 0;
        if ((*p) == '-')
            p++;
        while ((*p)) {
            if ((*p) < '0' || (*p) > '9')
                return 0;
            p++;
        }
        return 1;
    }

    /**
     * 将字符串转为全小写
     */
    static string toLower(const string &s) {
        string tmp = s;
        int len = tmp.length();
        for (int i = 0; i < len; i++) {
            tmp[i] = tolower((unsigned char) tmp[i]);
        }
        return tmp;
    }

    /**
     * 去除前后空格
     */
    static string trim(const string &s) {
        const char* p0 = s.c_str();
        const char* p1 = p0;
        const char* p2 = p0 + s.length() - 1;
        while (isspace((unsigned char) *p1)) {
            p1++;
        }
        while (isspace((unsigned char) *p2)) {
            p2--;
        }

        string tmp = s.substr(p1 - p0, p2 - p1 + 1);
        return tmp;
    }

    /**
     * 去前后空格
     * str 操作的字符串，可能会被修改
     * what 需要trim的字符，一般是" \t\n\r"
     * mode = 1 (left trim), 2(right trim), 3 (left+right trim)
     */
    static char * trim(char *str, const char *what = " \t\r\n", int mode = 3) {
        char mask[256];
        unsigned char *p;
        unsigned char *ret;
        memset(mask, 0, 256);
        ret = (unsigned char *) str;
        p = (unsigned char *) what;
        while (*p) {
            mask[*p] = '\1';
            p++;
        }
        if (mode & 1) { // 前面
            p = ret;
            while (*p) {
                if (!mask[*p]) {
                    break;
                }
                ret++;
                p++;
            }
        }
        if (mode & 2) { // 后面
            p = ret + strlen((const char*) ret) - 1;
            while (p >= ret) {
                if (!mask[*p]) {
                    break;
                }
                p--;
            }
            p++;
            *p = '\0';
        }
        return (char*) ret;
    }

    /**
     * 得到str的hash值 (返回int值)
     */
    static int hashCode(const char *str) {
        int h = 0;
        while (*str) {
            h = 31 * h + (*str);
            str++;
        }
        return h;
    }

    /**
     * urldecode
     */
    static char * urlDecode(const char *src, char *dest) {
        if (src == NULL || dest == NULL) {
            return NULL;
        }

        const char *psrc = src;
        char *pdest = dest;

        while (*psrc) {
            if (*psrc == '+') {
                *pdest = ' ';
            } else if (*psrc == '%' && isxdigit(*(psrc + 1)) && isxdigit(*(psrc + 2))) {
                int c = 0;
                for (int i = 1; i <= 2; i++) {
                    c <<= 4;
                    if (psrc[i] >= '0' && psrc[i] <= '9') {
                        c |= (psrc[i] - '0');
                    } else if (psrc[i] >= 'a' && psrc[i] <= 'f') {
                        c |= (psrc[i] - 'a') + 10;
                    } else if (psrc[i] >= 'A' && psrc[i] <= 'F') {
                        c |= (psrc[i] - 'A') + 10;
                    }
                }
                *pdest = (char) (c & 0xff);
                psrc += 2;
            } else {
                *pdest = *psrc;
            }
            psrc++;
            pdest++;
        }
        *pdest = '\0';
        return dest;
    }

    /**
     * 比较好的hash算法
     * http://murmurhash.googlepages.com/
     */
    static unsigned int murMurHash(const void *key, int len) {
        const unsigned int m = 0x5bd1e995;
        const int r = 24;
        const int seed = 97;
        unsigned int h = seed ^ len;
        // Mix 4 bytes at a time into the hash
        const unsigned char *data = (const unsigned char *) key;
        while (len >= 4) {
            unsigned int k = *(unsigned int *) data;
            k *= m;
            k ^= k >> r;
            k *= m;
            h *= m;
            h ^= k;
            data += 4;
            len -= 4;
        }
        // Handle the last few bytes of the input array
        switch (len) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
        };
        // Do a few final mixes of the hash to ensure the last few
        // bytes are well-incorporated.
        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;
        return h;
    }

    /**
     * 将Byte格式化为K,M,G,T,P单位
     */
    static string formatByteSize(double bytes) {
        static const char _sizeunits[] = "KMGTP";
        char s[16];
        int level = 0;
        while (bytes >= 1024.0) {
            bytes /= 1024.0;
            level++;
            if (level >= 5)
                break;
        }

        if (level > 0) {
            snprintf(s, 16, "%.1f%c", bytes, _sizeunits[level - 1]);
        } else {
            snprintf(s, 16, "%d", (int) bytes);
        }
        return s;
    }

    static size_t toByteSize(string strSize) {
        size_t iSize = 0;
        if (strSize.length() < 1)
            return iSize;

        char unit = strSize[strSize.length() - 1];
        switch (unit) {
        case 'K':
        case 'k':
            iSize = 1024;
            break;
        case 'M':
        case 'm':
            iSize = 1024 * 1024;
            break;
        case 'G':
        case 'g':
            iSize = 1024 * 1024 * 1024;
            break;
        default:
            iSize = 1;
            break;
        }

        size_t _tmp = atoi(strSize.c_str());
        iSize = iSize * _tmp;
        return iSize;
    }

    /**
     * 替换字符串
     */
    static string replaceAll(const string &s, const string &s1, const string &s2) {
        string tmp = s;
        int len = s1.length();
        int len2 = s2.length();
        int curPos = 0;
        size_t idx = tmp.find(s1, curPos);

        while (idx != string::npos) {
            tmp.erase(idx, len);
            tmp.insert(idx, s2);
            curPos = idx + len2;
            idx = tmp.find(s1, curPos);
        }
        return tmp;
    }

    /**
     * 忽略大小写比较
     */
    static bool compareIgnoreCase(const string &s1, const string &s2) {
        return toUpper(s1) == toUpper(s2);
    }

    /**
     * 是否以某子串开头
     */
    static bool startWith(const string &s, const string &subs) {
        return (s.find(subs) == 0);
    }

    /**
     * 是否以某子串结尾
     */
    static bool endWith(const string &s, const string &subs) {
        size_t idx = s.length() - subs.length();
        return (s.rfind(subs, idx) == idx);
    }

    /**
     * 将int转为string
     */
    static string fromInt(int i) {
        char tmp[30];
        sprintf(tmp, "%d", i);
        return string(tmp);
    }

    /**
     * 将double转为string
     */
    static string fromDouble(double d) {
        char tmp[50];
        sprintf(tmp, "%G", d);
        return string(tmp);
    }

    /**
     * 将bool转为string
     */
    static string fromBool(bool b) {
        if (b) {
            return "true";
        } else {
            return "false";
        }
    }

    /**
     * 将字符串转换为整数
     * @param s
     * @return
     */
    static int parseInt(string s, int d = -1) {
        if (StringUtil::isInt(s.c_str())) {
            return atoi(s.c_str());
        } else {
            return d;
        }
    }

    /**
     * 将字符串转换为整数
     * @param s
     * @return
     */
    static int parseInt(const char * s, int d = -1) {
        if (StringUtil::isInt(s)) {
            return atoi(s);
        } else {
            return d;
        }
    }

    /**
     * 将字符串转为Double
     */
    static double parseDouble(string s) {
        return atof(s.c_str());
    }

    /**
     * 将字符串转为Double
     */
    static double parseDouble(const char * s) {
        return atof(s);
    }

    /**
     * 将字符串转为Boolean
     */
    static bool parseBool(string s) {
        if (toUpper(s) == "TRUE" || toUpper(s) == "YES" || s == "1") {
            return true;
        } else {
            return false;
        }
    }

    /**
     * 将字符串切分成数组
     */
    static vector<string> split(string s, string d = " ") {
        vector<string> ret;
        char * input;
        input = new char[s.length() + 1];
        strcpy(input, s.c_str());

        const char * delimiters = d.c_str();

        char * p;
        p = strtok(input, delimiters);
        while (p != NULL) {

            ret.push_back(p);

            p = strtok(NULL, delimiters);
        }
        delete[] input;
        return ret;
    }

};

}

#endif /* STRINGUTIL_H_ */
