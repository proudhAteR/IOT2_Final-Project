#ifndef SERVICES_H
#define SERVICES_H

#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <malloc.h>
#include <time.h>
#include "Course.ino"

// Constants
const char *NO_CLASS_MESSAGE = "No ongoing class found";
const int NUM_CLASSES = 3;

// Structure definition for the Service
typedef struct
{
    const char *ssid;
    const char *password;
    Course *classes;
} Services;

// Function declarations
Course *getRoomClasses();
void connectToNetwork(const char *ssid, const char *password);
void connectToBroker(WiFiClient wifiClient, Services *service);
const char *getTopic(Services *service);
bool isOngoingClass(int index, Services *service);
boolean isInClassRange(Course course, int currentHour);
boolean isSameDay(Course course, int weekDay);
int getCurrentWeekDay();
int getCurrentHour();

// Functions
Course *getRoomClasses()
{
    static Course roomClasses[2] = {
        {"Mobile", 5, 8, 11},
        {"IOT", 2, 15, 20}};
    return roomClasses;
}

void initializeService(Services *service, const char *ssid, const char *password)
{
    service->ssid = ssid;
    service->password = password;
    service->classes = (Course *)malloc(NUM_CLASSES * sizeof(Course));
    memcpy(service->classes, getRoomClasses(), sizeof(Course) * NUM_CLASSES);

    connectToNetwork(ssid, password);
}

void connectToNetwork(const char *ssid, const char *password)
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
}

void connectToBroker(WiFiClient wifiClient, Services *service)
{
    PubSubClient client(wifiClient);
    const char *domain = "broker.emqx.io";
    const char *topic = getTopic(service);
    boolean connected;

    while (!client.connected() || !connected)
    {
        client.setServer(domain, 1883);
        if (client.connect("C1201_Card_Scanner"))
        {
            if (topic != NO_CLASS_MESSAGE)
            {
                client.subscribe(topic);
                connected = true;
                // client.setCallback(getPayload); // Function to call
            }
            else
            {
                connected = false;
                delay(10000);
            }
        }
        else
        {
            Serial.println(".");
            delay(10000);
        }
    }
}

const char *getTopic(Services *service)
{
    for (int i = 0; i < NUM_CLASSES; i++)
    {
        if (isOngoingClass(i, service))
        {
            static char buffer[256];
            sprintf(buffer, "C1201/%s", getClassName(&service->classes[i]));
            return buffer;
        }
    }
    return NO_CLASS_MESSAGE;
}

bool isOngoingClass(int index, Services *service)
{
    return isInClassRange(service->classes[index], getCurrentHour()) &&
           isSameDay(service->classes[index], getCurrentWeekDay());
}

boolean isInClassRange(Course course, int currentHour)
{
    return getStartingHour(&course) <= currentHour && getEndHour(&course) > currentHour;
}

boolean isSameDay(Course course, int weekDay)
{
    return weekDay == getCurrentWeekDay();
}
int getCurrentWeekDay()
{
    // {"Sunday = 0", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    return now->tm_wday;
}

int getCurrentHour()
{
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    setenv("TZ", "EST5EDT", 1);
    tzset();
    return now->tm_hour;
}

#endif
