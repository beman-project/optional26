include_guard(GLOBAL)

set(CMAKE_C_COMPILER clang-18)
set(CMAKE_CXX_COMPILER clang++-18)
set(GCOV_EXECUTABLE "llvm-cov-18 gcov" CACHE STRING "GCOV executable" FORCE)

include("${CMAKE_CURRENT_LIST_DIR}/clang-flags.cmake")
