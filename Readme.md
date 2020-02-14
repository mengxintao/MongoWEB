## MongoWEB是C++11编写的Web静态服务器

特点：可解析get、head请求，处理静态资源，支持HTTP长连接，支持管线化请求，实现了异步日志（muduo方法），记录服务器运行状态。

## 特点

使用c++11，使用智能指针等RAII机制管理资源。


使用Reactor模式，


使用IO多路复用Epoll的ET的方法，非阻塞IO。


使用多线程充分利用多核CPU，并使用线程池避免线程频繁创建销毁的开销


使用eventfd异步唤醒阻塞在epoll_wait上的线程


使用基于小根堆的定时器关闭超时请求（priority_queue)。


主线程只负责accept请求，并以Round Robin的方式分发给其它IO线程，锁的争用只会出现在主线程和某一特定线程


使用状态机解析了HTTP请求,支持管线化请求


使用双缓冲区技术实现了简单的异步日志系统（muduo方法）


优雅关闭连接shutdown 


## 并发模型

使用Reactor模型，并使用多线程提高并发度。

除Log线程外，每个线程一个事件循环，遵循One loop per thread。

为避免线程频繁创建和销毁带来的开销，使用线程池，在程序的开始创建固定数量的线程。

使用epoll作为IO多路复用的实现方式。Web静态服务器计算量较小，所以没有分配计算线程。

每个SubReactor持有一个定时器，用于处理超时请求和长时间不活跃的连接。

MainReactor负责响应client的连接请求。在建立连接后用Round Robin的方式分配给某个SubReactor,因为涉及到跨线程任务分配，需要加锁，这里的锁由某个特定线程中的loop创建，只会被该线程和主线程竞争。SubReactor有一个或者多个，每个subReactor都会在一个独立线程中运行。当主线程把新连接分配给了某个SubReactor，该线程此如果正阻塞在多路选择器(epoll)的等待中，使用了eventfd进行异步唤醒，线程会从epoll_wait中醒来，得到活跃事件，进行处理。

## Channel类


Channel是Reactor结构中的“事件”，它自始至终都属于一个EventLoop，负责一个文件描述符的IO事件，在Channel类中保存这IO事件的类型以及对应的回调函数，当IO事件发生时，最终会调用到Channel类中的回调函数。因此，程序中所有带有读写时间的对象都会和一个Channel关联，包括loop中的eventfd，listenfd，HttpData等。 

## EventLoop 


One loop per thread意味着每个线程只能有一个EventLoop对象，EventLoop即是时间循环，每次从poller里拿活跃事件，并给到Channel里分发处理。EventLoop中的loop函数会在最底层(Thread)中被真正调用，开始无限的循环，直到某一轮的检查到退出状态后从底层一层一层的退出。
  
  
## log异步日志 

学习了muduo，Log的实现分为前端和后端，前端往后端写，后端往磁盘写。

前端是IO线程，负责产生log，后端是Log线程，负责收集前端产生的log采用muduo双缓冲思想，设计了4个多的缓冲区，4个缓冲区分两组，每组的两个一个主要的，另一个防止第一个写满了，写满或者时间到了就和另外两个交换指针，然后把满的往磁盘写。

与Log相关的类包括FileUtil、LogFile、AsyncLogging、LogStream、Logging。 其中前4个类每一个类都含有一个append函数，Log的设计也是主要围绕这个append函数展开的。
FileUtil是最底层的文件类，封装了Log文件的打开、写入并在类析构的时候关闭文件，底层使用了标准IO，该append函数直接向文件写。
LogFile进一步封装了FileUtil，并设置了一个循环次数，每过这么多次就flush一次。
AsyncLogging是核心，它负责启动一个log线程，专门用来将log写入LogFile，应用了“双缓冲技术”，其实有4个以上的缓冲区，但思想是一样的。AsyncLogging负责(定时到或被填满时)将缓冲区中的数据写入LogFile中。
LogStream主要用来格式化输出，重载了<<运算符，同时也有自己的一块缓冲区，这里缓冲区的存在是为了缓存一行，把多个<<的结果连成一块。
Logging是对外接口，Logging类内涵一个LogStream对象，主要是为了每次打log的时候在log之前和之后加上固定的格式化的信息，比如打log的行、文件名等信息。

