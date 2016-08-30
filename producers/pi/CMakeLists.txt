set(namecontrol "PIControl.exe")
set(nameproducer "PIProducer.exe")

FIND_PACKAGE(PIController REQUIRED)

IF(PIController_FOUND)
ELSE()
#  MESSAGE(FATAL_ERROR "couldn't find lib or header check line abouve for details ")
ENDIF()

set(sourcefilecontrol src/PIController.cc src/PIControl.cxx)

set(sourcefileproducer src/PIProducer.cxx src/PIController.cc)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/include/lib/PI_GCS2_DLL.dll" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../../bin")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${namecontrol} ${sourcefilecontrol})
add_executable(${nameproducer} ${sourcefileproducer})

MESSAGE(${PIController_LIBRARIES} "   if this shows directorie of the lib, then everything is fine")

target_link_libraries(${namecontrol} EUDAQ ${PIController_LIBRARIES})
target_link_libraries(${nameproducer} EUDAQ ${PIController_LIBRARIES})

#MESSAGE(${PIController_LIBRARIES} "   " ${PIController_INCLUDE_DIRS})

INSTALL(TARGETS PIControl.exe PIProducer.exe
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)