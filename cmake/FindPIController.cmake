# - Try to find pxarCore used in native reader library for exporting RAW to pxarCore format needed for reconstruction
# Once done this will define
#  IPHCRC_FOUND - System has pxarCore
#  IPHCRC_INCLUDE_DIRS - The pxarCore include directories
#  IPHCRC_LIBRARIES - The libraries needed to use pxarCore
#  IPHCRC_DEFINITIONS - Compiler switches required for using pxarCore

MESSAGE(STATUS "Looking for PIController dependencies: lib and header")

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

MESSAGE(${CMAKE_FIND_LIBRARY_SUFFIXES})

find_path(PIController_INCLUDE_DIR PIController.h 
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../producer/pi/include/headers")

MESSAGE(${PIController_INCLUDE_DIR})

# looks for lib[name]
find_library(PIController_LIBRARY PI_GCS2_DLL
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../producer/include/lib")

MESSAGE(${PIController_LIBRARY})

MESSAGE(${CMAKE_FIND_LIBRARY_SUFFIXES})

#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

#find_library(PIController_LIBRARY PI_GCS2_DLL
 # HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../producer/include/lib")

MESSAGE("coolest library ever!!" ${PIController_LIBRARY})

set(PIController_LIBRARIES ${PIController_LIBRARY})
set(PIController_INCLUDE_DIRS ${PIController_INCLUDE_DIR})

MESSAGE(${PIController_INCLUDE_DIR})

IF(PIController_LIBRARY AND PIController_INCLUDE_DIR)
   SET(PIController_FOUND TRUE)
   MESSAGE(STATUS "Found PIController library and headers.")
ELSE(PIController_LIBRARY AND PIController_INCLUDE_DIR)
  MESSAGE(STATUS "dir: " ${PIController_INCLUDE_DIR} " Lib: " ${PIController_LIBRARY})
ENDIF(PIController_LIBRARY AND PIController_INCLUDE_DIR)

mark_as_advanced(PIController_LIBRARY PIController_INCLUDE_DIR)
