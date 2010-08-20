// #define RUN_UNIT_TESTS to enable unit testing
#include "unittests.h"
#include <exception>
#include <iostream>
#include <vector>
using std::vector;
using std::exception;
using std::cout;
using std::endl;

int bork() {
	return 3;
}

testsuite BorkSuite
{
	int integerHarness = 100;

	unittest(bork) {
		REQUIRE_EQUAL(integerHarness, 100);
		int result = bork();
		return result == 3;
	}

	unittest(fail) {
		return false;
	}
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

class Berk {
	int privot, data;
public:
	Berk():privot(20),data(30){}
	unittesthook;
};

classunittest(Berk, whablamo) {
	REQUIRE_EQUAL(privot, data-10);
	return true;
}

classunittest(Berk, whablamoz) {
	REQUIRE_EQUAL(privot+80, 100);
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
