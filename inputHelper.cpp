#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "inputHelper.h"

/// @brief In place tranformation of a string to lowercase
/// @param txt string to be transformed
void stringToLower(std::string& txt)
{
    std::transform(txt.begin(), txt.end(), txt.begin(), [](unsigned char c){ return std::tolower(c); });
}

/// @brief In place tranformation of a string to uppercase
/// @param txt string to be transformed
void stringToUpper(std::string& txt)
{
    std::transform(txt.begin(), txt.end(), txt.begin(), [](unsigned char c){ return std::toupper(c); });
}

/// @brief Given a set amount of retries, prompts the user for a set of valid inputs
/// @param inputtedString string to contain a valid user-inputted string
/// @param retries number of retries allowed for the user before failure
/// @param promptString the prompt sent to the user
/// @param validInputs vector of all possible valid strings
/// @return 0 for success, -1 for failure
int inputValidator(std::string& inputtedString, int retries, std::string promptString, std::vector<std::string> validInputs)
{
    int attempts = 0;
    while(attempts < retries) {
        std::cout << promptString << std::endl;
        std::cin >> inputtedString;

        if (!validInputs.empty() && std::find(validInputs.begin(), validInputs.end(), inputtedString) != validInputs.end()) {
            return 0;
        }

        attempts++;
        printf("-- Invalid input --\n");
    }

    printf("Retry count exceeded: cancelling operation.\n");
    return -1;
}
