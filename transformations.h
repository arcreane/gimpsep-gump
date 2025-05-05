#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

void dilate(cv::Mat& edited);

void erode(cv::Mat& edited);

void resize(cv::Mat& edited);

void lighten(cv::Mat& edited);

void darken(cv::Mat& edited);

void stitch(cv::Mat& edited);

void cannyEdgeDetection(cv::Mat& edited);

#endif /* TRANSFORMATIONS_H */
