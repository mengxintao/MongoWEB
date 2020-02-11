// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_EPOLL_H
#define MONGOWEB_EPOLL_H
#endif //MONGOWEB_EPOLL_H

#include "Channel.h"
#include "HttpData.h"
#include "Timer.h"
#include <sys/epoll.h>
#include <memory>
#include <unordered_map>
#include <vector>

class Epoll {
public:
    Epoll();
    ~Epoll();
    void epoll_add(std::shared_ptr<Channel> request, int timeout);
    void epoll_mod(std::shared_ptr<Channel> request, int timeout);
    void epoll_del(std::shared_ptr<Channel> request);
    std::vector<std::shared_ptr<Channel>> poll();
    std::vector<std::shared_ptr<Channel>> getEventsRequest(int events_num);
    void add_timer(std::shared_ptr<Channel> request_data, int timeout);
    int getEpollFd() { return epollFd_; }
    void handleExpired();
private:
    static const int MAXFDS = 100000;
    int epollFd_;
    std::vector<epoll_event> events_;
    std::shared_ptr<Channel> fd2chan_[MAXFDS];
    std::shared_ptr<HttpData> fd2http_[MAXFDS];
    TimerManager timerManager_;
};



