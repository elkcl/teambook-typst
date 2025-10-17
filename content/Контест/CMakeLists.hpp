cmake_minimum_required(VERSION 3.14) project(mrc)

    set(CMAKE_CXX_STANDARD 17) set(
        CMAKE_CXX_FLAGS
        "-Wall -Wextra -pedantic -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -fsanitize=address,undefined,\
signed-integer-overflow,pointer-compare,\
pointer-subtract,shadow-call-stack,\
leak,bounds,pointer-overflow -fno-sanitize-recover -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -DONPC")

        add_executable(A A.cpp)
