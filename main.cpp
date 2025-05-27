#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdio.h>
#include "operations.h"
#include "inputHelper.h"
#include "record.h"
#include "inputs.h"

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

int main(int argc, char* argv[])
{
    Record record(RECORD_SIZE);
    cv::Mat source;
    // Videocapture cap;
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
        source;  
    }

    record.setSource(source);
    record.push(source, "ORIGINAL");
    
    while (active) {        
        if (stringInputValidator(opInput, 3, promptString, validInputs)) {
            return -1;
        }
        stringToUpper(opInput);
        
        int opStatus = OP_TABLE.at(opInput) -> operate(record);
        switch(opStatus){
            case Operation::OP_FAILURE:
                std::cout << "Operation failed: " << opInput << std::endl;
                break;
            case Operation::OP_SUCCESS:
                break;
            case Operation::OP_EXIT:
                active = false;
                break;
            case Operation::OP_SAVE:
                active = OP_TABLE.at("SAVE") -> operate(record) != Operation::OP_EXIT;
                break;
            default:
                std::cout << "Invalid status: " << std::to_string(opStatus) << std::endl;
                break;
        }

        // cv::namedWindow("Editor Display", cv::WINDOW_AUTOSIZE);
        // cv::imshow("Editor Display", record.getCurrent());
        // cv::waitKey(0);
        // cv::destroyWindow("Editor Display");
    }

    // cap.close();
    cv::destroyAllWindows();
    return 0;
}
