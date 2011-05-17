
cmake_minimum_required(VERSION 2.8)

#-----------------------------------------------------------------------------
project(Log4Qt)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Output directories.
#
SET(output_dir ${Log4Qt_BINARY_DIR}/bin)
FOREACH(type LIBRARY RUNTIME ARCHIVE)
  IF(NOT DEFINED CMAKE_${type}_OUTPUT_DIRECTORY)
    SET(CMAKE_${type}_OUTPUT_DIRECTORY ${output_dir} CACHE INTERNAL "Single output directory for building all libraries.")
  ENDIF()
ENDFOREACH()

#-----------------------------------------------------------------------------
# Library type - Statically built
#
set(BUILD_SHARED_LIBS FALSE)

#-----------------------------------------------------------------------------
# Set symbol visibility Flags
#
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  SET(VISIBILITY_CXX_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
ENDIF()

#-----------------------------------------------------------------------------
# Update C/CXX Flags
#
SET(CMAKE_CXX_FLAGS "${VISIBILITY_CXX_FLAGS} ${CMAKE_CXX_FLAGS}" CACHE STRING "CMake CXX Flags")

#-----------------------------------------------------------------------------
# Qt
set(minimum_required_qt_version "4.6")

find_package(Qt4)

if(QT4_FOUND)
  set(found_qt_version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH})
  
  if(${found_qt_version} VERSION_LESS ${minimum_required_qt_version})
      message(FATAL_ERROR "error: PythonQt requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${found_qt_version}.")
  endif()
  
  include(${QT_USE_FILE})
else()
  message(FATAL_ERROR "error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
endif()


# --------------------------------------------------------------------------
# Include dirs
set(log4qt_includes
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR}/src
  )
include_directories(${log4qt_includes})

#-----------------------------------------------------------------------------
# Sources

file(GLOB LOG4QT_SOURCE
  src/log4qt/*.cpp
  src/log4qt/helpers/*.cpp
  src/log4qt/spi/*.cpp
  src/log4qt/varia/*.cpp
  )

# Headers that should run through moc
set(LOG4QT_MOC_SRCS
  src/log4qt/appenderskeleton.h
  src/log4qt/appender.h
  src/log4qt/layout.h
  src/log4qt/consoleappender.h
  src/log4qt/dailyrollingfileappender.h
  src/log4qt/fileappender.h
  src/log4qt/helpers/configuratorhelper.h
  src/log4qt/helpers/logobject.h
  src/log4qt/logger.h
  src/log4qt/patternlayout.h
  src/log4qt/rollingfileappender.h
  src/log4qt/simplelayout.h
  src/log4qt/spi/filter.h
  src/log4qt/ttcclayout.h
  src/log4qt/varia/debugappender.h
  src/log4qt/varia/denyallfilter.h
  src/log4qt/varia/levelmatchfilter.h
  src/log4qt/varia/levelrangefilter.h
  src/log4qt/varia/listappender.h
  src/log4qt/varia/nullappender.h
  src/log4qt/varia/stringmatchfilter.h
  src/log4qt/writerappender.h
)

#-----------------------------------------------------------------------------
# Do wrapping

qt4_wrap_cpp(LOG4QT_MOC_CXX ${LOG4QT_MOC_SRCS})

source_group("Generated" FILES
  ${LOG4QT_MOC_CXX}
  )
    
#-----------------------------------------------------------------------------
# Build the library

add_library(${PROJECT_NAME}
  ${LOG4QT_SOURCE} 
  ${LOG4QT_MOC_CXX}
  )

#-----------------------------------------------------------------------------
#
# To fix compilation problem: relocation R_X86_64_32 against `a local symbol' can not be
# used when making a shared object; recompile with -fPIC
# See http://www.cmake.org/pipermail/cmake/2007-May/014350.html
#
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

#-----------------------------------------------------------------------------
# Install rules
foreach(dir . helpers spi varia)
  file(GLOB InstallHeaders 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log4qt/${dir}/*.h" 
    )
  install( FILES ${InstallHeaders}
    DESTINATION include/log4qt/${dir} COMPONENT Development )
endforeach()

install(TARGETS ${PROJECT_NAME} DESTINATION lib COMPONENT Development )
#-----------------------------------------------------------------------------
# Generate Log4Qt Config
#
include(GenerateLog4QtConfig.cmake)

