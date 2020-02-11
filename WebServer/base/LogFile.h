// @Copyright：Copyright (c) 2019
// @Author Meng xintao 
// @Version 1.0
// @email  anmxt@vip.qq.com
//

#ifndef MONGOWEB_LOGFILE_H
#define MONGOWEB_LOGFILE_H
#endif /

#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"
#include <memory>
#include <string>

class LogFile:noncopyable {
public:
    // 每被append flushEveryN次，就flush一下往文件写写。不过，文件也是带缓冲区的
    LogFile(const std::string& basename, int flushEveryN = 1024);
    ~LogFile();
    void append(const char* logline, int len);
    void flush();
    bool rollFile();
private:
    void append_unlocked(const char* logline, int len);
    const std::string basename_;
    const int flushEveryN_;
    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
};