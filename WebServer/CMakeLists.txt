set(SRCS
        HttpData.cpp
        #ThreadPool.cpp
        )
include_directories(${PROJECT_SOURCE_DIR}/base)


add_executable(WebServer ${SRCS})
target_link_libraries(WebServer libserver_base)


add_subdirectory(base)
