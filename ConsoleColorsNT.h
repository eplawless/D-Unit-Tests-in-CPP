#pragma once
#ifndef UNIT_TEST_CONSOLE_COLORS_NT_H_
#define UNIT_TEST_CONSOLE_COLORS_NT_H_

#include <windows.h>

#include "NS.h"

/// Console color code for Windows stolen mostly from this article:
/// http://www.codeproject.com/KB/cpp/AddColorConsole.aspx

namespace UNIT_TEST_NS {

/// @{
/// Windows Console Color Values
#define DEFINE_COLOR_VALUES(p, long_p) \
    static const WORD p##_intensity  = long_p##_INTENSITY; \
    static const WORD p##_lo_red     = long_p##_RED; \
    static const WORD p##_lo_green   = long_p##_GREEN; \
    static const WORD p##_lo_blue    = long_p##_BLUE; \
    static const WORD p##_lo_cyan    = p##_lo_green   | p##_lo_blue; \
    static const WORD p##_lo_magenta = p##_lo_red     | p##_lo_blue; \
    static const WORD p##_lo_yellow  = p##_lo_red     | p##_lo_green; \
    static const WORD p##_lo_white   = p##_lo_red | p##_lo_green | \
				       p##_lo_blue; \
    static const WORD p##_hi_black   = 0; \
    static const WORD p##_hi_gray    = p##_hi_black   | p##_intensity; \
    static const WORD p##_hi_white   = p##_lo_white   | p##_intensity; \
    static const WORD p##_hi_blue    = p##_lo_blue    | p##_intensity; \
    static const WORD p##_hi_green   = p##_lo_green   | p##_intensity; \
    static const WORD p##_hi_red     = p##_lo_red     | p##_intensity; \
    static const WORD p##_hi_cyan    = p##_lo_cyan    | p##_intensity; \
    static const WORD p##_hi_magenta = p##_lo_magenta | p##_intensity; \
    static const WORD p##_hi_yellow  = p##_lo_yellow  | p##_intensity; \
    static const WORD p##_mask       = p##_lo_blue | p##_lo_green | \
				       p##_lo_red | p##_intensity;

DEFINE_COLOR_VALUES(fg, FOREGROUND);
DEFINE_COLOR_VALUES(bg, BACKGROUND);

#undef DEFINE_COLOR_VALUES
/// @}

/// Windows Console Abstraction
class WindowsConsole
{
public: // methods
    WindowsConsole() 
	: m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
	, m_numCharsWritten(0)
    {
	updateInfo();
    }

    WORD getColorAttrib() 
    { 
	return m_bufferInfo.wAttributes; 
    }

    bool setColor(WORD rgbi, WORD mask)
    {
	if (!updateInfo())
	    return false;

	m_bufferInfo.wAttributes &= mask;
	m_bufferInfo.wAttributes |= rgbi;
	return SetConsoleTextAttribute(m_hConsole, m_bufferInfo.wAttributes);
    }

    bool clear() 
    {
	static const COORD topLeft = {0,0};
	static const char space = ' ';

	return updateInfo()
	    && FillConsoleOutputCharacter(
	           m_hConsole, space, m_consoleSize, topLeft, 
		   &m_numCharsWritten)
	    && updateInfo()
	    && FillConsoleOutputAttribute(
		   m_hConsole, m_bufferInfo.wAttributes, m_consoleSize, 
		   topLeft, &m_numCharsWritten);
    }

    bool updateInfo() 
    {
	if (m_hConsole == INVALID_HANDLE_VALUE)
	    return false;

	bool got_info = GetConsoleScreenBufferInfo(m_hConsole, &m_bufferInfo);
	if (!got_info)
	    return false;

	m_consoleSize = m_bufferInfo.dwSize.X * m_bufferInfo.dwSize.Y;
	return true;
    }

private: // members
    HANDLE                      m_hConsole;
    DWORD                       m_numCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO  m_bufferInfo;
    DWORD                       m_consoleSize;
};

/// Global Console Manipulation
WindowsConsole& getConsole();

/// Current Console Colors
class ConsoleColors
{
public: // methods
    ConsoleColors() 
	: fg(0xF) // white
	, bg(0x0) // black
    {
	getConsole().updateInfo();
	WORD color = getConsole().getColorAttrib();
	fg = color & 0xF;
	bg = color >> 4;
    }

public: // members
    unsigned short  fg;
    unsigned short  bg;
};

/// @{
/// Stream Manipulators
template <typename OStream>
inline OStream& clear(OStream& ostream)
{
    ostream.flush();
    getConsole().clear();
    return ostream;
}

#define DEFINE_STREAM_MANIP_FUNC(prefix, mask, color) \
    inline std::ostream& prefix##_##color(std::ostream& ostream) \
    { \
        ostream.flush(); \
        getConsole().setColor(prefix##_hi_##color, mask); \
        return ostream; \
    }

#define DEFINE_ALL_COLOR_STREAM_MANIP_FUNCS(prefix, mask) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, red) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, green) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, blue) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, cyan) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, magenta) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, yellow) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, white) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, black) \
    DEFINE_STREAM_MANIP_FUNC(prefix, mask, gray)

DEFINE_ALL_COLOR_STREAM_MANIP_FUNCS(fg, bg_mask)
DEFINE_ALL_COLOR_STREAM_MANIP_FUNCS(bg, fg_mask)

#undef DEFINE_ALL_COLOR_STREAM_MANIP_FUNCS
#undef DEFINE_COLOR_MANIP

inline std::ostream& operator<<(std::ostream& ostream, const ConsoleColors& colors)
{
    ostream.flush();
    getConsole().setColor(colors.fg, bg_mask);
    getConsole().setColor(colors.bg, fg_mask);
    return ostream;
}
/// @}

} // namespace

#endif // UNIT_TEST_CONSOLE_COLORS_NT_H_

