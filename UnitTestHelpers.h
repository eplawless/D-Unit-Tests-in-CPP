#pragma once
#ifndef UNIT_TEST_HELPERS_H_
#define UNIT_TEST_HELPERS_H_

#include <string>
#include <sstream>
#include <exception>

#include "NS.h"

namespace UNIT_TEST_NS {

typedef std::string String;
typedef std::exception Exception;
typedef std::stringstream SStream;

template <typename LHSType, typename RHSType>
void require_equal(
    const String& lhs_name,
    const String& rhs_name, 
    const LHSType& lhs_value, 
    const RHSType& rhs_value) 
{
    if (lhs_value == rhs_value)
	return;

    SStream stream;
    stream << "REQUIRE_EQUAL failed: " << lhs_name << " != " << rhs_name 
	<< " (values " << lhs_value << ", " << rhs_value << ")";

    throw Exception(stream.str().c_str());
}

void require(const String& expression, bool value) 
{
    if (value)
	return;

    SStream stream;
    stream << "REQUIRE failed: " << expression;

    throw Exception(stream.str().c_str());
}

} // namespace

#endif // UNIT_TEST_HELPERS_H_

