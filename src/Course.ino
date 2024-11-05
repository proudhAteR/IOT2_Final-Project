#ifndef COURSE_H
#define COURSE_H

#include <Arduino.h>

// Define the Course structure
typedef struct
{
    const char *name;
    int weekDay;
    int startingHour;
    int endHour;
} Course;

// Function to get the starting hour
int getStartingHour(Course *course)
{
    return course->startingHour;
}

// Function to get the end hour
int getEndHour(Course *course)
{
    return course->endHour;
}

// Function to get the weekday
int getWeekDay(Course *course)
{
    return course->weekDay;
}

// Function to get the class name
const char *getClassName(Course *course)
{
    return course->name;
}

#endif
