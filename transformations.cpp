#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "inputHelper.h"

/// @brief Dilate given file
/// @param current current file to be edited
/// @param edited edited output file
void dilate(cv::Mat& current, cv::Mat& edited)
{
    int iterations = 1;
    std::string promptIterString = "Please enter the desired iteration count [0-100]: ";
    std::pair<int, int> validIterRange (0, 100);
    if (intInputValidator(iterations, 3, promptIterString, validIterRange)) { return; }
    
    int kernelDims = 3;
    std::string promptKernelString = "Please enter the desired dilation size: ";
    std::pair<int, int> validKernelRange (0, std::min(current.size().height, current.size().width));
    if (intInputValidator(kernelDims, 3, promptKernelString, validKernelRange)) { return; }
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelDims, kernelDims), cv::Point(-1, -1));
    cv::dilate(current, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}

/// @brief Erode given file
/// @param current current file to be edited
/// @param edited edited output file
void erode(cv::Mat& current, cv::Mat& edited)
{
    int iterations = 1;
    std::string promptIterString = "Please enter the desired iteration count [0-100]: ";
    std::pair<int, int> validIterRange (0, 100);
    if (intInputValidator(iterations, 3, promptIterString, validIterRange)) { return; }

    int kernelDims = 3;
    std::string promptString = "Please enter the desired erosion size: ";
    std::pair<int, int> validRange (0, std::min(current.size().height, current.size().width));
    if (intInputValidator(kernelDims, 3, promptString, validRange)) { return; }
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelDims, kernelDims), cv::Point(-1, -1));
    cv::erode(current, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}

/// @brief Resize given file
/// @param current current file to be edited
/// @param edited edited output file
void resize(cv::Mat& current, cv::Mat& edited)
{
    double factor;
    std::string promptString = "Please enter the desired scaling percentage [0-1000%]: ";
    std::pair<double, double> validRange (0.0, 1000.0);
    if (doubleInputValidator(factor, 3, promptString, validRange)) { return; }
    double fx = factor / 100.0;
    double fy = factor / 100.0;
    int interpolation = (factor >= 100) ? cv::INTER_LINEAR : cv::INTER_AREA;

    cv::resize(current, edited, cv::Size(), fx, fy, interpolation);
}

/// @brief Lighten given file
/// @param current current file to be edited
/// @param edited edited output file
void lighten(cv::Mat& current, cv::Mat& edited)
{
    int beta;
    std::string promptString = "Please enter the lightening amount [0-255]: ";
    std::pair<int, int> validRange (0, 255);
    if (intInputValidator(beta, 3, promptString, validRange)) { return; }

    current.convertTo(edited, -1, 1, beta);
}

/// @brief Darken given file
/// @param current current file to be edited
/// @param edited edited output file
void darken(cv::Mat& current, cv::Mat& edited)
{
    int beta;
    std::string promptString = "Please enter the darkening amount [0-255]: ";
    std::pair<int, int> validRange (0, 255);
    if (intInputValidator(beta, 3, promptString, validRange)) { return; }

    current.convertTo(edited, -1, 1, -beta);
}

void stitch(cv::Mat& current, cv::Mat& edited)
{
    std::vector<cv::Mat> imgs;
    std::string modeString;
    std::string modePrompt = "Would you like to use SCAN or PANORMA stitching mode: ";
    std::vector<std::string> validModes = {"SCAN", "PANORAMA"};
    stringInputValidator(modeString, 3, modePrompt, validModes);

    parseStitchArgs(imgs, current);
    
    cv::Stitcher::Mode mode = modeString.compare("SCAN") ? cv::Stitcher::PANORAMA : cv::Stitcher::SCANS;
    // cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
    // cv::Stitcher::Status status = stitcher -> stitch(imgs, edited);

    // if (status != cv::Stitcher::OK)
    // {
    //     std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
    //     return;
    // }
}

/// @brief Utilize Canny algorithm to detect file edges
/// @param current current file to be edited
/// @param edited edited output file
void cannyEdgeDetection(cv::Mat& current, cv::Mat& edited)
{
    double lowerThreshold = 0;
    std::string promptLTString = "Please enter the lower threshold: ";
    std::pair<double, double> validLTRange (0.0, DBL_MAX);
    if (doubleInputValidator(lowerThreshold, 3, promptLTString, validLTRange)) { return; }

    double upperThreshold = 0;
    std::string promptUTString = "Please enter the upper Threshold: ";
    std::pair<double, double> validUTRange (lowerThreshold, DBL_MAX);
    if (doubleInputValidator(upperThreshold, 3, promptUTString, validUTRange)) { return; }

    int kernelSize;
    std::string promptKernelString = "Please enter the desires kernel size: ";
    std::pair<int, int> validKernelRange (0, INT_MAX);
    if (intInputValidator(kernelSize, 3, promptKernelString, validKernelRange)) { return; }

    cv::Canny(current, edited, lowerThreshold, upperThreshold, kernelSize);
}

/// @brief Removes all current unsaved edits and restores the original file
/// @param source source image to be restored
/// @param edited edited file to be overwritten
void restore(cv::Mat& source, cv::Mat& edited)
{
    std::string userInput;
    bool restore;
    std::string promptString = "Would you like to continue? [y/n]: ";

    printf("Restoring the image will wipe all current edits.\n");
    printf("There is no way to reverse this operation.\n");
    if (ynInputValidator(restore, 3, promptString)) { return; }

    if (restore)
    {
        edited = source;
    }
}

/// @brief Save the current edited file into or onto a file
/// @param edited edited file to be saved
/// @param active boolean set for to determine program continuation
void saveFile(cv::Mat& edited, bool& active)
{
    std::string outputName;
    std::string outPromptString = "Please enter the name of the output file (extension included):\n";
    std::vector<std::string> validInputs = {};
    if (stringInputValidator(outputName, 3, outPromptString, validInputs)) { return; }
    imwrite(outputName, edited);

    bool continueOps;
    std::string contPromptString = "Would you like to continue editing operations [y/n]: ";
    if (ynInputValidator(continueOps, 3, contPromptString)) { return; }
    active = continueOps;
}
