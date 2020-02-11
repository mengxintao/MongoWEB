// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_EVENTLOOPTHREAD_H
#define MONGOWEB_EVENTLOOPTHREAD_H
#endif

#include "EventLoop.h"
#include "base/Condition.h"
#include "base/MutexLock.h"
#include "base/Thread.h"
#include "base/noncopyable.h"

class EventLoopThread : noncopyable {
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop* startLoop();

private:
    void threadFunc();
    EventLoop* loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};