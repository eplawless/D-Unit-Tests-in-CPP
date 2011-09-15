#pragma once
#ifndef UNIT_TEST_MAIN_H_
#define UNIT_TEST_MAIN_H_

// NOTE: To enable unit tests, define ENABLE_UNIT_TESTS in your build.

/// @{
/// Unit Tests Enabled
#ifdef ENABLE_UNIT_TESTS

#include "UnitTestImpl.h"
#include "UnitTestHelpers.h"

#define UNIT_TEST_HOOKS \
    namespace UNIT_TEST_NS { \
       	TestManager& getTestManager() { \
	    static TestManager manager; \
	    return manager; \
	} \
	WindowsConsole& getConsole() { \
	    static WindowsConsole console; \
	    return console; \
	} \
    }

#define RUN_UNIT_TESTS \
    UNIT_TEST_NS::getTestManager().runTests(); \
    exit(0);

#define STRINGIFY_HELPER(a) #a

#define UNIT_TEST_HELPER(file, line, counter) \
    static void _unittest_##line##counter(); \
    static bool _unittest_##line##counter##added = \
	UNIT_TEST_NS::getTestManager().addTest( \
	    file, line, _unittest_##line##counter); \
    static void _unittest_##line##counter()

#define UNIT_TEST_INTERMEDIATE_HELPER(file, line, counter) \
    UNIT_TEST_HELPER(file, line, counter)

#define unittest \
    UNIT_TEST_INTERMEDIATE_HELPER(__FILE__, __LINE__, __COUNTER__)

#define REQUIRE_COUT_EQUAL(a) \
    UNIT_TEST_NS::require_stream_equal("REQUIRE_COUT_EQUAL", std::cout, (a));

#define REQUIRE_COUT_PREFIX(a) \
    UNIT_TEST_NS::require_stream_prefix("REQUIRE_COUT_PREFIX", std::cout, (a));

#define REQUIRE_CERR_EQUAL(a) \
    UNIT_TEST_NS::require_stream_equal("REQUIRE_CERR_EQUAL", std::cerr, (a));

#define REQUIRE_CERR_PREFIX(a) \
    UNIT_TEST_NS::require_stream_prefix("REQUIRE_CERR_PREFIX", std::cerr, (a));

#define REQUIRE_EQUAL(a, b) \
    UNIT_TEST_NS::require_equal(#a, #b, a, b)

#define REQUIRE_CLOSE(a, b) \
    UNIT_TEST_NS::require_close(#a, #b, a, b)

#define REQUIRE(a) \
    UNIT_TEST_NS::require(#a, a)
/// @}

/// @{
/// Unit Tests Disabled
#else

#define UNIT_TEST_HELPER(file, line, counter) \
    template<typename T> static void deadFunction##line##counter()

#define UNIT_TEST_INTERMEDIATE_HELPER(file, line, counter) \
    UNIT_TEST_HELPER(file, line, counter)

#define unittest \
    UNIT_TEST_INTERMEDIATE_HELPER(__FILE__, __LINE__, __COUNTER__)

#define UNIT_TEST_HOOKS
#define RUN_UNIT_TESTS
#define REQUIRE_COUT_EQUAL(a)
#define REQUIRE_COUT_PREFIX(a)
#define REQUIRE_CERR_EQUAL(a)
#define REQUIRE_CERR_PREFIX(a)
#define REQUIRE_EQUAL(a, b)
#define REQUIRE_CLOSE(a, b)
#define REQUIRE(a)

#endif
/// @}

#endif // UNIT_TEST_MAIN_H_
