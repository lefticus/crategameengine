PROJECT (crategameengine)

FIND_PACKAGE (SWIG REQUIRED)
INCLUDE (${SWIG_USE_FILE})

INCLUDE (FindDoxygen)

IF (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

IF (APPLE)
  INCLUDE_DIRECTORIES (/usr/local/include/boost-1_34)
ENDIF (APPLE)

INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR})


FIND_LIBRARY(BOOST_THREAD_LIBRARY_DEBUG NAMES boost_thread-mt-d boost_thread-d 
  PATHS  /usr/lib /usr/local/lib)

FIND_LIBRARY(BOOST_THREAD_LIBRARY_RELEASE NAMES boost_thread-mt boost_thread
  PATHS  /usr/lib /usr/local/lib)

SET (LINK_LIBS_RELEASE ${BOOST_THREAD_LIBRARY_RELEASE})

IF(NOT BOOST_THREAD_LIBRARY_DEBUG AND NOT BOOST_THREAD_LIBRARY_RELEASE)
  MESSAGE(SEND_ERROR "Can not find the Boost thread library.")
ENDIF(NOT BOOST_THREAD_LIBRARY_DEBUG AND NOT BOOST_THREAD_LIBRARY_RELEASE)

IF(BOOST_THREAD_LIBRARY_DEBUG)
  #MESSAGE(STATUS "BOOST Debug libraries found: ${BOOST_THREAD_LIBRARY_DEBUG}")
  SET (LINK_LIBS_DEBUG ${BOOST_THREAD_LIBRARY_DEBUG})
ELSE(BOOST_THREAD_LIBRARY_DEBUG)
  IF (CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    MESSAGE(STATUS "BOOST Debug Libraries NOT found and building in DEBUG MODE")
  ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
  SET (LINK_LIBS_DEBUG ${BOOST_THREAD_LIBRARY_RELEASE})
ENDIF(BOOST_THREAD_LIBRARY_DEBUG)

IF(BOOST_THREAD_LIBRARY_RELEASE)
  MESSAGE(STATUS "BOOST Release libraries found: ${BOOST_THREAD_LIBRARY_RELEASE}")
  SET (LINK_LIBS_DEBUG ${BOOST_THREAD_LIBRARY_RELEASE})
ELSE(BOOST_THREAD_LIBRARY_RELEASE)
  IF (CMAKE_BUILD_TYPE MATCHES ".*Rel.*")
    MESSAGE(STATUS "BOOST Release Libraries NOT found and building in RELEASE MODE")
  ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Rel.*")
  SET (LINK_LIBS_RELEASE ${BOOST_THREAD_LIBRARY_DEBUG})
ENDIF(BOOST_THREAD_LIBRARY_RELEASE)

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/world_script_access_wrap.hxx
  COMMAND swig -lua -nodefaultctor -c++ -o ${CMAKE_CURRENT_BINARY_DIR}/world_script_access_wrap.hxx lib/game_script_handlers/lua_handler/lua.i
  DEPENDS lib/game_script_handlers/lua_handler/lua.i lib/game_script_handlers/lua_handler/lua_handler.cpp
  )

FILE (GLOB PERSONALITY_SRCS lib/game_personality/*.cpp)
FILE (GLOB GAME_ENGINE_SRCS lib/game_engine/*.cpp)
FILE (GLOB MEMORY_WORLD_SRCS lib/game_worlds/memory/*.cpp)
FILE (GLOB VIEWER_STUB_SRCS lib/game_viewers/stub/*.cpp)
FILE (GLOB LUA_HANDLER_SRCS lib/game_script_handlers/lua_handler/*.cpp)


ADD_LIBRARY (game_personality SHARED ${PERSONALITY_SRCS})
ADD_LIBRARY (game_engine MODULE ${GAME_ENGINE_SRCS})
ADD_LIBRARY (memory_world MODULE ${MEMORY_WORLD_SRCS})
ADD_LIBRARY (viewerstub MODULE ${VIEWER_STUB_SRCS})
ADD_LIBRARY (lua_handler MODULE ${CMAKE_CURRENT_BINARY_DIR}/world_script_access_wrap.hxx ${LUA_HANDLER_SRCS})

#SET(CMAKE_SWIG_FLAGS "")

#SET_SOURCE_FILES_PROPERTIES(lib/game_script_handlers/lua_handler/lua.i PROPERTIES CPLUSPLUS ON)
#SET_SOURCE_FILES_PROPERTIES(lib/game_script_handlers/lua_handler/lua.i
#  PROPERTIES SWIG_FLAGS "-nodefaultctor")

#SWIG_ADD_MODULE(world_script_access_lua lua
#    lib/game_script_handlers/lua_handler/lua.i
#    lib/game_script_handlers/lua_handler/lua_handler.cpp)
#SWIG_LINK_LIBRARIES(world_script_access_lua lua)



FILE (GLOB MAIN_SRCS game_loader/*.cpp )

INCLUDE_DIRECTORIES (./include ./lib)
ADD_EXECUTABLE (crategameengine ${MAIN_SRCS} ${LIBS_SRCS})
TARGET_LINK_LIBRARIES (crategameengine dl game_personality lua debug ${LINK_LIBS_DEBUG} optimized ${LINK_LIBS_RELEASE})
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
