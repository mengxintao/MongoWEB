// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_EVENTLOOP_H
#define MONGOWEB_EVENTLOOP_H
#endif //MONGOWEB_EVENTLOOP_H

#include "Channel.h"
#include "Epoll.h"
#include "Util.h"
#include "base/CurrentThread.h"
#include "base/Logging.h"
#include "base/Thread.h"
#include <functional>
#include <memory>
#include <vector>
#include <iostream>

using namespace std;

class EventLoop {
public:
    typedef std::function<void()> FuncPtr;
    EventLoop();
    ~EventLoop();
    void loop();   //循环
    void quit();
    void runInLoop(FuncPtr&& cb); //关键函数
    void queueInLoop(FuncPtr&& cb);//

    bool isInLoopThread() const{
        return threadId_ == CurrentThread::tid();
    }
    void assertInLoopThread(){
        assert(isInLoopThread());
    }
    void shutdown(shared_ptr<Channel>channel){
        shutDownWR(channel->getFd());
    }
    void removeFromPoller(shared_ptr<Channel>channel){
        poller_->epoll_del(channel);
    }
    void updatePoller(shared_ptr<Channel>channel,int timeout = 0){
        poller_->epoll_mod(channel, timeout);
    }
    void addToPoller(shared_ptr<Channel>channel,int timeout = 0){
        poller_->epoll_add(channel, timeout);
    }

private:
    // 注意声明顺序
    bool looping_;
    shared_ptr<Epoll> poller_;
    int wakeupFd_;
    bool quit_;
    bool eventHandling_;
    mutable MutexLock mutex_
    std::vector<FuncPtr> pendingFunctors_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    shared_ptr<Channel> pwakeupChannel_;

    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleConn();
}