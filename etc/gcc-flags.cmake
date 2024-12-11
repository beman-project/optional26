include_guard(GLOBAL)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "-std=c++23 -Wall -Wextra " CACHE STRING "CXX_FLAGS" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG
    "-O0 -fno-inline -g3"
    CACHE STRING
    "C++ DEBUG Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELEASE
    "-Ofast -g0 -DNDEBUG"
    CACHE STRING
    "C++ Release Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "-O3 -g -DNDEBUG"
    CACHE STRING
    "C++ RelWithDebInfo Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_TSAN
    "-O3 -g -DNDEBUG -fsanitize=thread"
    CACHE STRING
    "C++ TSAN Flags"
    FORCE
)
if(APPLE)
    set(CMAKE_CXX_FLAGS_ASAN
        "-O3 -g -DNDEBUG -fsanitize=address,undefined"
        CACHE STRING
        "C++ ASAN Flags"
        FORCE
    )
else()
    set(CMAKE_CXX_FLAGS_ASAN
        "-O3 -g -DNDEBUG -fsanitize=address,undefined,leak"
        CACHE STRING
        "C++ ASAN Flags"
        FORCE
    )
endif()

set(CMAKE_CXX_FLAGS_GCOV
    "-O0 -fno-inline -g --coverage -fprofile-abs-path"
    CACHE STRING
    "C++ GCOV Flags"
    FORCE
)

set(CMAKE_LINKER_FLAGS_GCOV "--coverage" CACHE STRING "Linker GCOV Flags" FORCE)
