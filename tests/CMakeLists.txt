cmake_minimum_required(VERSION 3.15)

project(mlfq_test C)

add_executable(mlfq-test
    mlfq_test.c
    compact.c
    ../kernel/multi-queue.c
)

target_include_directories(mlfq-test PRIVATE
    ../kernel
    .
)
