#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "record.h"

/// @brief Record class constructor 
/// @param size max size of the record
Record::Record(int size)
{
    maxSize = size;
    record = {};
    opRecord = {};
    curIndex = 0;
    source = cv::Mat();
}

/// @brief pushes a new Mat into the record with its operation, removing the oldest if over max size
/// @param file the new file to be added to the record
/// @param op the new operation to be added to the record
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

/// @brief gets the original source file
/// @return the original source Mat
cv::Mat Record::getSource()
{
    return source;
}

/// @brief sets the original source file into the record
/// @param mat the source mat to be stored
void Record::setSource(cv::Mat& mat)
{
    source = mat.clone();
}

/// @brief gets the previous item in the record and moves the current index back
/// @return the previous item in the record
cv::Mat Record::getLast() 
{
    if (curIndex > 0) {
        curIndex--;
        if (!opRecord[curIndex].compare("SAVE")) { return getLast(); }
    } else {
        printf("At oldest entry -- cannot get last.\n");
    }

    return record[curIndex];
}

/// @brief gets the item in the record pointed to by the current index
/// @return the current item in the record
cv::Mat Record::getCurrent()
{
    return record[curIndex];
}

/// @brief gets the next item in the record and moves the current index forward
/// @return the next item in the record
cv::Mat Record::getNext() 
{
    if (curIndex < maxSize - 1 && curIndex < record.size() - 1) {
        curIndex++;
        if (!opRecord[curIndex].compare("SAVE")) { return getNext(); }
    } else {
        printf("At most recent entry -- cannot get next.\n");
    }

    return record[curIndex];
}

/// @brief returns the most recent operation from the record
/// @return the most recent operation on record
std::string Record::mostRecentOperation() 
{
    return opRecord[curIndex];
}

/// @brief clears the record of all values and operations
void Record::clear()
{
    while(record.size() > 0) { 
        record.erase(record.end()); 
        opRecord.erase(opRecord.end());
    }
}

/// @brief generates a string representation of the current record
/// @return the string representation of the current top 10 items in the record
std::string Record::toString()
{
    std::string str = "Current Recorded Operations:\n";
    int iterPoint = std::max(curIndex + 1, std::min(10, static_cast<int>(opRecord.size())));
    for (int i = iterPoint; i --> std::max(iterPoint - 10, 0);) {
        str += "| " + opRecord[i] + (i == curIndex ? " <<< {current}\n" : "\n");
    }
    return str;
}
