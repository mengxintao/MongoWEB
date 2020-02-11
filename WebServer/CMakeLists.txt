set(SRCS
        HttpData.cpp
        #ThreadPool.cpp
        )
include_directories(${PROJECT_SOURCE_DIR}/base)


add_executable(WEBServer ${SRCS})
target_link_libraries(WEBServer libserver_base)


add_subdirectory(base)