cmake_minimum_required(VERSION 2.8)

project(blconfig-sample)

add_definitions( -DBOOST_LOG_DYN_LINK )

set(EXE_SOURCES
    main.cpp
)

include_directories(
    ${blconfig_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_rvalue_references)

target_link_libraries (${PROJECT_NAME}
    ${blconfig_LIBRARY}
)


