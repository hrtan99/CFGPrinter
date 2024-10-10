cmake_minimum_required(VERSION 3.10.3)
project(CFGPrinter)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

include(AddLLVM)

# The plugin expects to not link against the Support and Core libraries,
# but expects them to exist in the process loading the plugin. This doesn't
# work with DLLs on Windows (where a shared library can't have undefined
# references), so just skip this on Windows.
if (NOT WIN32 AND NOT CYGWIN)
  add_llvm_pass_plugin(CFGPrinter
    src/CFGPrinter.cpp
    src/DomTreePrinter.cpp
    src/HeatCallPrinter.cpp
    src/HeatCFGPrinter.cpp
    src/HeatUtils.cpp
    DEPENDS
    intrinsics_gen
    BUILDTREE_ONLY
  )
endif()
