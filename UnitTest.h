#pragma once
#ifndef UNIT_TEST_MAIN_H_
#define UNIT_TEST_MAIN_H_

// NOTE: To enable unit tests, define ENABLE_UNIT_TESTS in your build.

/// @{
/// Unit Tests Enabled
#ifdef ENABLE_UNIT_TESTS

#include "UnitTestImpl.h"
#include "UnitTestHelpers.h"

#define RUN_UNIT_TESTS \
    UNIT_TEST_NS::g_testManager.runTests(); \
    exit(0);

#define STRINGIFY_HELPER(a) #a

#define UNIT_TEST_HELPER(name, testNum) \
    void _unittest_##name##testNum(); \
    static bool _unittest_##name##testNum##added = \
	UNIT_TEST_NS::g_testManager.addTest(#name, _unittest_##name##testNum); \
    void _unittest_##name##testNum()

#define UNIT_TEST_INTERMEDIATE_HELPER(name, testNum) \
    UNIT_TEST_HELPER(name, testNum)

#define testsuite namespace

#define unittest(testname) \
    UNIT_TEST_INTERMEDIATE_HELPER(testname, __LINE__)

#define unittesthook \
    template <typename T> void _testhook()

#define classunittest(classname, testname) \
    class testname##classname##Key {}; \
    void run##testname##classname##test(); \
    template<> void classname::_testhook<testname##classname##Key>(); \
    static bool _unittest_##testname##classname##added = \
	UNIT_TEST_NS::g_testManager.addTest( \
	    STRINGIFY_HELPER(classname::testname), \
	    run##testname##classname##test); \
    void run##testname##classname##test() { \
	classname instance; \
	instance._testhook<testname##classname##Key>(); \
    } \
    template<> void classname::_testhook<testname##classname##Key>() 

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

#define RUN_UNIT_TESTS

#define UNIT_TEST_HELPER(name, testNum) \
    template<typename T> void deadFunction##name##testNum()

#define UNIT_TEST_INTERMEDIATE_HELPER(name, testNum) \
    UNIT_TEST_HELPER(name, testNum)

#define unittest(testname) \
    UNIT_TEST_INTERMEDIATE_HELPER(testname, __LINE__)

#define testsuite namespace

#define unittesthook \
    template <typename T> void _testhook()

#define classunittest(classname, testname) \
    class testname##classname##Key {}; \
    template<> void classname::_testhook<testname##classname##Key>() 

#define REQUIRE_EQUAL(a, b)

#define REQUIRE_CLOSE(a, b)

#define REQUIRE(a)

#endif
/// @}

#endif // UNIT_TEST_MAIN_H_
