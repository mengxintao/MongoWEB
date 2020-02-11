// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com

#include "Channel.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

#include <memory>

class Server {
public:
    Server(EventLoop *loop, int threadNum, int port);
    EventLoop* getLoop() const { return loop_; }
    void start();
    void handNewConn();
    void handThisConn() { loop_->updatePoller(acceptChannel_); }
    ~Server() {}
private:
    EventLoop* loop_;
    int threadNum_;
    std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
    bool started_;
    std::shared_ptr<Channel> acceptChannel_;
    int port_;
    int listenFd_;
    static const int MAXFDS = 100000;
};