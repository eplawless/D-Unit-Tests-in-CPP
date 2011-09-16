// #define RUN_UNIT_TESTS to enable unit testing
#include "UnitTest.h"
#include <iostream>

#pragma warning(push, 4)

struct Base 
{
    Base() { std::cout << "1 "; }
    ~Base() { std::cout << "3 "; }
    int foo() { std::cout << "2 "; return 4; }
};

unittest {
    {
       	Base b;
	REQUIRE_EQUAL(b.foo(), 4);
    }
    REQUIRE_COUT_PREFIX("1 2 3");
}

namedtest("@#>^#SYMBOLLLLS") {
    REQUIRE(sizeof(int) == 4);
}

UNIT_TEST_HOOKS;
void main()
{
    RUN_UNIT_TESTS;
    std::cout << "NORMAL EXECUTION" << std::endl;
}

#pragma warning(pop)

