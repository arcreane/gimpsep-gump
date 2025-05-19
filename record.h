#ifndef RECORD_H
#define RECORD_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <stdio.h>

class Record {
    public:
        int maxSize;
        Record(int size);
        void push(cv::Mat& file, std::string op);
        cv::Mat getLast();
        cv::Mat getCurrent();
        cv::Mat getNext();
        std::string toString();

    private:
        std::vector<cv::Mat> record;
        std::vector<std::string> opRecord;
        int curIndex;

};

#endif /* RECORD_H */