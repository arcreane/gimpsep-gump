#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "operations.h"
#include "inputHelper.h"
#include "record.h"

/// @brief Dilate given file
/// @param record record of recent images and their operations
int Dilate::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    int iterations = 1;
    std::string promptIterString = "Please enter the desired iteration count [0-100]: ";
    std::pair<int, int> validIterRange (0, 100);
    if (intInputValidator(iterations, 3, promptIterString, validIterRange)) { return OP_FAILURE; }
    
    int kernelDims = 3;
    std::string promptKernelString = "Please enter the desired dilation size: ";
    std::pair<int, int> validKernelRange (0, std::min(current.size().height, current.size().width));
    if (intInputValidator(kernelDims, 3, promptKernelString, validKernelRange)) { return OP_FAILURE; }
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelDims, kernelDims), cv::Point(-1, -1));
    cv::dilate(current, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());

    record.push(edited, "DILATE");
    std::cout << "Successfully dilated image with a kernel size of " << kernelDims << " " << iterations << " times." << std::endl;
    
    return OP_SUCCESS;
}

/// @brief Erode given file
/// @param record record of recent images and their operations
int Erode::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    int iterations = 1;
    std::string promptIterString = "Please enter the desired iteration count [0-100]: ";
    std::pair<int, int> validIterRange (0, 100);
    if (intInputValidator(iterations, 3, promptIterString, validIterRange)) { return OP_FAILURE; }

    int kernelDims = 3;
    std::string promptString = "Please enter the desired erosion size: ";
    std::pair<int, int> validRange (0, std::min(current.size().height, current.size().width));
    if (intInputValidator(kernelDims, 3, promptString, validRange)) { return OP_FAILURE; }
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelDims, kernelDims), cv::Point(-1, -1));
    cv::erode(current, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());

    record.push(edited, "ERODE");
    std::cout << "Successfully eroded image with a kernel size of " << kernelDims << " " << iterations << " times." << std::endl;  

    return OP_SUCCESS;
}

/// @brief Resize given file
/// @param record record of recent images and their operations
int Resize::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    double factor;
    std::string promptString = "Please enter the desired scaling percentage [0-1000%]: ";
    std::pair<double, double> validRange (0.0, 1000.0);
    if (doubleInputValidator(factor, 3, promptString, validRange)) { return OP_FAILURE; }
    double fx = factor / 100.0;
    double fy = factor / 100.0;
    int interpolation = (factor >= 100) ? cv::INTER_LINEAR : cv::INTER_AREA;

    cv::resize(current, edited, cv::Size(), fx, fy, interpolation);

    record.push(edited, "RESIZE");
    std::cout << "Successfully resized image by a factor of " << factor << "." << std::endl;

    return OP_SUCCESS;
}

/// @brief Flips given image either horizantally or vertically
/// @param record record of recent images and their operations
int Flip::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    int flipMode;
    std::vector<cv::Mat> imgs;
    std::string modeString;
    std::string modePrompt = "Would you like flip horizantally or vertically: ";
    std::vector<std::string> validModes = {"HORIZANTALLY", "VERTICALLY"};
    if (stringInputValidator(modeString, 3, modePrompt, validModes)) { return OP_FAILURE; }
    flipMode = modeString.compare("VERTICAL") ? 1 : 0;

    cv::flip(current, edited, flipMode);

    record.push(edited, "FLIP");
    std::cout << "Successfully flipped image " << modeString << "." << std::endl;
    return OP_SUCCESS;
}

/// @brief Lighten given file
/// @param record record of recent images and their operations
int Lighten::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    int beta;
    std::string promptString = "Please enter the lightening amount [0-255]: ";
    std::pair<int, int> validRange (0, 255);
    if (intInputValidator(beta, 3, promptString, validRange)) { return OP_FAILURE; }

    current.convertTo(edited, -1, 1, beta);

    record.push(edited, "LIGHTEN");
    std::cout << "Successfully lightened image by  " << beta << "." << std::endl;

    return OP_SUCCESS;
}

/// @brief Darken given file
/// @param record record of recent images and their operations
int Darken::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    int beta;
    std::string promptString = "Please enter the darkening amount [0-255]: ";
    std::pair<int, int> validRange (0, 255);
    if (intInputValidator(beta, 3, promptString, validRange)) { return OP_FAILURE; }

    current.convertTo(edited, -1, 1, -beta);

    record.push(edited, "DARKEN");
    std::cout << "Successfully darkened image by  " << beta << "." << std::endl;

    return OP_SUCCESS;
}

/// @brief stitches together a series of images in either SCAN or PANORAMA modes
/// @param record record of recent images and their operations
int Stitch::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    std::vector<cv::Mat> imgs;
    std::string modeString;
    std::string modePrompt = "Would you like to use SCAN or PANORAMA stitching mode: ";
    std::vector<std::string> validModes = {"SCAN", "PANORAMA"};
    if (stringInputValidator(modeString, 3, modePrompt, validModes)) { return OP_FAILURE; }

    parseStitchArgs(imgs, current);
    
    cv::Stitcher::Mode mode = modeString.compare("SCAN") ? cv::Stitcher::PANORAMA : cv::Stitcher::SCANS;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
    cv::Stitcher::Status status = stitcher -> stitch(imgs, edited);

    if (status != cv::Stitcher::OK)
    {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
        return OP_FAILURE;
    }

    record.push(edited, "STITCH");
    std::cout << "Successfully stitched images together in " << modeString << " mode." << std::endl;

    return OP_SUCCESS;
}

/// @brief Utilize Canny algorithm to detect file edges
/// @param record record of recent images and their operations
int Canny::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = cv::Mat();
    
    double lowerThreshold = 0;
    std::string promptLTString = "Please enter the lower threshold: ";
    std::pair<double, double> validLTRange (0.0, DBL_MAX);
    if (doubleInputValidator(lowerThreshold, 3, promptLTString, validLTRange)) { return OP_FAILURE; }

    double upperThreshold = 0;
    std::string promptUTString = "Please enter the upper Threshold: ";
    std::pair<double, double> validUTRange (lowerThreshold, DBL_MAX);
    if (doubleInputValidator(upperThreshold, 3, promptUTString, validUTRange)) { return OP_FAILURE; }

    int kernelSize;
    std::string promptKernelString = "Please enter the desires kernel size: ";
    std::pair<int, int> validKernelRange (0, INT_MAX);
    if (intInputValidator(kernelSize, 3, promptKernelString, validKernelRange)) { return OP_FAILURE; }

    cv::Canny(current, edited, lowerThreshold, upperThreshold, kernelSize);

    record.push(edited, "CANNY");
    std::cout << "Successfully computed edges with the thresholds (" << lowerThreshold << "," << upperThreshold \
        << ") and kernel size of " << kernelSize << "." << std::endl;

    return OP_SUCCESS;
}

/// @brief Uses Yunet model to identify faces in an image
/// @param record record of recent images and their operations
int Face::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    cv::Mat edited = record.getCurrent();
    cv::Mat faces = cv::Mat();
    
    std::string modelPath = "face_detection_yunet_2023mar.onnx";
    cv::Ptr<cv::FaceDetectorYN> detector = cv::FaceDetectorYN::create(modelPath, "", current.size());
    
    detector -> setInputSize(current.size());
    detector -> detect(current, faces);

    for (int i = 0; i < faces.rows; i++) {
        cv::Rect box( \
            faces.at<float>(i, 0), \
            faces.at<float>(i, 1), \
            faces.at<float>(i, 2), \
            faces.at<float>(i, 3));
        cv::rectangle(edited, box, cv::Scalar(10, 20, 255), 2);
    }

    record.push(edited, "FACE");
    std::cout << "Successfully detected " << faces.size().height << " faces." << std::endl;

    return OP_SUCCESS;
}

/// @brief Removes all current unsaved edits and restores the original file
/// @param record record of recent images and their operations
int Restore::operate(Record& record)
{
    std::string userInput;
    bool restore;
    std::string promptString = "Would you like to continue with restoration? [y/n]: ";

    printf("Restoring the image will wipe all current edits.\n");
    printf("There is no way to reverse this operation.\n");
    if (ynInputValidator(restore, 3, promptString)) { return OP_FAILURE; }

    if (restore)
    {
        cv::Mat source = record.getSource();
        record.clear();
        record.push(source, "ORIGINAL");
    }

    std::cout << "Successfully restored image." << std::endl;
    return OP_SUCCESS;
}

/// @brief Save the current edited file into or onto a file
/// @param record record of recent images and their operations
int Save::operate(Record& record)
{
    cv::Mat current = record.getCurrent();
    
    std::string outputName;
    std::string outPromptString = "Please enter the name of the output file (extension included): ";
    std::vector<std::string> validInputs = {};
    if (stringInputValidator(outputName, 3, outPromptString, validInputs)) { return OP_FAILURE; }
    imwrite(outputName, current);

    bool continueOps;
    std::string contPromptString = "Would you like to continue editing operations [y/n]: ";
    if (ynInputValidator(continueOps, 3, contPromptString)) { return OP_FAILURE; }
    
    record.push(current, "SAVE");
    std::cout << "Successfully saved image to  " << outputName << "." << std::endl;

    return continueOps ? OP_SUCCESS : OP_EXIT;
}

/// @brief Attempts to exit the program, checking for an updated save
/// @param record record of recent images and their operations
int Exit::operate(Record& record)
{
    std::string mro = record.mostRecentOperation();
    if (mro.compare("SAVE") && mro.compare("ORIGINAL")) {
        printf("You have made changes since your last save.\n");
        std::string inputtedString;
        std::string promptString = "Would you like to [Cancel, Quit, Save]: ";
        std::vector<std::string> validInputs = {"CANCEL", "QUIT", "SAVE"};
        if (stringInputValidator(inputtedString, 3, promptString, validInputs)) { return OP_FAILURE; } 
        stringToUpper(inputtedString);
        
        if (!inputtedString.compare("SAVE")) {
            return OP_SAVE;
        } else if (!inputtedString.compare("CANCEL")) {
            return OP_SUCCESS;
        }
    }

    std::cout << "Exiting program..." << std::endl;
    return OP_EXIT;
}

/// @brief Attempts to exit the program, checking for an updated save
/// @param record record of recent images and their operations
int History::operate(Record& record)
{
    std::cout << record.toString() << std::endl;
    return OP_SUCCESS;
}

/// @brief Attempts to exit the program, checking for an updated save
/// @param record record of recent images and their operations
int Undo::operate(Record& record)
{
    std::cout << "Undoing last operation." << std::endl;
    record.getLast();
    return OP_SUCCESS;
}

/// @brief Attempts to exit the program, checking for an updated save
/// @param record record of recent images and their operations
int Redo::operate(Record& record)
{
    std::cout << "Redoing last operation." << std::endl;
    record.getNext();
    return OP_SUCCESS;
}
