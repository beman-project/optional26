# cmake-format: off
# etc/clang-flags.cmake -*- cmake -*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

include_guard(GLOBAL)

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_FLAGS
    "-std=c++${CMAKE_CXX_STANDARD} -Wall -Wextra -stdlib=libc++"
    CACHE STRING "CXX_FLAGS" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG
    "-O0 -fno-inline -g3"
    CACHE STRING "C++ DEBUG Flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE
    "-Ofast -g0 -DNDEBUG"
    CACHE STRING "C++ Release Flags" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "-O3 -g -DNDEBUG"
    CACHE STRING "C++ RelWithDebInfo Flags" FORCE)
set(CMAKE_CXX_FLAGS_TSAN
    "-O3 -g -DNDEBUG -fsanitize=thread"
    CACHE STRING "C++ TSAN Flags" FORCE)
set(CMAKE_CXX_FLAGS_ASAN
    "-O3 -g -DNDEBUG -fsanitize=address,undefined,leak"
    CACHE STRING "C++ ASAN Flags" FORCE)
