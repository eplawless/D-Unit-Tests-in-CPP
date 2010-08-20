#pragma once
#ifndef UNITTESTS_HEADER
#define UNITTESTS_HEADER

#ifdef RUN_UNIT_TESTS

#include <iostream>
#include <algorithm>
#include <exception>
#include <string>
#include <map>
#include <sstream>
#include <limits>
#include <iomanip>

// w/e, fuck linux
#include <windows.h>

// stolen outright
namespace JadedHoboConsole
{
	static const WORD bgMask( BACKGROUND_BLUE      | 
		BACKGROUND_GREEN     | 
		BACKGROUND_RED       | 
		BACKGROUND_INTENSITY   );
	static const WORD fgMask( FOREGROUND_BLUE      | 
		FOREGROUND_GREEN     | 
		FOREGROUND_RED       | 
		FOREGROUND_INTENSITY   );

	static const WORD fgBlack    ( 0 ); 
	static const WORD fgLoRed    ( FOREGROUND_RED   ); 
	static const WORD fgLoGreen  ( FOREGROUND_GREEN ); 
	static const WORD fgLoBlue   ( FOREGROUND_BLUE  ); 
	static const WORD fgLoCyan   ( fgLoGreen   | fgLoBlue ); 
	static const WORD fgLoMagenta( fgLoRed     | fgLoBlue ); 
	static const WORD fgLoYellow ( fgLoRed     | fgLoGreen ); 
	static const WORD fgLoWhite  ( fgLoRed     | fgLoGreen | fgLoBlue ); 
	static const WORD fgGray     ( fgBlack     | FOREGROUND_INTENSITY ); 
	static const WORD fgHiWhite  ( fgLoWhite   | FOREGROUND_INTENSITY ); 
	static const WORD fgHiBlue   ( fgLoBlue    | FOREGROUND_INTENSITY ); 
	static const WORD fgHiGreen  ( fgLoGreen   | FOREGROUND_INTENSITY ); 
	static const WORD fgHiRed    ( fgLoRed     | FOREGROUND_INTENSITY ); 
	static const WORD fgHiCyan   ( fgLoCyan    | FOREGROUND_INTENSITY ); 
	static const WORD fgHiMagenta( fgLoMagenta | FOREGROUND_INTENSITY ); 
	static const WORD fgHiYellow ( fgLoYellow  | FOREGROUND_INTENSITY );
	static const WORD bgBlack    ( 0 ); 
	static const WORD bgLoRed    ( BACKGROUND_RED   ); 
	static const WORD bgLoGreen  ( BACKGROUND_GREEN ); 
	static const WORD bgLoBlue   ( BACKGROUND_BLUE  ); 
	static const WORD bgLoCyan   ( bgLoGreen   | bgLoBlue ); 
	static const WORD bgLoMagenta( bgLoRed     | bgLoBlue ); 
	static const WORD bgLoYellow ( bgLoRed     | bgLoGreen ); 
	static const WORD bgLoWhite  ( bgLoRed     | bgLoGreen | bgLoBlue ); 
	static const WORD bgGray     ( bgBlack     | BACKGROUND_INTENSITY ); 
	static const WORD bgHiWhite  ( bgLoWhite   | BACKGROUND_INTENSITY ); 
	static const WORD bgHiBlue   ( bgLoBlue    | BACKGROUND_INTENSITY ); 
	static const WORD bgHiGreen  ( bgLoGreen   | BACKGROUND_INTENSITY ); 
	static const WORD bgHiRed    ( bgLoRed     | BACKGROUND_INTENSITY ); 
	static const WORD bgHiCyan   ( bgLoCyan    | BACKGROUND_INTENSITY ); 
	static const WORD bgHiMagenta( bgLoMagenta | BACKGROUND_INTENSITY ); 
	static const WORD bgHiYellow ( bgLoYellow  | BACKGROUND_INTENSITY );

	static class con_dev
	{
	private:
		HANDLE                      hCon;
		DWORD                       cCharsWritten; 
		CONSOLE_SCREEN_BUFFER_INFO  csbi; 
		DWORD                       dwConSize;

	public:
		con_dev() 
		{ 
			hCon = GetStdHandle( STD_OUTPUT_HANDLE );
		}
	private:
		void GetInfo()
		{
			GetConsoleScreenBufferInfo( hCon, &csbi );
			dwConSize = csbi.dwSize.X * csbi.dwSize.Y; 
		}
	public:
		void Clear()
		{
			COORD coordScreen = { 0, 0 };

			GetInfo(); 
			FillConsoleOutputCharacter( hCon, ' ',
				dwConSize, 
				coordScreen,
				&cCharsWritten ); 
			GetInfo(); 
			FillConsoleOutputAttribute( hCon,
				csbi.wAttributes,
				dwConSize,
				coordScreen,
				&cCharsWritten ); 
			SetConsoleCursorPosition( hCon, coordScreen ); 
		}
		void SetColor( WORD wRGBI, WORD Mask )
		{
			GetInfo();
			csbi.wAttributes &= Mask; 
			csbi.wAttributes |= wRGBI; 
			SetConsoleTextAttribute( hCon, csbi.wAttributes );
		}
	} console;

	//narrow manipulators
	inline std::ostream& clr( std::ostream& os )
	{
		os.flush();
		console.Clear();
		return os;
	};

	inline std::ostream& fg_red( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgHiRed, bgMask );

		return os;
	}

	inline std::ostream& fg_green( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgHiGreen, bgMask );

		return os;
	}

	inline std::ostream& fg_blue( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgHiBlue, bgMask );

		return os;
	}

	inline std::ostream& fg_white( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgHiWhite, bgMask );

		return os;
	}

	inline std::ostream& fg_cyan( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgHiCyan, bgMask );

		return os;
	}

	inline std::ostream& fg_magenta( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgHiMagenta, bgMask );

		return os;
	}

	inline std::ostream& fg_yellow( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgHiYellow, bgMask );

		return os;
	}

	inline std::ostream& fg_black( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgBlack, bgMask );

		return os;
	}

	inline std::ostream& fg_gray( std::ostream& os )
	{
		os.flush();
		console.SetColor( fgGray, bgMask );

		return os;
	}

	inline std::ostream& bg_red( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgHiRed, fgMask );

		return os;
	}

	inline std::ostream& bg_green( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgHiGreen, fgMask );

		return os;
	}

	inline std::ostream& bg_blue( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgHiBlue, fgMask );

		return os;
	}

	inline std::ostream& bg_white( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgHiWhite, fgMask );

		return os;
	}

	inline std::ostream& bg_cyan( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgHiCyan, fgMask );

		return os;
	}

	inline std::ostream& bg_magenta( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgHiMagenta, fgMask );

		return os;
	}

	inline std::ostream& bg_yellow( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgHiYellow, fgMask );

		return os;
	}

	inline std::ostream& bg_black( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgBlack, fgMask );

		return os;
	}

	inline std::ostream& bg_gray( std::ostream& os )
	{
		os.flush();
		console.SetColor( bgGray, fgMask );

		return os;
	}

	//wide manipulators
	inline std::wostream& clr( std::wostream& os )
	{
		os.flush();
		console.Clear();
		return os;
	};

	inline std::wostream& fg_red( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgHiRed, bgMask );

		return os;
	}

	inline std::wostream& fg_green( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgHiGreen, bgMask );

		return os;
	}

	inline std::wostream& fg_blue( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgHiBlue, bgMask );

		return os;
	}

	inline std::wostream& fg_white( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgHiWhite, bgMask );

		return os;
	}

	inline std::wostream& fg_cyan( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgHiCyan, bgMask );

		return os;
	}

	inline std::wostream& fg_magenta( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgHiMagenta, bgMask );

		return os;
	}

	inline std::wostream& fg_yellow( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgHiYellow, bgMask );

		return os;
	}

	inline std::wostream& fg_black( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgBlack, bgMask );

		return os;
	}

	inline std::wostream& fg_gray( std::wostream& os )
	{
		os.flush();
		console.SetColor( fgGray, bgMask );

		return os;
	}

	inline std::wostream& bg_red( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgHiRed, fgMask );

		return os;
	}

	inline std::wostream& bg_green( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgHiGreen, fgMask );

		return os;
	}

	inline std::wostream& bg_blue( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgHiBlue, fgMask );

		return os;
	}

	inline std::wostream& bg_white( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgHiWhite, fgMask );

		return os;
	}

	inline std::wostream& bg_cyan( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgHiCyan, fgMask );

		return os;
	}

	inline std::wostream& bg_magenta( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgHiMagenta, fgMask );

		return os;
	}

	inline std::wostream& bg_yellow( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgHiYellow, fgMask );

		return os;
	}

	inline std::wostream& bg_black( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgBlack, fgMask );

		return os;
	}

	inline std::wostream& bg_gray( std::wostream& os )
	{
		os.flush();
		console.SetColor( bgGray, fgMask );

		return os;
	}

	struct ConsoleColors 
	{
		unsigned short fg;
		unsigned short bg;

		static ConsoleColors get() 
		{
			ConsoleColors colors = {0xF, 0x0}; // black, white

			// Get the console screen buffer info function
			HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
			BOOL (WINAPI *doGetConsoleScreenBufferInfo)(HANDLE, CONSOLE_SCREEN_BUFFER_INFO*) = reinterpret_cast<BOOL (WINAPI*)(HANDLE,CONSOLE_SCREEN_BUFFER_INFO*)>(getConsoleFunction("GetConsoleScreenBufferInfo"));
			if (doGetConsoleScreenBufferInfo==NULL) { return colors; }

			// Get the console colors using that function
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			(*doGetConsoleScreenBufferInfo)(hCon,&csbi);
			WORD color = csbi.wAttributes;

			// Create a ConsoleColors object with those colors in
			colors.fg = color & 0xf;
			colors.bg = color >> 4;
			return colors;
		}

	private:
		static void* getConsoleFunction(char *name) {
			static HMODULE kernel32=(HMODULE)0xffffffff;
			if(kernel32==0)
				return NULL;
			if(kernel32==(HMODULE)0xffffffff) {
				kernel32=LoadLibrary(TEXT("kernel32.dll"));
				if(kernel32==0)
					return 0;
			}
			return GetProcAddress(kernel32,name);
		}
	};

} // namespace JadedHoboConsole

template <typename Stream>
Stream& operator<<( Stream& os, JadedHoboConsole::ConsoleColors const& colors )
{
	using namespace JadedHoboConsole;

	os.flush();
	console.SetColor( colors.fg, bgMask );
	console.SetColor( colors.bg, fgMask );

	return os;
}

namespace unittests 
{
	namespace color = JadedHoboConsole;
	
	std::map<std::string, bool(*)()> test_list;

	bool addUnitTest(std::string const& name, bool(*func)()) {
		if (func) { 
			test_list[name] = func;
			return true;
		}
		return false;
	}

	void run(bool verbose = true, bool colors = true) {

		// Get the previous foreground and background colors
		color::ConsoleColors defaultColors = color::ConsoleColors::get();

		// If there are no tests, don't do anything
		if (test_list.size() == 0) {
			std::cout << defaultColors << "No unit tests defined, canceling test run." << std::endl;
			return;
		}

		size_t testNum = 0;
		size_t passedTests = 0;
		size_t numTests = test_list.size();
		std::cout << "Running " << numTests << " unit tests...\n" << std::endl;

		// If there are tests to run, run them and print the results
		for (std::map<std::string, bool(*)()>::iterator it = test_list.begin();
			it != test_list.end(); ++it) 
		{
			bool passed = false;
			++testNum;
			try {
				if (verbose) { 
					std::cout << testNum << ". Running test " << it->first << "... "; 
					std::cout.flush();
				}
				passed = it->second(); // run the test function
				if (verbose) { 
					if (passed) {
						std::cout << color::fg_green << "passed";
					} else {
						std::cout << color::fg_red << "failed";
					}
					std::cout << defaultColors << std::endl;
				} else if (!passed) {
					std::cout << "Test " << it->first << color::fg_red << " failed" << defaultColors << std::endl;
				}
			} catch(std::exception e) {
				if (verbose) {
					std::cout << color::fg_red << "failed" << color::fg_yellow << "\n*** " << e.what() << defaultColors << std::endl;
				} else {
					std::cout << "Test " << it->first << color::fg_red << " failed" << color::fg_yellow << "\n*** " << e.what() << defaultColors << std::endl;
				}
			}
			passedTests += passed;
		}

		if (passedTests == 0) {
			std::cout << color::fg_red;
		} else if (passedTests < numTests) {
			std::cout << color::fg_yellow;
		} else {
			std::cout << color::fg_green;
		}
		std::cout << "\n" << passedTests << " of " << numTests << " tests passed.\n" << defaultColors << std::endl;

		exit(0);
	}

	template <typename T, typename U>
	void require_equal(std::string const& name1, std::string const& name2, T const& value1, U const& value2) {
		if (!(value1 == value2)) {
			std::stringstream message;
			message << "REQUIRE_EQUAL failed: " << name1 << " != " << name2 << " (values " << value1 << ", " << value2 << ")";
			throw std::exception(message.str().c_str());
		}
	}

	bool require(std::string const& expression, bool value) {
		if (!value) {
			std::stringstream message;
			message << "REQUIRE failed: " << expression;
			throw std::exception(message.str().c_str());
		}
	}
}

#   define UNIT_TEST_HELPER(name, testNum) bool _unittest_##name##testNum(); \
	static bool _unittest_##name##testNum##added = unittests::addUnitTest(#name, _unittest_##name##testNum); \
	bool _unittest_##name##testNum()
#   define UNIT_TEST_INTERMEDIATE_HELPER(name, testNum) UNIT_TEST_HELPER(name, testNum)
#   define unittest(testname) UNIT_TEST_INTERMEDIATE_HELPER(testname, __LINE__)
#   define REQUIRE_EQUAL(a, b) unittests::require_equal(#a, #b, a, b)
#   define REQUIRE_CLOSE(a, b) unittests::require_close(#a, #b, a, b)
#   define REQUIRE(a) unittests::require(#a, a)

#else // if RUN_UNIT_TESTS is undefined

namespace unittests {
	void run(bool verbose = true, bool colors = true){}
}

#   define UNIT_TEST_HELPER(name, testNum) template<typename T> void deadFunction##name##testNum()
#   define UNIT_TEST_INTERMEDIATE_HELPER(name, testNum) UNIT_TEST_HELPER(name, testNum)
#   define unittest(testname) UNIT_TEST_INTERMEDIATE_HELPER(testname, __LINE__)
#   define REQUIRE_EQUAL(a, b)
#   define REQUIRE_CLOSE(a, b)
#   define REQUIRE(a)

#endif

#endif // UNITTESTS_HEADER
