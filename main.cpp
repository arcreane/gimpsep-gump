#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdio.h>
#include "transformations.h"
#include "inputHelper.h"
#include "record.h"

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
    UNDO,
    REDO,
    DILATE,
    ERODE,
    RESIZE,
    FLIP,
    LIGHTEN,
    DARKEN,
    STITCH,
    CANNY,
    SAVE
};

/// @brief mapping of string to operation type
const std::unordered_map<std::string, Operation> OP_TABLE = { 
    {"RESTORE", RESTORE},
    {"UNDO", UNDO},
    {"REDO", REDO},
    {"DILATE", DILATE},
    {"ERODE", ERODE},
    {"RESIZE", RESIZE},
    {"FLIP", FLIP},
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
    Record record(30);
    cv::Mat source, current, edited;
    // Videocapture cap;
    Operation op = RESTORE;
    File f;
    
    bool active = true;
    bool updateRecord = true;
    std::string currentOp = "ORIGINAL";
    std::string opInput;
    std::string promptString = "Please enter your desired editing operation:\n\
        [Dilate, Erode, Resize, Flip, Lighten, Darken, Stitch, Canny (for Canny Edge Detection)]\n\
        or for a file operation, enter one of: [Restore, Undo, Redo, Save]\n";
    std::vector<std::string> validInputs = { 
        "RESTORE",
        "UNDO",
        "REDO",
        "SAVE",
        "DILATE", 
        "ERODE", 
        "RESIZE", 
        "FLIP",
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
    
    record.push(current, "ORIGINAL");
    while (active) {
        if (updateRecord) { record.push(current, currentOp); }
        updateRecord = true;
        std::cout << record.toString() << std::endl;

        if (stringInputValidator(opInput, 3, promptString, validInputs)) {
            return -1;
        }
        stringToUpper(opInput);
        currentOp = opInput;
        
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
            case FLIP:
                flip(current, edited);
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
            case UNDO:
                current = record.getLast().clone();
                updateRecord = false;
                break;
            case REDO:
                current = record.getNext().clone();
                updateRecord = false;
                break;
            case SAVE:
                active = false;
                saveFile(edited, active);
                break;
            default:
                printf("-- Invalid operation request. --\n");
                break;
        }
        if (updateRecord) { 
            current = edited;
            record.push(current, currentOp); 
        }
    }

    // cap.close();
    cv::destroyAllWindows();
    return 0;
}
