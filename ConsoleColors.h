#pragma once
#ifndef UNIT_TEST_CONSOLE_COLORS_H_
#define UNIT_TEST_CONSOLE_COLORS_H_

#ifdef _WIN32
#include "ConsoleColorsNT.h"
#else
#error "Non-Windows Platforms Not Currently Supported"
#endif

#endif // UNIT_TEST_CONSOLE_COLORS_H_

