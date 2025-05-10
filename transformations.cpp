#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "inputHelper.h"

void dilate(cv::Mat& edited)
{
    // dilate current image or load new one
    cv::InputArray kernel = cv::Mat(); // cv::Mat(3, 3, cv::CV_8UC3, cv::Scalar::all(0));
    int iterations;
    std::string promptString = "Please enter the desired iterations: ";
    std::pair<int, int> validRange (0, 0);
    intInputValidator(iterations, 3, promptString, validRange);

    cv::dilate(edited, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}

void erode(cv::Mat& edited)
{
    // erode current image or load new one
    cv::InputArray kernel = cv::Mat();
    int iterations;
    std::string promptString = "Please enter the desired iterations: ";
    std::pair<int, int> validRange (0, 0);
    intInputValidator(iterations, 3, promptString, validRange);

    cv::erode(edited, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}

void resize(cv::Mat& edited)
{
    // resize current image or load new one
    int factor;
    std::string promptString = "Please enter the desired scaling percentage [0-1000]: ";
    std::pair<int, int> validRange (0, 1000);
    intInputValidator(factor, 3, promptString, validRange);

    double fx = edited.size().width * (factor / 100);
    double fy = edited.size().height * (factor / 100);
    int interpolation = (fx * fy >= edited.size().area()) ? cv::INTER_LINEAR : cv::INTER_AREA;

    cv::resize(edited, edited, cv::Size(), fx, fy, cv::INTER_LINEAR);
}

void lighten(cv::Mat& edited)
{
    // lighten current image or load new one
    int beta;
    std::string promptString = "Please enter the lightening amount [0-255]: ";
    std::pair<int, int> validRange (0, 255);
    intInputValidator(beta, 3, promptString, validRange);

    edited.convertTo(edited, -1, 1, beta);
}

void darken(cv::Mat& edited)
{
    // darken current image or load new one
    int beta;
    std::string promptString = "Please enter the darkening amount [0-255]: ";
    std::pair<int, int> validRange (0, 255);
    intInputValidator(beta, 3, promptString, validRange);

    edited.convertTo(edited, -1, 1, -beta);
}

void stitch(cv::Mat& edited)
{
    // stitch together images with or without current images
    // cv::stitch();
}

void cannyEdgeDetection(cv::Mat& edited)
{
    // perform canny edge detection on the current image or load new one
    double lowerThreshold = 0;
    double upperThreshold = 0;
    int kernelSize;
    std::string promptString = "Please enter the desired kernel size: ";
    std::pair<int, int> validRange (0, 0);
    intInputValidator(kernelSize, 3, promptString, validRange);

    cv::Canny(edited, edited, lowerThreshold, upperThreshold, kernelSize);
}
