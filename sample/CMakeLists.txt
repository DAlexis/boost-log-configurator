cmake_minimum_required(VERSION 3.1)

project(blconfig-sample)

add_definitions( -DBOOST_LOG_DYN_LINK )

set(EXE_SOURCES
    main.cpp
)

include_directories(
    ${blconfig_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)

target_link_libraries (${PROJECT_NAME} PUBLIC
    blconfig
)


