// Copyright (C) 2001 Claus Dræby
// Terms of use are in the file COPYING
#include <iostream>
#include "tester.h"
#include "optmap.h"

/**
 * The main of a test program that executes the main test suite and then
 * reports the summary.
 *
 * A #-v# or #--verbose# will turn on verbose, that reports succesful test
 * cases; the default behaviour is to report only those that fails.
 */
int main(int argc, const char* argv[]);

/// @name{unitpp}
namespace unitpp {
/// split the string by the char
vector<string> vectorize(const string&, char);
/**
 * @name{run_test-id}
 * Run a test found in the suite::main() test by id. If id is empty run the
 * main test.
 * @return true, if the test was totally succesful.
 */
bool run_test(const string& id = "");
/// Run the test and return true if succesful. @see{run_test-id}
bool run_test(test*);
/// find the test with the given id
test* find_test(const string& id);
}
