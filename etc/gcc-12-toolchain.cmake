include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/gcc-flags.cmake")

set(CMAKE_C_COMPILER gcc-12)
set(CMAKE_CXX_COMPILER g++-12)
set(GCOV_EXECUTABLE "gcov-12" CACHE STRING "GCOV executable" FORCE)
