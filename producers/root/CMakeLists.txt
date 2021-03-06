GET_FILENAME_COMPONENT(DIRNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
SET(PLUGIN_NAME plugin.${DIRNAME})
SET_PROPERTY(GLOBAL APPEND PROPERTY CONVERTER_PLUGIN ${PLUGIN_NAME})

INCLUDE_DIRECTORIES(include)

SET(PLUGIN_SRC
  src/SCTConverterPlugin.cc
  src/SCTUpgradeConverterPlugin.cc
  )
ADD_LIBRARY(${PLUGIN_NAME} SHARED ${PLUGIN_SRC})
INSTALL(TARGETS ${PLUGIN_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )



FIND_PACKAGE(ROOT REQUIRED)

INCLUDE_DIRECTORIES(include ${ROOT_INCLUDE_DIR})

SET(ROOTProducer_name ROOTProducer)
SET(ROOTProducer_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/${ROOTProducer_name}.cxx)
SET(ROOTProducer_SOURCES
  src/ROOTProducer.cpp
  src/DemoHardware.cpp
  )


ROOT_GENERATE_DICTIONARY(${ROOTProducer_name}  ROOTProducer.h DemoHardware.h)
ADD_LIBRARY(${ROOTProducer_name} SHARED ${ROOTProducer_SOURCES} ${ROOTProducer_DICTIONARY})
TARGET_LINK_LIBRARIES(${ROOTProducer_name} EUDAQ ${EUDAQ_THREADS_LIB} ${ROOT_LIBRARIES} ${ADDITIONAL_LIBRARIES})

INSTALL(TARGETS ${ROOTProducer_name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

IF(found_root_major_vers GREATER 5)
  SET(ROOTProducer_PCM ${CMAKE_CURRENT_BINARY_DIR}/${ROOTProducer_name}_rdict.pcm)
  INSTALL(FILES
    ${ROOTProducer_PCM}
    DESTINATION lib
    )
ENDIF(found_root_major_vers GREATER 5)
