// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_MUTEXLOCK_H
#define MONGOWEB_MUTEXLOCK_H
#endif //MONGOWEB_MUTEXLOCK_H

#include "noncopyable.h"
#include <pthread.h>
#include <cstdio>

class MutexLock : noncopyable {
public:
    MutexLock() { pthread_mutex_init(&mutex, NULL); }
    ~MutexLock() {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }
    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }
    pthread_mutex_t *get() { return &mutex; }

private:
    pthread_mutex_t mutex;
    friend class Condition;//友元类
};

class MutexLockGuard : noncopyable {
public:
    explicit MutexLockGuard(MutexLock &_mutex) : mutex(_mutex) { mutex.lock(); }
    ~MutexLockGuard() { mutex.unlock(); }

private:
    MutexLock &mutex;
};