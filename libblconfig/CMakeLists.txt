cmake_minimum_required(VERSION 2.8)

project(blconfig)

find_package (Boost COMPONENTS log log_setup filesystem system thread date_time REQUIRED)
find_package (Threads)


set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(Boost_ALL_DYN_LINK           ON)

add_definitions( -DBOOST_LOG_DYN_LINK )

set(LIB_SOURCE
    logging.cpp
)

include_directories(
    ${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_rvalue_references)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_lambdas)

export_include_dirs(
    ${PROJECT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)

export_libs(
    ${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
)
