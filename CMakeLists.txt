PROJECT (crategameengine)

INCLUDE (FindDoxygen)

OPTION (DISABLE_DEBUG "Use the release versions of compiler flags and linked libraries")

IF (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

IF (APPLE)
  INCLUDE_DIRECTORIES (/usr/local/include/boost-1_34)
ENDIF (APPLE)

IF (DISABLE_DEBUG)
  FIND_LIBRARY(BOOST_THREAD_LIBRARY NAMES boost_thread-mt boost_thread boost_thread-mt-d boost_thread-d
    PATHS  /usr/lib /usr/local/lib)
  set (CMAKE_BUILD_TYPE Release)
ELSE (DISABLE_DEBUG)
  FIND_LIBRARY(BOOST_THREAD_LIBRARY NAMES boost_thread-mt-d boost_thread-d boost_thread-mt boost_thread
       PATHS  /usr/lib /usr/local/lib)
  set (CMAKE_BUILD_TYPE Debug)
ENDIF (DISABLE_DEBUG)

IF (BOOST_THREAD_LIBRARY STREQUAL "") 
  MESSAGE(SEND_ERROR "Can not find Boost library")
ENDIF (BOOST_THREAD_LIBRARY STREQUAL "") 

SET (LINK_LIBS ${BOOST_THREAD_LIBRARY})

#SET (PERSONALITY_SRCS game_personality/personality.cpp)
#SET (MAIN_SRCS main.cpp)
FILE (GLOB PERSONALITY_SRCS game_personality/*.cpp)
FILE (GLOB MAIN_SRCS *.cpp)

INCLUDE_DIRECTORIES (. ./game_personality)
ADD_EXECUTABLE (crategameengine ${MAIN_SRCS} ${PERSONALITY_SRCS})
TARGET_LINK_LIBRARIES (crategameengine ${LINK_LIBS} debug ${LINK_LIBS} optimized ${LINK_LIBS} )
IF (DOXYGEN_FOUND)
  ADD_CUSTOM_COMMAND(TARGET crategameengine POST_BUILD COMMAND ${DOXYGEN_EXECUTABLE}) 
ENDIF (DOXYGEN_FOUND)

# Stub for testing
#ENABLE_TESTING()
# Build a binary for each group of tests
#ADD_EXECUTABLE (TestPersonality ${PERSONALITY_SRCS})
#TARGET_LINK_LIBRARIES (TestPersonality debug ${LINK_LIBS_DEBUG} optimized ${LINK_LIBS_RELEASE})
# Then add the test so the test suite can find it
#ADD_TEST (TestPersonalityTest TestPersonality)
# To run the test after the binaries have been built, execute 'ctest -V' in the same directory as the CMakeLists.txt
