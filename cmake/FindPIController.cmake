MESSAGE(STATUS "Looking for PIController dependencies: lib and header")

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

MESSAGE(${CMAKE_FIND_LIBRARY_SUFFIXES})

find_path(PIController_INCLUDE_DIR PIController.h 
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../producers/pi/include/")

MESSAGE(${PIController_INCLUDE_DIR})

# looks for lib[name]
find_library(PIController_LIBRARY PI_GCS2_DLL
  HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../producers/pi/include/lib")

MESSAGE(${PIController_LIBRARY})

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
