// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#include "EventLoop.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "Util.h"
#include "base/Logging.h"
#include <iostream>

using namespace std;

__thread EventLoop* t_loopInThisThread = 0;

int createEventfd() {
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC); //创建eventfd，非阻塞
    if (evtfd < 0) {
        LOG << "Failed in eventfd";
        abort();
    }
    return evtfd;
}

EventLoop::EventLoop()
        : looping_(false),
          poller_(new Epoll()),
          wakeupFd_(createEventfd()),
          quit_(false),
          eventHandling_(false),
          callingPendingFunctors_(false),
          threadId_(CurrentThread::tid()),
          pwakeupChannel_(new Channel(this, wakeupFd_)) {
    if (t_loopInThisThread) {}
    else {
        t_loopInThisThread = this;
    }
    pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);
    pwakeupChannel_->setReadHandler(bind(&EventLoop::handleRead, this));
    pwakeupChannel_->setConnHandler(bind(&EventLoop::handleConn, this));
    poller_->epoll_add(pwakeupChannel_, 0);
}

void EventLoop::handleConn() {
    updatePoller(pwakeupChannel_, 0);//添加
}

EventLoop::~EventLoop() {
    close(wakeupFd_);
    t_loopInThisThread = NULL;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = writen(wakeupFd_, (char*)(&one), sizeof one);
    if (n != sizeof one) {
        LOG << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = readn(wakeupFd_, &one, sizeof one);
    if (n != sizeof one) {
        LOG << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
    pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);  //设置读事件，ET触发模式
}

void EventLoop::runInLoop(FuncPtr&& cb) {
    if (isInLoopThread())
        cb();
    else
        queueInLoop(std::move(cb));
}

void EventLoop::queueInLoop(FuncPtr&& cb) {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.emplace_back(std::move(cb)); //减少类对象的创建，只调用一次构造函数
    if(!isInLoopThread() || callingPendingFunctors_) wakeup();
}

void EventLoop::loop() {
    assert(!looping_);    //one loop per thread
    assert(isInLoopThread());
    looping_ = true;
    {
        quit_ = false;
        std::vector <shared_ptr<Channel>> ret;
        while (!quit_) {
            ret.clear();
            ret = poller_->poll();
            eventHandling_ = true;
            {
                for (auto &it : ret) it->handleEvents();//处理channel中的事件
            }
            eventHandling_ = false;
            doPendingFunctors();
            poller_->handleExpired(); //处理超时事件
        }
    }
    looping_ = false;
}

void EventLoop::doPendingFunctors() {
    std::vector<FuncPtr> functors;
    callingPendingFunctors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
        for (size_t i = 0; i < functors.size(); ++i) functors[i]();
    }
    callingPendingFunctors_ = false;
}

void EventLoop::quit() {
    quit_ = true;
    if (!isInLoopThread()) {
        wakeup();
    }
}