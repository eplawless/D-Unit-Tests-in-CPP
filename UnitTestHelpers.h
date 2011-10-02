#pragma once
#ifndef UNIT_TEST_HELPERS_H_
#define UNIT_TEST_HELPERS_H_

#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <typeinfo>
#include <iostream>

#include "NS.h"

namespace UNIT_TEST_NS {

/// @{
/// Conversion to raw bytes. This allows types which have no comparison
/// operators or stream output operators to be compared and output. It's
/// kind of a terrible hack, but that's okay.
struct RawBytes 
{
    template <typename T>
    RawBytes(const T& value) 
	: size(sizeof(T)) 
	, name(typeid(T).name())
    {
	data = reinterpret_cast<const char*>(&value);
    }

    const char *name;
    const int size;
    const char *data;
};

bool operator==(const RawBytes& lhs, const RawBytes& rhs) {
    return lhs.size == rhs.size
	&& strncmp(lhs.data, rhs.data, lhs.size) == 0;
}

std::ostream& operator<<(std::ostream& ostream, const RawBytes& bytes) {
    ostream << bytes.name << " bytes [" << std::hex;
    ostream.fill('0');
    for (int idx = 0; idx < bytes.size; ++idx) {
	if (idx != 0)
	    ostream << " ";
	ostream << std::setw(2) << static_cast<size_t>(bytes.data[idx]);
    }
    ostream << "]" << std::dec;
    return ostream;
}
/// @}

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
	std::streamsize chars_read = sbuf->sgetn(buffer.data, buffer_size-1);
    buffer.data[chars_read] = '\0';
    for (std::streamsize idx = 0; idx < chars_read; ++idx)
	sbuf->sungetc();
    return static_cast<size_t>(chars_read);
}

static size_t require_stream_prefix(
    const std::string& name,
    const std::ostream& stream,
    const std::string& value)
{
    const size_t value_size = value.length();
    ScopedArray<char> buffer(value_size+1);
    size_t chars_read = get_buffer(stream, buffer, value_size+1);
    if (value == buffer.data)
	return chars_read;

    std::stringstream err;
    err << name << " failed: " << buffer.data << " != " << value;
    throw std::exception(err.str().c_str());
}

static void require_stream_equal(
    const std::string& name,
    const std::ostream& stream,
    const std::string& value)
{
    size_t chars_read = require_stream_prefix(name, stream, value);
    size_t chars_in_stream = static_cast<size_t>(stream.rdbuf()->in_avail());
    if (chars_read == chars_in_stream)
	return;

    ScopedArray<char> buffer(chars_in_stream+1);
    get_buffer(stream, buffer, chars_in_stream+1);

    std::stringstream err;
    err << name << " failed; matched \"" << value 
	<< "\", but there was extra input: " << buffer.data;
    throw std::exception(err.str().c_str());
}


template <typename LHSType, typename RHSType>
static void require_equal(
    const std::string& lhs_name,
    const std::string& rhs_name, 
    const LHSType& lhs_value, 
    const RHSType& rhs_value) 
{
    if (lhs_value == rhs_value)
	return;

    std::stringstream stream;
    stream << "REQUIRE_EQUAL failed: " << lhs_name << " != " << rhs_name 
	<< " (values " << lhs_value << ", " << rhs_value << ")";

    throw std::exception(stream.str().c_str());
}

static void require(const std::string& expression, bool value) 
{
    if (value)
	return;

    std::stringstream stream;
    stream << "REQUIRE failed: " << expression;

    throw std::exception(stream.str().c_str());
}

} // namespace

#endif // UNIT_TEST_HELPERS_H_

