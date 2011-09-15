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
    OUTPUT_NONE    = 0,
    OUTPUT_VERBOSE = 1,
    OUTPUT_COLOR   = 2,
    OUTPUT_FULL    = OUTPUT_VERBOSE|OUTPUT_COLOR
};

static inline bool 
isVerbose(OutputOptions options) 
{ return options & OUTPUT_VERBOSE; }

static inline bool 
isColor(OutputOptions options) 
{ return options & OUTPUT_COLOR; }
/// @}

// Helper Types
typedef void (*TestFunc)();
typedef std::string		    String;
typedef std::exception		    Exception;
typedef std::pair<String, TestFunc> TestEntry;
typedef std::map<String, TestFunc>  TestMap;
typedef TestMap::const_iterator	    TestIterator;

/// Unit Test Manager
class TestManager
{
public: // methods
    TestManager() : m_options(OUTPUT_NONE) {}

    bool addTest(
	const String& quoted_path,
       	int line,
       	TestFunc func)
    {
	if (func == NULL)
	    return false;

	const String& name = getTestName(quoted_path, line);
	m_tests[name] = func;
	return true;
    }

    bool addTest(
	const String& name,
	const String& quoted_path,
       	int line,
       	TestFunc func)
    {
	if (func == NULL)
	    return false;

	const String& full_name = getTestName(name, quoted_path, line);
	m_tests[full_name] = func;
	return true;
    }

    void runTests(OutputOptions options = OUTPUT_FULL)
    {
	if (m_tests.empty()) {
	    std::cout << m_defaultColors << "No tests defined." << std::endl;
	    return;
	}

	m_options = options;
	int num_tests = m_tests.size();
	int passed_tests = 0;

	std::cout << "Running " << num_tests << " test" 
	    << (num_tests > 1 ? "s" : "") << "...\n" << std::endl;
	for (TestIterator it = m_tests.begin(); it != m_tests.end(); ++it) {
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

    String getTestName(
	const String& name,
	const String& quoted_path,
	int line)
    {
	std::stringstream ss;
	ss << "\"" << name << "\" " << getTestName(quoted_path, line);
	return ss.str();
    }

    String getTestName(const String& quoted_path, int line)
    {
	size_t end = quoted_path.find_last_of("\"");
	size_t last_slash = quoted_path.find_last_of("\\/");
	size_t first_quote = quoted_path.find_first_of("\"");
	size_t begin = 0;
	if (last_slash != String::npos)
	    begin = last_slash+1;
	else if (first_quote != String::npos)
	    begin = first_quote+1;

	std::stringstream ss;
	ss << "at " << quoted_path.substr(begin, end) << ":" << line;
	return ss.str();
    }

    bool runTest(const String& name, TestFunc test_func)
    {
	bool passed = true;
	try {
	    printTestName(name);
	    StreamRedirector redirect(m_new_cout, m_new_cerr);
	    test_func();
	} catch (const Exception& ex) {
	    printFailed(name, ex.what());
	    return false;
	}
	printPassed(name);
	return passed;
    }

    void printResult(int num_tests, int passed_tests)
    {
	if (passed_tests == 0)
	    std::cout << fg_red;
	else if (passed_tests == num_tests)
	    std::cout << fg_green;
	else
	    std::cout << fg_yellow;

	std::cout << "\n" << passed_tests << " of " << num_tests << " tests passed.\n"
	    << m_defaultColors << std::endl;
    }

    void printTestName(const String& name)
    {
	if (!isVerbose(m_options))
	    return;

	std::cout << "Running test " << name << "... ";
	std::cout.flush();
    }

    void printPassed(const String& name)
    {
	if (!isVerbose(m_options))
	    return;

	std::cout << fg_green << "passed" << m_defaultColors << std::endl;
    }

    void printFailed(const String& name)
    {
	if (!isVerbose(m_options))
	    std::cout << "Test " << name << " ";
	std::cout << fg_red << "failed" << m_defaultColors << std::endl;
    }

    void printFailed(const String& name, const String& error)
    {
	printFailed(name);
	std::cout << fg_yellow << "\n*** " << error << m_defaultColors 
	    << std::endl;
    }

private: // members
    TestMap             m_tests;
    ConsoleColors       m_defaultColors;
    OutputOptions       m_options;
    std::stringstream   m_new_cout;
    std::stringstream   m_new_cerr;
};

// Global Variables
TestManager& getTestManager();

} // namespace

#endif // UNIT_TEST_IMPL_H_

