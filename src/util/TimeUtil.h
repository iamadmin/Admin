/*
 * TimeUtil.h
 *
 *  Created on: 2013-3-24
 *      Author: tomzhou
 */

#ifndef TIMEUTIL_H_
#define TIMEUTIL_H_

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

namespace util {

class TimeUtil {
public:

    /**
     * 将时间转换为20120320183050的格式
     */
    static char * timeToStr(time_t t, char *dest) {
        struct tm r;
        memset(&r, 0, sizeof(r));
        if (localtime_r((const time_t*) &t, &r) == NULL) {
            dest[0] = '\0';
            return dest;
        }
        sprintf(dest, "%04d%02d%02d%02d%02d%02d", r.tm_year + 1900, r.tm_mon + 1, r.tm_mday, r.tm_hour, r.tm_min, r.tm_sec);
        return dest;
    }


    /**
     * 将20120320183050格式的字符串转换为时间
     */
    static int strToTime(char *str) {
        if (str == NULL || strlen(str) != 14) {
            return 0;
        }
        char *p = str;
        while ((*p)) {
            if ((*p) < '0' || (*p) > '9')
                return 0;
            p++;
        }

        struct tm t;
        t.tm_year = (str[0] - '0') * 1000 + (str[1] - '0') * 100 + (str[2] - '0') * 10 + (str[3] - '0') - 1900;
        t.tm_mon = (str[4] - '0') * 10 + (str[5] - '0') - 1;
        t.tm_mday = (str[6] - '0') * 10 + (str[7] - '0');
        t.tm_hour = (str[8] - '0') * 10 + (str[9] - '0');
        t.tm_min = (str[10] - '0') * 10 + (str[11] - '0');
        t.tm_sec = (str[12] - '0') * 10 + (str[13] - '0');
        t.tm_isdst = 0;
        int t1 = mktime(&t);
        return t1;
    }

};

}

#endif /* TIMEUTIL_H_ */
