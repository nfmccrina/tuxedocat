cmake_minimum_required(VERSION 3.5.1)
project("tuxedocat")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_PROFILE ${CMAKE_CXX_FLAGS_PROFILE} "-O3 -pg")

set(TUXEDOCAT_SOURCES "src/CastlingStatus.cpp" "src/Color.cpp" "src/Controller.cpp" "src/DivideMessage.cpp" "src/Engine.cpp" "src/Interface.cpp" "src/LookupData.cpp" "src/Main.cpp" "src/MessageArgument.cpp" "src/Message.cpp" "src/MessageQueue.cpp" "src/Move.cpp" "src/MoveList.cpp" "src/PerftMessage.cpp" "src/Piece.cpp" "src/Position.cpp" "src/PrintMessage.cpp" "src/QuitMessage.cpp" "src/Rank.cpp" "src/SANEngine.cpp" "src/SetBoardMessage.cpp" "src/UndoMoveMessage.cpp" "src/UserMoveMessage.cpp" "src/Utility.cpp")

include_directories(include "./include")

add_subdirectory("test")

find_package(Threads REQUIRED)

add_executable(tuxedocat ${TUXEDOCAT_SOURCES})
target_link_libraries(tuxedocat ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(tuxedocat PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(tuxedocat PUBLIC cxx_long_long_type)
