# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appMyServer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appMyServer_autogen.dir\\ParseCache.txt"
  "appMyServer_autogen"
  )
endif()
