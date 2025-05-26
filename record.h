#ifndef RECORD_H
#define RECORD_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdio.h>

/// @brief Record class to store operations sequences and handle sequence manipulation
class Record {
    public:
        int maxSize;
        Record(int size);
        void push(cv::Mat& file, std::string op);
        cv::Mat getSource();
        void setSource(cv::Mat& mat);
        cv::Mat getLast();
        cv::Mat getCurrent();
        cv::Mat getNext();
        std::string mostRecentOperation();
        void clear();
        std::string toString();

    private:
        std::vector<cv::Mat> record;
        std::vector<std::string> opRecord;
        int curIndex;
        cv::Mat source;
};

#endif /* RECORD_H */