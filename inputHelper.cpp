#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "inputHelper.h"

/// @brief parses string input into an int
/// @param inputtedString string inputted by user
/// @param inputtedInt int for the string to be parsed into
/// @return returns true on success, false on failure
bool parseInt(std::string& inputtedString, int& inputtedInt)
{
    try{
        inputtedInt = std::stoi(inputtedString);
    } catch (std::invalid_argument) {
        return false;
    }
    return true;
}

/// @brief parses string input into a double
/// @param inputtedString string inputted by user
/// @param inputtedDouble double for the string to be parsed into
/// @return returns true on success, false on failure
bool parseDouble(std::string& inputtedString, double& inputtedDouble)
{
    try{
        inputtedDouble = std::stod(inputtedString);
    } catch (std::invalid_argument) {
        return false;
    }
    return true;
}

/// @brief clears std::cin input buffer and error status
void clearInput()
{
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
}

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
/// @param validInputs vector of all possible valid strings in all caps. If empty, all inputs are valid
/// @return 0 for success, -1 for failure
int stringInputValidator(std::string& inputtedString, int retries, std::string promptString, std::vector<std::string> validInputs)
{
    int attempts = 0;
    while(attempts < retries) {
        std::cout << promptString << std::endl;
        std::cin >> inputtedString;
        std::string capString = inputtedString;
        stringToUpper(capString);

        if (validInputs.empty() || std::find(validInputs.begin(), validInputs.end(), capString) != validInputs.end()) {
            return 0;
        }

        clearInput();
        attempts++;
        printf("-- Invalid input --\n");
    }

    printf("Retry count exceeded: cancelling operation.\n");
    return -1;
}

/// @brief Given a set amount of retries, prompts the user for a set of valid inputs
/// @param inputtedInt int to contain a valid user-inputted int
/// @param retries number of retries allowed for the user before failure
/// @param promptString the prompt sent to the user
/// @param validRange pair defining lower and upper value limits. If pair values are equal, all inputs are valid
/// @return 0 for success, -1 for failure
int intInputValidator(int& inputtedInt, int retries, std::string promptString, std::pair<int, int> validRange)
{
    int attempts = 0;
    std::string inputtedString;
    while(attempts < retries) {
        std::cout << promptString << std::endl;
        std::cin >> inputtedString;

        if (parseInt(inputtedString, inputtedInt) && (validRange.first == validRange.second || \
            validRange.first <= inputtedInt  && inputtedInt <= validRange.second)) {
            return 0;
        }

        clearInput();
        attempts++;
        printf("-- Invalid input --\n");
    }

    printf("Retry count exceeded: cancelling operation.\n");
    return -1; 
}

/// @brief Given a set amount of retries, prompts the user for a set of valid inputs
/// @param inputtedDouble double to contain a valid user-inputted double
/// @param retries number of retries allowed for the user before failure
/// @param promptString the prompt sent to the user
/// @param validRange pair defining lower and upper value limits. If pair values are equal, all inputs are valid
/// @return 0 for success, -1 for failure
int doubleInputValidator(double& inputtedDouble, int retries, std::string promptString, std::pair<double, double> validRange)
{
    int attempts = 0;
    std::string inputtedString;
    while(attempts < retries) {
        std::cout << promptString << std::endl;
        std::cin >> inputtedString;

        if (parseDouble(inputtedString, inputtedDouble) && (validRange.first == validRange.second || \
            validRange.first <= inputtedDouble  && inputtedDouble <= validRange.second)) {
            return 0;
        }

        clearInput();
        attempts++;
        printf("-- Invalid input --\n");
    }

    printf("Retry count exceeded: cancelling operation.\n");
    return -1; 
}

/// @brief Given a set amount of retries, prompts the user for a set of valid inputs
/// @param inputtedBool bool to contain a a valid yes or no answer entered by the user
/// @param retries number of retries allowed for the user before failure
/// @param promptString the prompt sent to the user
/// @return 0 for success, -1 for failure
int ynInputValidator(bool& inputtedBool, int retries, std::string promptString)
{
    int attempts = 0;
    std::string inputtedString;
    std::vector<std::string> validTrue = {"Y", "YES"};
    std::vector<std::string> validFalse = {"N", "NO"};

    /// stringInputValidator(inputtedString, 3, promptString, validTrue);
    while(attempts < retries) {
        std::cout << promptString << std::endl;
        std::cin >> inputtedString;
        stringToUpper(inputtedString);

        if (std::find(validTrue.begin(), validTrue.end(), inputtedString) != validTrue.end()) {
            inputtedBool = true;
            return 0;
        } else if (std::find(validFalse.begin(), validFalse.end(), inputtedString) != validFalse.end()) {
            inputtedBool = false;
            return 0;
        }

        clearInput();
        attempts++;
        printf("-- Invalid input --\n");
    }

    printf("Retry count exceeded: defaulting to no.\n");
    inputtedBool = false;
    return -1;
}

/// @brief 
/// @param imgs 
/// @param current 
/// @return 
int parseStitchArgs(std::vector<cv::Mat>& imgs, cv::Mat& current)
{    
    bool useCurrent;
    std::string promptString = "Would you like to include the currently selected image [y/n]: ";
    if (ynInputValidator(useCurrent, 3, promptString)) { return -1; }
    if (useCurrent) { imgs.push_back(current); }

    std::string inputtedString;
    std::string imgPromptString = "Please enter all files you wish to stitch (seperated by spaces)";
    std::vector<std::string> validInputs = {};
    stringInputValidator(inputtedString, 1, imgPromptString, validInputs);

    std::vector<std::string> fileNames;
    std::istringstream stream(inputtedString);
    std::string token;
    while (getline(stream, token, ' ')) {
        fileNames.push_back(token);
    }
    clearInput();

    for (std::string fName : fileNames) {
        cv::Mat img = cv::imread(cv::samples::findFile(fName));
        if (img.empty())
        {
            std::cout << "Can't read image: " << fName << std::endl;
            return -1;
        }

        imgs.push_back(img);
    }

    return imgs.size() > 0 ? 0 : -1;
}
