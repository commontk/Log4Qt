
CONFIGURE_FILE(
  ${Log4Qt_SOURCE_DIR}/UseLog4Qt.cmake.in
  ${Log4Qt_BINARY_DIR}/UseLog4Qt.cmake COPYONLY)

# Include directories
SET(Log4Qt_INCLUDE_DIRS_CONFIG ${Log4Qt_SOURCE_DIR}/src)

# UseLog4Qt file
SET(Log4Qt_USE_FILE_CONFIG ${Log4Qt_BINARY_DIR}/UseLog4Qt.cmake)

# Configure Log4QtConfig.cmake
CONFIGURE_FILE(
  ${Log4Qt_SOURCE_DIR}/Log4QtConfig.cmake.in
  ${Log4Qt_BINARY_DIR}/Log4QtConfig.cmake @ONLY)
