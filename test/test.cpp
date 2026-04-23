#define CATCH_CONFIG_MAIN
#include "catch/catch_amalgamated.hpp"
#include "../src/CampusCompass.h"
#include <sstream>
#include <iostream>
using namespace std;
/*
string runCommands(string input) {
    CampusCompass c;
    c.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream in(input);
    stringstream out;

    streambuf* cinbuf = std::cin.rdbuf();
    streambuf* coutbuf = std::cout.rdbuf();

    std::cin.rdbuf(in.rdbuf());
    std::cout.rdbuf(out.rdbuf());

    int n;
    std::cin >> n;
    std::cin.ignore();

    string command;
    for (int i = 0; i < n; i++) {
        getline(std::cin, command);
        c.ParseCommand(command);
    }
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);

    return out.str();
}

TEST_CASE("1:Invalid Commands Test", "[invalid]") {
    string input = R"(5
insert "A11y" 45679999 1 1 COP3530
insert "Valid Name" 1234 1 1 COP3530
insert "Valid Name" 12345678 1 0
insert "Valid Name" 12345678 1 2 COP3530
remove 99999999
)";

    string output = runCommands(input);

    REQUIRE(output ==
        "unsuccessful\n"
        "unsuccessful\n"
        "unsuccessful\n"
        "unsuccessful\n"
        "unsuccessful\n"
    );
}
// testing edge cases like removing non-existent students and empty class issues
TEST_CASE("2:Edge Case Test", "[edge]") {
    string input = R"(4
remove 11111111
insert "Edge Case" 22222222 1 1 COP3530
dropClass 22222222 COP3530
remove 22222222
)";

    string output = runCommands(input);

    REQUIRE(output ==
        "unsuccessful\n"
        "successful\n"
        "successful\n"
        "unsuccessful\n"
    );
}
TEST_CASE("3:Core Functionality Test", "[core]") {
    string input = R"(6
insert "Test User" 33333333 1 2 COP3530 MAC2311
replaceClass 33333333 COP3530 CDA3101
dropClass 33333333 MAC2311
removeClass CDA3101
remove 33333333
remove 33333333
)";

    string output = runCommands(input);

    REQUIRE(output ==
        "successful\n"
        "successful\n"
        "successful\n"
        "1\n"
        "unsuccessful\n"
        "unsuccessful\n"
    );
}

// testing shortest path before and after closing an edge
TEST_CASE("4:Shortest Path Toggle Test", "[dijkstra]") {
    string input = R"(4
insert "Traveler" 44444444 1 1 COP3530
printShortestEdges 44444444
toggleEdgesClosure 1 1 2
printShortestEdges 44444444
)";

    string output = runCommands(input);

    REQUIRE(output.find("Time For Shortest Edges: Traveler") != string::npos);
}

TEST_CASE("Example Test Name - Change me!", "[tag]") {
  // instantiate any class members that you need to test here
  int one = 1;

  // anything that evaluates to false in a REQUIRE block will result in a
  // failing test
  REQUIRE(one == 1); // fix me!

  // all REQUIRE blocks must evaluate to true for the whole test to pass
  REQUIRE(true); // also fix me!
}

TEST_CASE("Test 2", "[tag]") {
  // you can also use "sections" to share setup code between tests, for example:
  int one = 1;

  SECTION("num is 2") {
    int num = one + 1;
    REQUIRE(num == 2);
  };

  SECTION("num is 3") {
    int num = one + 2;
    REQUIRE(num == 3);
  };

  // each section runs the setup code independently to ensure that they don't
  // affect each other
}

// Refer to Canvas for a list of required tests. 
// We encourage you to write more than required to ensure proper functionality, but only the ones on Canvas will be graded.

// See the following for an example of how to easily test your output.
// Note that while this works, I recommend also creating plenty of unit tests for particular functions within your code.
// This pattern should only be used for final, end-to-end testing.

// This uses C++ "raw strings" and assumes your CampusCompass outputs a string with
//   the same thing you print.
/*
TEST_CASE("Example CampusCompass Output Test", "[flag]") {
  // the following is a "raw string" - you can write the exact input (without
  //   any indentation!) and it should work as expected
  // this is based on the input and output of the first public test case
  string input = R"(6
insert "Student A" 10000001 1 1 COP3502
insert "Student B" 10000002 1 1 COP3502
insert "Student C" 10000003 1 2 COP3502 MAC2311
dropClass 10000001 COP3502
remove 10000001
removeClass COP3502
)";

  string expectedOutput = R"(successful
successful
successful
successful
unsuccessful
2
)";

  string actualOutput = runCommands(input);

  REQUIRE(actualOutput == expectedOutput);
}
*/
