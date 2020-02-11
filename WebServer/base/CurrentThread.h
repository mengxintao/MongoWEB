// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_CURRENTTHREAD_H
#define MONGOWEB_CURRENTTHREAD_H

#endif //MONGOWEB_CURRENTTHREAD_H

#include <stdint.h>

namespace CurrentThread {
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* t_threadName;
    void cacheTid();
    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {
            cacheTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString()  // for logging
    {
        return t_tidString;
    }

    inline int tidStringLength()  // for logging
    {
        return t_tidStringLength;
    }

    inline const char* name() { return t_threadName; }
}
