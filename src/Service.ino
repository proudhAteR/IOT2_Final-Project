#ifndef SERVICE_H
#define SERVICE_H

#include "Arduino.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "Course.ino"
#include <ESP32Time.h>

const char *NO_CLASS_MESSAGE = "No ongoing class found";
const int MAX_CLASSES = 2;
const int BUFFER_SIZE = 256;

typedef struct
{
    const char *ssid;
    const char *password;
    Course classes[MAX_CLASSES];
} Service;

// Function declarations
void connectToNetwork(const char *ssid, const char *password);
void connectToBroker(PubSubClient &client, Service *service);
void subscribe(PubSubClient &client, const char *topic);
const char *getTopic(Service *service);
bool isOngoingClass(int index, Service *service);
bool isInClassRange(Course &course, int currentHour);
bool isSameDay(Course &course, int weekDay);
int getCurrentWeekDay();
int getCurrentHour();
void handleConnectionError();
void initializeService(Service *service, const char *ssid, const char *password);

void initializeService(Service *service, const char *ssid, const char *password)
{
    // Initialize a single example class
    service->classes[0] = {"IOT", 6, 4, 23};

    connectToNetwork(ssid, password);
    configTime((3600 * (-5)), 3600, "pool.ntp.org");
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
    Serial.println("\nConnected to WiFi");
}

void connectToBroker(PubSubClient &client, Service *service)
{
    const char *esp_id = "C1201_Card_Scanner";
    const char *domain = "35.183.155.93";
    const char *username = "ecosystem-connecte";
    const char *password = "Omega123*";

    while (!client.connected())
    {
        client.setServer(domain, 1883);
        if (client.connect(esp_id, username, password))
        {
            const char *topic = getTopic(service);
            if (strcmp(topic, NO_CLASS_MESSAGE) != 0) 
            {
                subscribe(client, topic);
            }
            Serial.println(topic);
        }
        else
        {
            handleConnectionError();
        }
    }
}

void subscribe(PubSubClient &client, const char *topic)
{
    client.subscribe(topic);
    Serial.println("Subscribed to topic");
}

void handleConnectionError()
{
    Serial.println("Connection to broker failed. Retrying...");
    delay(5000);
}

const char *getTopic(Service *service)
{
    static char buffer[BUFFER_SIZE];
    for (int i = 0; i < MAX_CLASSES; i++)
    {
        if (isOngoingClass(i, service))
        {
            snprintf(buffer, BUFFER_SIZE, "C1201/%s", getClassName(&service->classes[i]));
            return buffer;
        }
    }
    return NO_CLASS_MESSAGE;
}

bool isOngoingClass(int index, Service *service)
{
    if (index < 0 || index >= MAX_CLASSES)
        return false; // Prevent invalid index access
    return isInClassRange(service->classes[index], getCurrentHour()) &&
           isSameDay(service->classes[index], getCurrentWeekDay());
}

bool isInClassRange(Course &course, int currentHour)
{
    return getStartingHour(&course) <= currentHour && getEndHour(&course) > currentHour;
}

bool isSameDay(Course &course, int weekDay)
{
    return weekDay == getWeekDay(&course);
}

int getCurrentWeekDay()
{
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    if (now == nullptr)
        return -1; // Ensure valid pointer
    return now->tm_wday;
}

int getCurrentHour()
{
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    if (now == nullptr)
        return -1; // Ensure valid pointer
    return now->tm_hour;
}

#endif
