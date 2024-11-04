include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/gcc-flags.cmake")

set(CMAKE_C_COMPILER gcc-13)
set(CMAKE_CXX_COMPILER g++-13)
set(GCOV_EXECUTABLE "gcov-13" CACHE STRING "GCOV executable" FORCE)
