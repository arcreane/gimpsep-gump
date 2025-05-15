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

/// @brief mapping of string to operation type
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
        std::string fileName;
        std::cin >> fileName;
        clearInput();
        // source = cv::imread(fileName, cv::IMREAD_COLOR);
        source = cv::imread(cv::samples::findFile(fileName), cv::IMREAD_COLOR);
        source.convertTo(source, CV_8U);

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

int main(int argc, char* argv[])
{
    cv::Mat source, current, edited;
    // Videocapture cap;
    Operation op = RESTORE;
    File f;
    
    bool active = true;
    std::string opInput;
    std::string promptString = "Please enter your desired operation:\n \
        [Restore, Save, Dilate, Erode, Resize, Lighten, Darken, Stitch, Canny (for Canny Edge Detection)]\n";
    std::vector<std::string> validInputs = { 
        "RESTORE", 
        "SAVE",
        "DILATE", 
        "ERODE", 
        "RESIZE", 
        "LIGHTEN", 
        "DARKEN", 
        "STITCH", 
        "CANNY"
    };

    if (loadFile(source) == -1) {
        return -1;
    } else {
        current = source;  
    }
    
    while (active) {
        if (stringInputValidator(opInput, 3, promptString, validInputs)) {
            return -1;
        }
        stringToUpper(opInput);
        
        switch(OP_TABLE.at(opInput)) {
            case DILATE:
                dilate(current, edited);
                break;
            case ERODE:
                erode(current, edited);
                break;
            case RESIZE:
                resize(current, edited);
                break;
            case LIGHTEN:
                lighten(current, edited);
                break;
            case DARKEN:
                darken(current, edited);
                break;
            case STITCH:
                stitch(current, edited);
                break;
            case CANNY:
                cannyEdgeDetection(current, edited);
                break;
            case RESTORE:
                restore(source, edited);
                break;
            case SAVE:
                active = false;
                saveFile(edited, active);
                break;
            default:
                printf("-- Invalid operation request. --\n");
                break;
        }
        current = edited;
    }

    // cap.close();
    cv::destroyAllWindows();
    return 0;
}
