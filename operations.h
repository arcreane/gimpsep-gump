#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <algorithm>
#include <iostream>

/// @brief file type to be tranformed
enum File {
    IMAGE,
    IMAGES,
    DIRECTORY,
    VIDEO
};

/// @brief requested operation type
enum Operation {
    HISTORY,
    RESTORE,
    UNDO,
    REDO,
    DILATE,
    ERODE,
    RESIZE,
    FLIP,
    LIGHTEN,
    DARKEN,
    STITCH,
    CANNY,
    SAVE,
    EXIT
};

/// @brief mapping of string to operation type
const std::unordered_map<std::string, Operation> OP_TABLE = { 
    {"HISTORY", HISTORY},
    {"RESTORE", RESTORE},
    {"UNDO", UNDO},
    {"REDO", REDO},
    {"DILATE", DILATE},
    {"ERODE", ERODE},
    {"RESIZE", RESIZE},
    {"FLIP", FLIP},
    {"LIGHTEN", LIGHTEN},
    {"DARKEN", DARKEN},
    {"STITCH", STITCH},
    {"CANNY", CANNY},
    {"SAVE", SAVE},
    {"EXIT", EXIT}
};

#endif /* OPERATIONS_H */
