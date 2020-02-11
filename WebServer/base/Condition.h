// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_CONDITION_H
#define MONGOWEB_CONDITION_H

#endif //MONGOWEB_CONDITION_H

#include "MutexLock.h"
#include "noncopyable.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <cstdint>

class Condition : noncopyable {
public:
    explicit Condition(MutexLock &_mutex) : mutex(_mutex) {
        pthread_cond_init(&cond, NULL);
    }
    ~Condition() { pthread_cond_destroy(&cond); }

    void wait() { pthread_cond_wait(&cond, mutex.get()); }
    void notify() { pthread_cond_signal(&cond); }
    void notifyAll() { pthread_cond_broadcast(&cond); }
    bool waitForSeconds(int seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }

private:
    MutexLock &mutex;
    pthread_cond_t cond;
};