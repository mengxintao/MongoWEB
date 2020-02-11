// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_EVENTLOOPTREADPOOL_H
#define MONGOWEB_EVENTLOOPTREADPOOL_H
#endif

#include "EventLoopThread.h"
#include "base/Logging.h"
#include "base/noncopyable.h"
#include <memory>
#include <vector>

class EventLoopThreadPool : noncopyable {
public:
    EventLoopThreadPool(EventLoop* baseLoop, int numThreads);
    ~EventLoopThreadPool() {
        LOG<<"~EventLoopThreadPool()";
    }
    void start();

    EventLoop* getNextLoop();

private:
    EventLoop* baseLoop_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};