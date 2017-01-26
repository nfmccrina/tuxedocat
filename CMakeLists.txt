cmake_minimum_required(VERSION 3.5.1)
project("tuxedocat")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_PROFILE ${CMAKE_CXX_FLAGS_PROFILE} "-O3 -pg")

set(TUXEDOCAT_SOURCES "src/CastlingStatus.cpp" "src/Color.cpp" "src/Controller.cpp" "src/DivideMessage.cpp" "src/Engine.cpp" "src/Interface.cpp" "src/LookupData.cpp" "src/Main.cpp" "src/MessageArgument.cpp" "src/Message.cpp" "src/MessageQueue.cpp" "src/Move.cpp" "src/MoveList.cpp" "src/PerftMessage.cpp" "src/Piece.cpp" "src/Position.cpp" "src/PrintMessage.cpp" "src/QuitMessage.cpp" "src/Rank.cpp" "src/SANEngine.cpp" "src/SetBoardMessage.cpp" "src/UndoMoveMessage.cpp" "src/UserMoveMessage.cpp" "src/Utility.cpp")

include_directories(include "include" "test_include")

set(GOOGLETEST_DIR googletest)
set(TUXEDOCAT_TEST_SOURCES "test_src/Main.cpp" "test_src/MoveTest.cpp" "test_src/PieceTest.cpp" "test_src/UtilityTest.cpp" "test_src/EngineTest.cpp" "test_src/SANEngineTest.cpp" "src/CastlingStatus.cpp" "src/Color.cpp" "src/Controller.cpp" "src/DivideMessage.cpp" "src/Engine.cpp" "src/Interface.cpp" "src/LookupData.cpp" "src/MessageArgument.cpp" "src/Message.cpp" "src/MessageQueue.cpp" "src/Move.cpp" "src/MoveList.cpp" "src/PerftMessage.cpp" "src/Piece.cpp" "src/Position.cpp" "src/PrintMessage.cpp" "src/QuitMessage.cpp" "src/Rank.cpp" "src/SANEngine.cpp" "src/SetBoardMessage.cpp" "src/UndoMoveMessage.cpp" "src/UserMoveMessage.cpp" "src/Utility.cpp")

option(BUILD_GTEST "Builds the googletest subproject" ON)
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
add_subdirectory(${GOOGLETEST_DIR})

find_package(Threads REQUIRED)

add_executable(tuxedocat ${TUXEDOCAT_SOURCES})
target_link_libraries(tuxedocat ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(tuxedocat PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tuxedocat PUBLIC cxx_long_long_type)

add_executable(tuxedocat_test ${TUXEDOCAT_TEST_SOURCES})
set_target_properties(tuxedocat_test gtest PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(tuxedocat_test gtest)
target_compile_features(tuxedocat_test PUBLIC cxx_long_long_type)
target_compile_features(gtest PUBLIC cxx_long_long_type)
