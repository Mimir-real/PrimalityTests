project(primality_tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(test_primality_tests tests.cpp PrimalityTests.h)
target_link_libraries(test_primality_tests gmpxx)
target_link_libraries(test_primality_tests gmp)
