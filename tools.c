//header.h
// Created by charname on 7/15/19.
//

#include "header.h"

long long getSystemTime() {
    struct timeb time;
    ftime(&time);
    return time.time * 1000 + time.millitm;
}

