#ifndef COURSE_H
#define COURSE_H

#include <Arduino.h>

typedef struct
{
    const char *name;
    int weekDay;
    int startingHour;
    int endHour;
} Course;

int getStartingHour(Course *course)
{
    return course->startingHour;
}

int getEndHour(Course *course)
{
    return course->endHour;
}

int getWeekDay(Course *course)
{
    return course->weekDay;
}

const char *getClassName(Course *course)
{
    return course->name;
}

#endif
