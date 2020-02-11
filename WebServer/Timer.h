// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_TIMIER_H
#define MONGOWEB_TIMIER_H

#endif //MONGOWEB_TIMIER_H

#include "HttpData.h"
#include "base/MutexLock.h"
#include "base/noncopyable.h"
#include <unistd.h>
#include <deque>
#include <memory>
#include <queue>

class HttpData; //前向声明

class TimerNode {
public:
    TimerNode(std::shared_ptr<HttpData> requestData, int timeout);
    ~TimerNode();
    TimerNode(TimerNode &tn);
    void update(int timeout);
    bool isValid();
    void clearReq();
    void setDeleted() { deleted_ = true; }
    bool isDeleted() const { return deleted_; }
    size_t getExpTime() const { return expiredTime_; }

private:
    bool deleted_;
    size_t expiredTime_;
    std::shared_ptr<HttpData> SPHttpData;
};

struct TimerCmp {
    bool operator()(std::shared_ptr<TimerNode> &a,
                    std::shared_ptr<TimerNode> &b) const {
        return a->getExpTime() > b->getExpTime();
    }
};


class TimerManager {
public:
    TimerManager();
    ~TimerManager();
    void addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout);
    void handleExpiredEvent();

private:
    std::priority_queue<std::shared_ptr<TimerNode>, std::deque<std::shared_ptr<TimerNode>>, TimerCmp>
            timerNodeQueue;
};