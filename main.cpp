#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdio.h>
#include "transformations.h"
#include "inputHelper.h"

#define stringify( name ) # name

/// @brief file type to be tranformed
enum File {
    IMAGE,
    IMAGES,
    DIRECTORY,
    VIDEO
};

/// @brief requested operation type
enum Operation {
    RESTORE,
    DILATE,
    ERODE,
    RESIZE,
    LIGHTEN,
    DARKEN,
    STITCH,
    CANNY,
    SAVE
};

const std::unordered_map<std::string, Operation> OP_TABLE = { 
    {"RESTORE", RESTORE},
    {"DILATE", DILATE},
    {"ERODE", ERODE},
    {"RESIZE", RESIZE},
    {"LIGHTEN", LIGHTEN},
    {"DARKEN", DARKEN},
    {"STITCH", STITCH},
    {"CANNY", CANNY},
    {"SAVE", SAVE}
};

/// @brief load file for transformation
/// @param source source to have the file loaded into
/// @return 0 for success, -1 for failure
int loadFile(cv::Mat& source)
{
    int retryCount = 3;
    
    printf("Enter the name of the file you'd like to load:\n");
    while(retryCount) {
        std::string fileName = "davec.JPG";
        source = cv::imread(fileName, cv::IMREAD_COLOR);

        if (!source.data) {
            retryCount--;
            if (retryCount) {
                printf("No image data found. Please enter a valid image file.\n");
            } else {
                printf("No image data found: Closing application.\n");
                return -1;
            }
        } else {
            return 0;
        }
    }

    return 0;
}

/// @brief removes all current unsaved edits and restores the original file
/// @param source 
/// @param edited 
void restore(cv::Mat& source, cv::Mat& edited)
{
    std::string userInput;

    printf("Restoring the image will wipe all current edits.\n");
    printf("There is no way to reverse this operation.\n");
    printf("Would you like to continue? [y/n]: \n");
    std::cin >> userInput;
    stringToLower(userInput);

    if (!userInput.compare("y"))
    {
        edited = source;
    }
}

/// @brief save the current edited file into or onto a file
/// @param edited 
void saveFile(cv::Mat& edited)
{
    std::string outputName;
    std::string promptString = "Please enter the name of the output file (extension included):\n";
    std::vector<std::string> validInputs = {};
    inputValidator(outputName, 3, promptString, validInputs);
    imwrite(outputName, edited);
}

int main(int argc, char* argv[])
{
    cv::Mat source, edited;
    // Videocapture cap;
    Operation op = RESTORE;
    File f;
    
    bool active = true;
    std::string opInput;
    std::string promptString = "Please enter your desired operation:\n \
        [Restore, Save, Dilate, Erode, Resize, Lighten, Darken, Stitch, Canny (for Canny Edge Detection)]\n";
    std::vector<std::string> validInputs = {"RESTORE, DILATE, ERODE, RESIZE, LIGHTEN, DARKEN, STITCH, CANNY"};

    if (loadFile(source) == -1) {
        return -1;
    } else {
        edited = source;    
    }
    
    while (active) {
        inputValidator(opInput, 3, promptString, validInputs);
        stringToUpper(opInput);
        
        switch(OP_TABLE.at(opInput)) {
            case DILATE:
                dilate(edited);
                break;
            case ERODE:
                erode(edited);
                break;
            case RESIZE:
                resize(edited);
                break;
            case LIGHTEN:
                lighten(edited);
                break;
            case DARKEN:
                darken(edited);
                break;
            case STITCH:
                stitch(edited);
                break;
            case CANNY:
                cannyEdgeDetection(edited);
                break;
            case SAVE:
                saveFile(edited);
                active = false;
                break;
            default:
                printf("-- Invalid operation request. --\n");
                break;
        }
    }

    // cap.close();
    cv::destroyAllWindows();
    return 0;
}
