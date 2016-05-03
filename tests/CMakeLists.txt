
#
# Setup CXXTESTS for serialmessages
#
# @author Natesh Narain
#

find_package(CxxTest REQUIRED)

include_directories(
	${INCL}
	${CXXTEST_INCLUDE_DIRS}
)

cxxtest_add_test(test_serial_stream test_serial_stream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_serial_stream.h
)

cxxtest_add_test(test_hash_table test_hash_table.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test_hash_table.h
)



