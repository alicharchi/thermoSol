set(GTEST_PROJECT gtestProject)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

include(FetchContent)
#set(GTEST_PROJECT gtestProject)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
include_directories("${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include")
include_directories("${CMAKE_BINARY_DIR}/_deps/googletest-src/googlemock/include")

include_directories(${INC_DIR})

set(Libraries
  GTest::GTest
  PhaseEquil
)

add_executable(VaporPressure VaporPressure.cpp)
target_link_libraries(VaporPressure PRIVATE ${Libraries})
add_test(API_VaporPressure VaporPressure)


#Copy test input files
#file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/InputFiles" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

set(SOURCES
  "VaporPressure.cpp"
  "runTests.cpp"
)

add_executable(runAllTests ${SOURCES})
target_link_libraries(runAllTests PUBLIC
  gtest
  gtest_main
  ${Libraries}
)
