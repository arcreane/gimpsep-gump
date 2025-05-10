#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

void stringToLower(std::string& txt);

void stringToUpper(std::string& txt);

int stringInputValidator(std::string& inputtedString, int retries, std::string promptString, std::vector<std::string> validInputs);

int intInputValidator(int& inputtedInt, int retries, std::string promptString, std::pair<int, int> validRange);

#endif /* INPUT_HELPER_H */
