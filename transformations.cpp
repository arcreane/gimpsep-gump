#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

void dilate(cv::Mat& edited)
{
    // dilate current image or load new one
    cv::InputArray kernel = cv::Mat(); // cv::Mat(3, 3, cv::CV_8UC3, cv::Scalar::all(0));
    int iterations = 2;

    cv::dilate(edited, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}

void erode(cv::Mat& edited)
{
    // erode current image or load new one
    cv::InputArray kernel = cv::Mat();
    int iterations = 2;

    cv::erode(edited, edited, kernel, cv::Point(-1, -1), iterations, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}

void resize(cv::Mat& edited)
{
    // resize current image or load new one
    // ADD FACTOR SUPPORT
    double fx = 100;
    double fy = 100;
    int interpolation;

    interpolation = (fx * fy >= edited.size().area()) ? cv::INTER_LINEAR : cv::INTER_AREA;

    cv::resize(edited, edited, cv::Size(), fx, fy, cv::INTER_LINEAR);
}

void lighten(cv::Mat& edited)
{
    // lighten current image or load new one
    int beta = 50;

    edited.convertTo(edited, -1, 1, beta);
}

void darken(cv::Mat& edited)
{
    // darken current image or load new one
    int beta = -50;

    edited.convertTo(edited, -1, 1, beta);
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
    int kernelSize = 3;

    cv::Canny(edited, edited, lowerThreshold, upperThreshold, kernelSize);
}
