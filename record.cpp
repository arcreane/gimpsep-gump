#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "record.h"

Record::Record(int size)
{
    maxSize = size;
    record = {};
    opRecord = {};
    curIndex = 0;
    source = cv::Mat();
}

void Record::push(cv::Mat& file, std::string op)  
{
    while (record.size() >= maxSize) {
        record.erase(record.begin());
        opRecord.erase(opRecord.begin());
    }

    // while (record.size() - 1 != curIndex) {
    //     record.erase();
    //     opRecord.erase();
    // }
    
    record.push_back(file.clone());
    opRecord.push_back(op);
    curIndex = record.size() - 1;
}

cv::Mat Record::getSource()
{
    return source;
}

void Record::setSource(cv::Mat& mat)
{
    source = mat.clone();
}

cv::Mat Record::getLast() 
{
    if (curIndex > 0) {
        curIndex--;
    } else {
        printf("At oldest entry -- cannot get last.\n");
    }
    if (!opRecord[curIndex].compare("SAVE")) { return getLast(); }
    return record[curIndex];
}

cv::Mat Record::getCurrent()
{
    return record[curIndex];
}

cv::Mat Record::getNext() 
{
    if (curIndex < maxSize - 1 && curIndex < record.size() - 1) {
        curIndex++;
    } else {
        printf("At most recent entry -- cannot get next.\n");
    }

    if (!opRecord[curIndex].compare("SAVE")) { return getNext(); }
    return record[curIndex];
}

std::string Record::mostRecentOperation() 
{
    return opRecord[curIndex];
}

void Record::clear()
{
    while(record.size() > 0) { 
        record.erase(record.end()); 
        opRecord.erase(opRecord.end());
    }
}

std::string Record::toString()
{
    std::string str = "Current Recorded Operations:\n";
    int iterPoint = std::max(curIndex + 1, std::min(10, static_cast<int>(opRecord.size())));
    for (int i = iterPoint; i --> std::max(iterPoint - 10, 0);) {
        str += "| " + opRecord[i] + (i == curIndex ? " <<< {current}\n" : "\n");
    }
    return str;
}
