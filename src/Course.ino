#ifndef COURSE_H
#define COURSE_H

#include <Arduino.h>

class Course
{
    const char *name;
    int weekDay;
    int startingHour;

public:
    Course(const char *name,int weekday, int startingHour)
    {
        this->name = name;
        this->weekDay = weekDay;
        this->startingHour = startingHour;
    }
};

#endif