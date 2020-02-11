// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//
#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "Util.h"
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <queue>

using namespace std;

Channel::Channel(EventLoop *loop)
        : loop_(loop),
        events_(0),
        lastEvents_(0),
        fd_(0){}

Channel::Channel(EventLoop *loop, int fd)
        : loop_(loop),
        fd_(fd),
        events_(0),
        lastEvents_(0){}

Channel::~Channel(){}

int Channel::getFd(){
    return fd_;
}

void Channel::setFd(int fd){
    fd_ = fd;
}
void Channel::handleRead() {
    if (readHandler_) {
        readHandler_();
    }
}

void Channel::handleWrite() {
    if (writeHandler_) {
        writeHandler_();
    }
}

void Channel::handleConn() {
    if (connHandler_) {
        connHandler_();
    }
}