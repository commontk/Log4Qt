
CONFIGURE_FILE(
  ${Log4Qt_SOURCE_DIR}/UseLog4Qt.cmake.in
  ${Log4Qt_BINARY_DIR}/UseLog4Qt.cmake COPYONLY)

# Library directory
SET(Log4Qt_LIBRARY_DIRS_CONFIG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Include directories
SET(Log4Qt_INCLUDE_DIRS_CONFIG 
  ${Log4Qt_SOURCE_DIR}/src
  #${Log4Qt_SOURCE_DIR}/src/log4qt
  #${Log4Qt_SOURCE_DIR}/src/log4qt/helpers
  #${Log4Qt_SOURCE_DIR}/src/log4qt/spi
  #${Log4Qt_SOURCE_DIR}/src/log4qt/varia
  )

# UseLog4Qt file
SET(Log4Qt_USE_FILE_CONFIG ${Log4Qt_BINARY_DIR}/UseLog4Qt.cmake)

# Export Targets file
SET(Log4Qt_TARGETS_FILE "${Log4Qt_BINARY_DIR}/Log4QtTargets.cmake")

# Configure Log4QtConfig.cmake
CONFIGURE_FILE(
  ${Log4Qt_SOURCE_DIR}/Log4QtConfig.cmake.in
  ${Log4Qt_BINARY_DIR}/Log4QtConfig.cmake @ONLY)
