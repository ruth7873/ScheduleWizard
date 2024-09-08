#include "Utility.h"

int Utility::integerValidation(string inputMassege, string variableName, bool negative)
{
	int numberInput;
	std::string input;
	while (true) {
		std::cout << inputMassege <<std::endl;
		std::cin >> input;

		// Check if the input contains only digits
		if (input.find_first_not_of("0123456789-") != std::string::npos) {
			std::cerr << "Invalid input. "<<variableName<<" should contain only numeric digits." << std::endl;
			continue; // Restart the loop for a valid input
		}

		// Convert the input to an integer
		numberInput = std::stoi(input);

		if (!negative && numberInput < 0) {
			std::cerr << "Invalid input: "<< variableName << " cannot be negative." << std::endl;
			continue; // Restart the loop for a valid input
		}

		break; // If numberInput is valid, exit the loop
	}
    return numberInput;
}
