#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <stdio.h>
#include "inputHelper.h"
#include "record.h"

void dilate(Record& record);

void erode(Record& record);

void resize(Record& record);

void flip(Record& record);

void lighten(Record& record);

void darken(Record& record);

void stitch(Record& record);

void cannyEdgeDetection(Record& record);

void faceDetection(Record& record);

void restore(Record& record);

void saveFile(Record& record, bool& active);

#endif /* TRANSFORMATIONS_H */
