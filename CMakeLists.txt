cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 20)
#add_compile_options(-std=c++2a -fconcepts)

include_directories(
        ${PROJECT_SOURCE_DIR}/src/
        ${PROJECT_SOURCE_DIR}/bin/
)
set(src_dir
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/bin/command.cpp
        ${PROJECT_SOURCE_DIR}/bin/bookdatabase.cpp
        ${PROJECT_SOURCE_DIR}/bin/account.cpp
        ${PROJECT_SOURCE_DIR}/bin/log.cpp
        ${PROJECT_SOURCE_DIR}/bin/Exception.h
        ${PROJECT_SOURCE_DIR}/bin/ull.h
        ${PROJECT_SOURCE_DIR}/bin/file_io.h
        )
add_executable(code ${src_dir})

# 添加链接库
#target_link_libraries(code ${PROJECT_SOURCE_DIR}/bin/libUll.a)

# 后缀名为 .a 和 .so 的文件分别为静态链接库和动态链接库