
cmake_minimum_required(VERSION 2.8)

#-----------------------------------------------------------------------------
# See http://cmake.org/cmake/help/cmake-2-8-docs.html#section_Policies for details
#

SET(project_policies
  CMP0001 # NEW: CMAKE_BACKWARDS_COMPATIBILITY should no longer be used.
  CMP0002 # NEW: Logical target names must be globally unique.
  CMP0003 # NEW: Libraries linked via full path no longer produce linker search paths.
  CMP0004 # NEW: Libraries linked may NOT have leading or trailing whitespace.
  CMP0005 # NEW: Preprocessor definition values are now escaped automatically.
  CMP0006 # NEW: Installing MACOSX_BUNDLE targets requires a BUNDLE DESTINATION.
  CMP0007 # NEW: List command no longer ignores empty elements.
  CMP0008 # NEW: Libraries linked by full-path must have a valid library file name.
  CMP0009 # NEW: FILE GLOB_RECURSE calls should not follow symlinks by default.
  CMP0010 # NEW: Bad variable reference syntax is an error.
  CMP0011 # NEW: Included scripts do automatic cmake_policy PUSH and POP.
  CMP0012 # NEW: if() recognizes numbers and boolean constants.
  CMP0013 # NEW: Duplicate binary directories are not allowed.
  CMP0014 # NEW: Input directories must have CMakeLists.txt
  )
FOREACH(policy ${project_policies})
  IF(POLICY ${policy})
    CMAKE_POLICY(SET ${policy} NEW)
  ENDIF()
ENDFOREACH()

#-----------------------------------------------------------------------------
project(Log4Qt)
#-----------------------------------------------------------------------------


#-----------------------------------------------------------------------------
# Library type - Statically built
#
set(BUILD_SHARED_LIBS FALSE)

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

#-----------------------------------------------------------------------------
# Sources

file(GLOB LOG4QT_HEADERS
  src/log4qt/*.h
  src/log4qt/varia/*.h
  src/log4qt/spi/*.h
  )

file(GLOB LOG4QT_SOURCE 
  src/log4qt/*.cpp
  src/log4qt/helpers/*.cpp
  src/log4qt/varia/*.cpp
  src/log4qt/spi/*.cpp
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

SOURCE_GROUP("Generated" FILES
  ${LOG4QT_MOC_CXX}
  )
    
#-----------------------------------------------------------------------------
# Build the library

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

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
  SET_TARGET_PROPERTIES(log4qt PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

#-----------------------------------------------------------------------------
# Install rules
foreach(dir "" helpers spi varia)
  file(GLOB InstallHeaders 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log4qt/${dir}/*.h" 
    )
  install( FILES ${InstallHeaders}
    DESTINATION include/log4qt/${dir} COMPONENT Development )
endforeach()

install(TARGETS ${PROJECT_NAME} DESTINATION lib COMPONENT Development )
