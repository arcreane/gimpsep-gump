#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <stdio.h>
#include "inputHelper.h"
#include "record.h"

class Operation {
    public:
        virtual int operate(Record& record) = 0;
        static const int OP_SUCCESS = 0;
        static const int OP_FAILURE = -1;
        static const int OP_EXIT = 1;
        static const int OP_SAVE = 10;

    private:
        std::string usage;
};

class Dilate: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Erode: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Resize: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Flip: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Lighten: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Darken: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Stitch: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Canny: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Face: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Restore: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Save: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Exit: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class History: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Undo: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

class Redo: public Operation {
    public: 
        int operate(Record& record);
    private:
        std::string usage = "";
};

#endif /* OPERATIONS_H */
