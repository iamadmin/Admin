/*
 * Timer.h
 *
 *  Created on: 2011-12-17
 *      Author: tomzhou
 *      Usage:
 *      Timer timer;
 *      timer.start();
 *      //do something...
 *      timer.stop();
 *      TimeDiff td = timer.getTimeDiff();
 *      printf("Use %d.%03d seconds",  td.lSeconds,  td.lMilliseconds);
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
#include <unistd.h>

namespace util {

/**
 * Structure that represents the time difference in seconds and milliseconds.
 */
struct TimeDiff {
    unsigned long lSeconds; /**< Time difference in seconds */
    unsigned long lMilliseconds; /**< Number of milliseconds in addition to number of seconds */
};

class Timer {
private:
    timeval _sysTimeStart;
    timeval _sysTimeStop;
    TimeDiff _timeDiff;
public:
    Timer() {

    }
    virtual ~Timer() {

    }

    inline void start() {
        gettimeofday(&_sysTimeStart, NULL);
    }

    /**
     * stop timer and calculate duration
     */
    inline void stop() {
        // time (seconds and milliseconds) since Epoch
        gettimeofday(&_sysTimeStop, NULL);
        if (_sysTimeStop.tv_usec < _sysTimeStart.tv_usec) {
            _sysTimeStop.tv_usec += 1000000L;
            _sysTimeStop.tv_sec--;
        }
        _timeDiff.lSeconds = _sysTimeStop.tv_sec - _sysTimeStart.tv_sec;
        // convert microseconds to milliseconds
        _timeDiff.lMilliseconds = (_sysTimeStop.tv_usec - _sysTimeStart.tv_usec) / 1000;
    }

    /**
     * Returns a const reference to a CtsTimeDiff representing the time difference
     */
    inline const TimeDiff& getTimeDiff() const {
        return _timeDiff;
    }
};

}

#endif /* TIMER_H_ */
