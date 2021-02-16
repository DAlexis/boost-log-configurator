cmake_minimum_required(VERSION 3.1)

project(blconfig)

find_package (Boost COMPONENTS log log_setup filesystem system thread date_time REQUIRED)
find_package (Threads)

#set(Boost_USE_STATIC_LIBS        OFF)
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME     OFF)
#set(Boost_ALL_DYN_LINK           ON)

add_definitions( -DBOOST_LOG_DYN_LINK )

set(LIB_SOURCE
    logging.cpp
)

include_directories(
    ${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)
