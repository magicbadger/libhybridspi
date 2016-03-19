#ifndef HYBRIDSPI_TEST_UTIL_H_
#define HYBRIDSPI_TEST_UTIL_H_

#include <iostream>

#define ASSERT(test, a, b) if(a != b) { cout << "Failed test: " << test << endl; cout << a << "\ndoes not match\n" << b << endl;; return 1; };

#endif
