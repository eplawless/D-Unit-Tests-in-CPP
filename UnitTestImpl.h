#pragma once
#ifndef UNIT_TEST_IMPL_H_
#define UNIT_TEST_IMPL_H_

#include <iostream>
#include <algorithm>
#include <exception>
#include <string>
#include <map>
#include <sstream>
#include <limits>
#include <iomanip>

#include "NS.h"
#include "ConsoleColors.h"

namespace UNIT_TEST_NS {

/// @{
/// Output Options
enum OutputOptions 
{
    OUTPUT_NONE    		= 0x0,
    OUTPUT_VERBOSE 		= 0x1,
    OUTPUT_COLOR   		= 0x2,
	OUTPUT_REDIRECT_STREAMS = 0x4,
    OUTPUT_FULL    		= OUTPUT_VERBOSE|OUTPUT_COLOR|OUTPUT_REDIRECT_STREAMS
};

/// @{
/// Default Output Options
static const int DEFAULT_OUTPUT_OPTIONS = OUTPUT_NONE
#ifndef UNIT_TESTS_DISABLE_COLOR
	| OUTPUT_COLOR
#endif
#ifndef UNIT_TESTS_QUIET
	| OUTPUT_VERBOSE
#endif
#ifdef UNIT_TESTS_ENABLE_STREAM_REDIRECT
	| OUTPUT_REDIRECT_STREAMS
#endif
	;
/// @}

static inline bool 
isVerbose(int options) 
{ return (options & OUTPUT_VERBOSE) != 0; }

static inline bool 
isColor(int options) 
{ return (options & OUTPUT_COLOR) != 0; }

static inline bool
shouldRedirectStreams(int options)
{ return (options & OUTPUT_REDIRECT_STREAMS) != 0; }
/// @}

// Helper Types
typedef void (*TestFunc)();
struct TestEntry
{
    std::string name;
    TestFunc function;
};
typedef std::map<std::string, TestEntry>  TestMap;
typedef TestMap::const_iterator	    TestIterator;

/// Unit Test Manager
class TestManager
{
public: // methods
    TestManager() : m_options(OUTPUT_NONE) {}

    bool addTest(
	const std::string& quoted_path,
       	int line,
       	TestFunc func)
    {
	if (func == NULL)
	    return false;

	const std::string& id = getTestId(quoted_path, line);
	m_tests[id].function = func;
	return true;
    }

    bool addTest(
	const std::string& name,
	const std::string& quoted_path,
       	int line,
       	TestFunc func)
    {
	if (func == NULL)
	    return false;

	const std::string& id = getTestId(quoted_path, line);
	TestEntry& entry = m_tests[id];
	entry.name = name;
	entry.function = func;
	return true;
    }

    void runTests(int options = DEFAULT_OUTPUT_OPTIONS)
    {
	if (m_tests.empty()) {
	    std::cout << m_defaultColors << "No tests defined." << std::endl;
	    return;
	}

	m_options = options;
	int num_tests = m_tests.size();
	int passed_tests = 0;

	isColor(m_options) ? enableColor() : disableColor();

	std::cout << "Running " << num_tests << " test" 
	    << (num_tests > 1 ? "s" : "") << "..." << std::endl;

	int test_num = 1;
	for (TestIterator it = m_tests.begin(); it != m_tests.end(); ++it) {
	    printPercentCompletion(test_num++, num_tests);
	    passed_tests += runTest(it->first, it->second);
	}

	printResult(num_tests, passed_tests);
    }

private: // types

    class StreamRedirector 
    {
    public: // methods

	template <typename OutStream, typename ErrStream>
	StreamRedirector(OutStream& out, ErrStream& err) 
	{
	    out.str("");
	    err.str("");
	    cout_backup = std::cout.rdbuf();
	    cerr_backup = std::cerr.rdbuf();
	    std::cout.rdbuf(out.rdbuf());
	    std::cerr.rdbuf(err.rdbuf());
	}

	~StreamRedirector()
	{
	    std::cout.rdbuf(cout_backup);
	    std::cerr.rdbuf(cerr_backup);
	}

    private: // members

	std::streambuf* cout_backup;
	std::streambuf* cerr_backup;
    };

private: // methods

    std::string getTestId(const std::string& quoted_path, int line)
    {
	size_t end = quoted_path.find_last_of("\"");
	size_t last_slash = quoted_path.find_last_of("\\/");
	size_t first_quote = quoted_path.find_first_of("\"");
	size_t begin = 0;
	if (last_slash != std::string::npos)
	    begin = last_slash+1;
	else if (first_quote != std::string::npos)
	    begin = first_quote+1;

	std::stringstream ss;
	ss << "at " << quoted_path.substr(begin, end) << ":" << line;
	return ss.str();
    }

    std::string getTestString(const std::string& id, const std::string& name)
    {
	return (name.empty() ? "" : "\""+name+"\" ") + id;
    }

    bool runTest(const std::string& id, const TestEntry& test)
    {
	std::string test_string = getTestString(id, test.name);
	try {
	    printTestName(test_string);
		if (shouldRedirectStreams(m_options)) {
			StreamRedirector redirect(m_new_cout, m_new_cerr);
			test.function();
		} else {
			test.function();
		}
	} catch (const std::exception& ex) {
	    printFailed(test_string, ex.what());
	    return false;
	}
	printPassed(test_string);
	return true;
    }

    void printPercentCompletion(int test_num, int num_tests)
    {
	if (!isVerbose(m_options))
	    return;
	std::cout << m_defaultColors << "[";
	std::cout.width(3);
	std::cout << ((test_num*100)/num_tests) << "%] ";
    }

    void printResult(int num_tests, int passed_tests)
    {
	if (passed_tests == 0)
	    std::cout << m_errorColor;
	else if (passed_tests == num_tests)
	    std::cout << m_successColor;
	else
	    std::cout << m_warningColor;

	std::cout << "\n" << passed_tests << " of " << num_tests << " tests passed.\n"
	    << m_defaultColors << std::endl;
    }

    void printTestName(const std::string& name)
    {
	if (!isVerbose(m_options))
	    return;

	std::cout << "Running test " << name << "... ";
	std::cout.flush();
    }

    void printPassed(const std::string& name)
    {
	if (!isVerbose(m_options))
	    return;

	std::cout << m_successColor << "passed" << m_defaultColors << std::endl;
    }

    void printFailed(const std::string& name)
    {
	if (!isVerbose(m_options))
	    std::cout << "Test " << name << " has ";
	std::cout << m_errorColor << "failed" << m_defaultColors << std::endl;
    }

    void printFailed(const std::string& name, const std::string& error)
    {
	printFailed(name);
	std::cout << m_warningColor << "*** " << error << m_defaultColors 
	    << std::endl;
    }

    void enableColor() 
    {
	m_successColor = fg_green;
	m_warningColor = fg_yellow;
	m_errorColor = fg_red;
    }

    void disableColor()
    {
	m_successColor = fg_null;
	m_warningColor = fg_null;
	m_errorColor = fg_null;
    }

private: // members
    TestMap             m_tests;
    ConsoleColors       m_defaultColors;
    StreamManipFunc	m_successColor;
    StreamManipFunc	m_warningColor;
    StreamManipFunc	m_errorColor;
    int       			m_options;
    std::stringstream   m_new_cout;
    std::stringstream   m_new_cerr;
};

// Global Variables
TestManager& getTestManager();

} // namespace

#endif // UNIT_TEST_IMPL_H_

