#pragma once
#ifndef UNIT_TEST_HELPERS_H_
#define UNIT_TEST_HELPERS_H_

#include <string>
#include <sstream>
#include <vector>
#include <exception>

#include "NS.h"

namespace UNIT_TEST_NS {

typedef std::string String;
typedef std::exception Exception;
typedef std::stringstream SStream;

// TODO: this is terrible
template <typename T>
struct ScopedArray {
    ScopedArray(size_t size = 0) : data(NULL) {
	resize(size);
    }
    void resize(size_t size) {
	if (data) {
	    delete[] data;
	    data = NULL;
	}
	if (size > 0)
	    data = new T[size];
    }
    ~ScopedArray() {
	try {
	    resize(0);
	} catch (...) {}
    }

    T* data;
};

static size_t get_buffer(
    const std::ostream& stream,
    ScopedArray<char>& buffer,
    size_t buffer_size)
{
    buffer.resize(buffer_size);
    std::streambuf* sbuf = stream.rdbuf();
    const size_t chars_read = sbuf->sgetn(buffer.data, buffer_size-1);
    buffer.data[chars_read] = '\0';
    for (size_t idx = 0; idx < chars_read; ++idx)
	sbuf->sungetc();
    return chars_read;
}

static size_t require_stream_prefix(
    const String& name,
    const std::ostream& stream,
    const String& value)
{
    const size_t value_size = value.length();
    ScopedArray<char> buffer(value_size+1);
    size_t chars_read = get_buffer(stream, buffer, value_size+1);
    if (value == buffer.data)
	return chars_read;

    SStream err;
    err << name << " failed: " << buffer.data << " != " << value;
    throw Exception(err.str().c_str());
}

static void require_stream_equal(
    const String& name,
    const std::ostream& stream,
    const String& value)
{
    size_t chars_read = require_stream_prefix(name, stream, value);
    size_t chars_in_stream = stream.rdbuf()->in_avail();
    if (chars_read == chars_in_stream)
	return;

    ScopedArray<char> buffer(chars_in_stream+1);
    get_buffer(stream, buffer, chars_in_stream+1);

    SStream err;
    err << name << " failed; matched \"" << value 
	<< "\", but there was extra input: " << buffer.data;
    throw Exception(err.str().c_str());
}


template <typename LHSType, typename RHSType>
static void require_equal(
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

static void require(const String& expression, bool value) 
{
    if (value)
	return;

    SStream stream;
    stream << "REQUIRE failed: " << expression;

    throw Exception(stream.str().c_str());
}

} // namespace

#endif // UNIT_TEST_HELPERS_H_

