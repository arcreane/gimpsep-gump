#ifndef INPUTS_H
#define INPUTS_H
#include <algorithm>
#include <iostream>
#include "operations.h"

/// @brief file type to be tranformed
enum File {
    IMAGE,
    IMAGES,
    DIRECTORY,
    VIDEO
};

/// @brief mapping of string to operation object
const std::unordered_map<std::string, Operation*> OP_TABLE = { 
    {"HISTORY", new History()},
    {"RESTORE", new Restore()},
    {"UNDO", new Undo()},
    {"REDO", new Redo()},
    {"DILATE", new Dilate()},
    {"ERODE", new Erode()},
    {"RESIZE", new Resize()},
    {"FLIP", new Flip()},
    {"LIGHTEN", new Lighten()},
    {"DARKEN", new Darken()},
    {"STITCH", new Stitch()},
    {"CANNY", new Canny()},
    {"FACE", new Face()},
    {"SAVE", new Save()},
    {"EXIT", new Exit()}
};

#endif /* INPUTS_H */
