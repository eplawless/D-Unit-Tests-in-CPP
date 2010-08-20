// #define RUN_UNIT_TESTS to enable unit tests

#include "unittests.h"
#include <exception>
#include <iostream>
using std::exception;
using std::cout;
using std::endl;

int bork() {
	return 3;
}

unittest(bork) {
	int result = bork();
	return result == 3;
}

unittest(fail) {
	return false;
}

unittest(exception) {
	int result = bork();
	throw exception("FUCKING, WHAT");
	return result == 3;
}

unittest(require_equal_fail) {
	int what = 10;
	REQUIRE_EQUAL(what, 20);
	return true;
}

unittest(require_equal_pass) {
	int what = 10;
	int why = 20;
	REQUIRE_EQUAL(what, why-10);
	return true;
}

unittest(require_fail) {
	REQUIRE((true || false) && false);
	return true;
}

unittest(require_pass) {
	REQUIRE((true || false) || false);
	return true;
}

void main(int argc, char* argv[])
{
	unittests::run();

	cout << "hooray!" << endl;
}