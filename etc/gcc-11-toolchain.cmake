include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/gcc-flags.cmake")

set(CMAKE_C_COMPILER gcc-11)
set(CMAKE_CXX_COMPILER g++-11)
set(GCOV_EXECUTABLE
    "gcov-11"
    CACHE STRING "GCOV executable" FORCE)
