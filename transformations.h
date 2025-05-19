#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <stdio.h>

void dilate(cv::Mat& current, cv::Mat& edited);

void erode(cv::Mat& current, cv::Mat& edited);

void resize(cv::Mat& current, cv::Mat& edited);

void flip(cv::Mat& current, cv::Mat& edited);

void lighten(cv::Mat& current, cv::Mat& edited);

void darken(cv::Mat& current, cv::Mat& edited);

void stitch(cv::Mat& current, cv::Mat& edited);

void cannyEdgeDetection(cv::Mat& current, cv::Mat& edited);

void restore(cv::Mat& source, cv::Mat& edited);

void saveFile(cv::Mat& edited, bool& active);

#endif /* TRANSFORMATIONS_H */
