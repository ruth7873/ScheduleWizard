#include "doctest.h"
#include <iostream>
#include <sstream>
#include "../Main/Utility.h"

TEST_CASE("Utility::integerValidation") {
    // Create a stringstream for capturing output from std::cerr
    std::ostringstream cerrStream;
    std::streambuf* oldCerrStream = std::cerr.rdbuf(cerrStream.rdbuf());

    // Test valid input
    SUBCASE("Valid input") {
        std::istringstream cinStream("123\n");
        std::streambuf* oldCinStream = std::cin.rdbuf(cinStream.rdbuf());

        int result = Utility::integerValidation("Enter number:", "TestVariable", false);
        CHECK_EQ(result, 123);
        CHECK(cerrStream.str().empty());

        std::cin.rdbuf(oldCinStream); // Restore std::cin
    }

    // Test invalid non-numeric input
    SUBCASE("Invalid non-numeric input") {
        std::istringstream cinStream("abc\n123\n");
        std::streambuf* oldCinStream = std::cin.rdbuf(cinStream.rdbuf());

        int result = Utility::integerValidation("Enter number:", "TestVariable", false);
        CHECK_EQ(result, 123);
        CHECK(cerrStream.str() == "Invalid input. TestVariable should contain only numeric digits.\n");

        std::cin.rdbuf(oldCinStream); // Restore std::cin
    }

    // Test invalid negative input with a valid input after invalid ones
    SUBCASE("Invalid negative input") {
        std::istringstream cinStream("-5\n5\n");
        std::streambuf* oldCinStream = std::cin.rdbuf(cinStream.rdbuf());

        int result = Utility::integerValidation("Enter number:", "TestVariable", false);
        CHECK_EQ(result, 5);
        CHECK(cerrStream.str() == "Invalid input: TestVariable cannot be negative.\n");

        std::cin.rdbuf(oldCinStream); // Restore std::cin
    }

   //  Test valid negative input
    SUBCASE("Valid negative input") {
        std::istringstream cinStream("-5\n");
        std::streambuf* oldCinStream = std::cin.rdbuf(cinStream.rdbuf());

        int result = Utility::integerValidation("Enter number:", "TestVariable", true);
        CHECK_EQ(result, -5);
        CHECK(cerrStream.str().empty());

        std::cin.rdbuf(oldCinStream); // Restore std::cin
    }

    // Restore std::cerr
    std::cerr.rdbuf(oldCerrStream);
}
