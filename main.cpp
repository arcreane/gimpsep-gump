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
#include "operations.h"

#define RECORD_SIZE 100

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
        source = cv::imread(cv::samples::findFile(fileName, false, true), cv::IMREAD_COLOR);

        if (source.empty()) {
            retryCount--;
            if (retryCount) {
                std::cout << "No image data found for: '" << fileName << "'. Please enter a valid image file.\n" << std::endl;
            } else {
                std::cout << "No image data found for: '" << fileName << "'. Closing application.\n" << std::endl;
                return -1;
            }
        } else {
            return 0;
        }
    }

    return 0;
}

/// @brief Attempts to exit the program, checking for an updated save
/// @param record record of operations and data
/// @param active bool determining program continuation post operation
void attemptExit(Record record, bool& active)
{
    std::string mro = record.mostRecentOperation();
    if (mro.compare("SAVE") && mro.compare("ORIGINAL")) {
        printf("You have made changes since your last save.\n");
        std::string inputtedString;
        std::string promptString = "Would you like to [Cancel, Quit, Save]: ";
        std::vector<std::string> validInputs = {"CANCEL", "QUIT", "SAVE"};
        if (stringInputValidator(inputtedString, 3, promptString, validInputs)) { return; } 
        stringToUpper(inputtedString);

        if (!inputtedString.compare("SAVE")) {
            saveFile(record, active);
            return;
        } else if (!inputtedString.compare("CANCEL")) {
            active = true;
            return;
        }
    }
    active = false;
}

int main(int argc, char* argv[])
{
    Record record(RECORD_SIZE);
    cv::Mat source, current, edited;
    // Videocapture cap;
    Operation op = RESTORE;
    File f;
    
    bool active = true;
    bool updateRecord = true;
    std::string opInput;
    std::string promptString = "Please enter your desired operation:\n\
        Editing : [Dilate, Erode, Resize, Flip, Lighten, Darken, Stitch, Canny (for Edge Detection), Face (for face detection)]\n\
        File    : [History, Restore, Undo, Redo, Save, Exit]\n";
    std::vector<std::string> validInputs;
    for (auto kv : OP_TABLE) { validInputs.push_back(kv.first); }


    if (loadFile(source) == -1) {
        return -1;
    } else {
        current = source;  
    }

    record.setSource(source);
    record.push(current, "ORIGINAL");
    while (active) {        
        if (stringInputValidator(opInput, 3, promptString, validInputs)) {
            return -1;
        }
        stringToUpper(opInput);
        
        switch(OP_TABLE.at(opInput)) {
            case DILATE:
                dilate(record);
                break;
            case ERODE:
                erode(record);
                break;
            case RESIZE:
                resize(record);
                break;
            case FLIP:
                flip(record);
                break;
            case LIGHTEN:
                lighten(record);
                break;
            case DARKEN:
                darken(record);
                break;
            case STITCH:
                stitch(record);
                break;
            case CANNY:
                cannyEdgeDetection(record);
                break;
            case FACE:
                faceDetection(record);
                break;
            case HISTORY:
                std::cout << record.toString() << std::endl;
                break;
            case RESTORE:
                restore(record);
                break;
            case UNDO:
                current = record.getLast().clone();
                break;
            case REDO:
                current = record.getNext().clone();
                break;
            case SAVE:
                active = false;
                saveFile(record, active);
                break;
            case EXIT:
                attemptExit(record, active);
                break;
            default:
                printf("-- Invalid operation request. --\n");
                break;
        }

        cv::namedWindow("Editor Display", cv::WINDOW_AUTOSIZE);
        cv::imshow("Editor Display", record.getCurrent());
        cv::waitKey(0);
        cv::destroyWindow("Editor Display");
    }

    // cap.close();
    cv::destroyAllWindows();
    return 0;
}
