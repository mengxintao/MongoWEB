// @Copyright：Copyright (c) 2019
// @Author Meng xintao
// @Version 1.0
// @email  anmxt@vip.qq.com
#include <getopt.h>
#include <string>
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"


int main(int argc, char *argv[]) {
    int threadNum = 4; //设置线程数
    int port = 80;
    std::string logPath = "./MongoWEB.log";
    // parse args
    int opt;
    const char *str = "t:l:p:";   //命令行选项
    while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {
            case 't': {
                threadNum = atoi(optarg);
                break;
            }
            case 'l': {
                logPath = optarg;
                if (logPath.size() < 2 || optarg[0] != '/') {
                    printf("logPath should start with \"/\"\n");
                    abort();
                }
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            default:
                break;
        }
    }
    Logger::setLogFileName(logPath);
// 多线程
#ifndef _PTHREADS
    LOG << "_PTHREADS is not defined !";
#endif
    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}