cmake_minimum_required(VERSION 3.9)
project(myProject)
IF(GPU)
#在命令行中使用cmake -DGPU，会进入这一行，C++代码中自动会有#define GPU
ADD_DEFINITIONS(-DGPU) #注意一定要有-D
ENDIF(GPU)

link_library_dictonary(path)
include_library_dictonary(path)

link_xxx()

add_executable(a.out testCmake.cc)