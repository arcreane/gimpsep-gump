#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include "record.h"

Record::Record(int size)
{
    maxSize = size;
    record = {};
    opRecord = {};
    curIndex = 0;
}

void Record::push(cv::Mat& file, std::string op)  
{
    while (record.size() >= maxSize) {
        record.erase(record.begin());
        opRecord.erase(opRecord.begin());
    }
    
    record.push_back(file.clone());
    opRecord.push_back(op);
    curIndex = record.size() - 1;
}

cv::Mat Record::getLast() 
{
    if (curIndex > 0) {
        curIndex--;
    } else {
        printf("At oldest entry -- cannot get last.\n");
    }
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
    return record[curIndex];
}

std::string Record::toString()
{
    std::string str = "Current Recorded Operations:\n";
    for (int i = opRecord.size(); i --> 0;) {
        str += "| " + opRecord[i] + (i == curIndex ? " <<< {current}\n" : "\n");
    }
    return str;
}
