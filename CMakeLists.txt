cmake_minimum_required(VERSION 3.7)
project("tuxedocat")

set(TUXEDOCAT_ROOT "..")
set(GTEST_DIR "${TUXEDOCAT_ROOT}/googletest")

include_directories(SYSTEM ${GTEST_DIR}/include)
include_directories(SYSTEM ${GTEST_DIR})

add_library(gtest STATIC ../googletest/src/gtest-all.cc)