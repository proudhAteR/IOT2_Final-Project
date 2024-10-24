#ifndef COURSE_H
#define COURSE_H

#include <Arduino.h>

class Course
{
    const char *name;
    int weekDay;
    int startingHour;
    int endHour;

public:
    Course(const char *name, int startingHour, int endHour, int weekday)
    {
        this->name = name;
        this->weekDay = weekDay;
        this->startingHour = startingHour;
    }

    public :
    int getStartingHour(){
        return this-> startingHour;
    }
    public :
    int getEndHour(){
        return this-> endHour;
    }

    public :
    int getWeekDay(){
        return this-> weekDay;
    }

    public :
    const char* getClassName(){
        return this-> name;
    }
};

#endif