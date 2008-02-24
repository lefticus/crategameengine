PROJECT (crategameengine)

INCLUDE (FindDoxygen)

IF (APPLE)
  INCLUDE_DIRECTORIES (/usr/local/include/boost-1_34)
  LINK_DIRECTORIES (/usr/local/lib)
ENDIF (APPLE)

IF (DOXYGEN_FOUND)
  MESSAGE(STATUS "Running doxygen...")
  EXECUTE_PROCESS(COMMAND ${DOXYGEN_EXECUTABLE} OUTPUT_QUIET)
ENDIF (DOXYGEN_FOUND)

#SET (PERSONALITY_SRCS game_personality/personality.cpp)
#SET (MAIN_SRCS main.cpp)
FILE (GLOB PERSONALITY_SRCS game_personality/*.cpp)
FILE (GLOB MAIN_SRCS *.cpp)

INCLUDE_DIRECTORIES (. ./game_personality)
ADD_EXECUTABLE (crategameengine ${MAIN_SRCS} ${PERSONALITY_SRCS})
IF (APPLE)
  TARGET_LINK_LIBRARIES (crategameengine boost_thread-mt)
ELSE (APPLE) 
  TARGET_LINK_LIBRARIES (crategameengine boost_thread)
ENDIF (APPLE)
