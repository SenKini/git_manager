#pragma once
#include <qstring.h>

enum ErrorType {
    GIT_ERROR,
    PERSISTENT_ERROR
};

static QString errorMess;
static ErrorType errorType;