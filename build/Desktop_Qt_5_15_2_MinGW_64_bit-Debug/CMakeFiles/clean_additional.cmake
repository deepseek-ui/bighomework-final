# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\tablecat02_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\tablecat02_autogen.dir\\ParseCache.txt"
  "tablecat02_autogen"
  )
endif()
