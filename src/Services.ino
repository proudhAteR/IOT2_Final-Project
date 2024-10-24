
#ifndef SERVICES_H
#define SERVICES_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <malloc.h>

#include "Course.ino"
class Services
{
private:
    const char *NO_CLASS_MESSAGE = "No ongoing class found";
    const int NUM_CLASSES = 3;
    const char *ssid = "";
    const char *password = "";
    static Course *classes;

public:
    Services(const char *ssid, const char *password)
    {
        this->ssid = ssid;
        this->password = password;

        classes = (Course *)malloc(NUM_CLASSES * sizeof(Course));

        memcpy(this->classes, getRoomClasses(), sizeof(Course) * NUM_CLASSES);
        connectToNetWork(ssid, password);
    };

private:
    Course *getRoomClasses()
    {
        static Course roomClasses[3] =
            {
                Course("Mobile", 8, 11, 4),
                Course("IOT", 15, 18, 2),
                Course("POO2", 8, 11, 1)
            };

        return roomClasses;
    }

private:
    void connectToNetWork(const char *ssid, const char *password)
    {
        Serial.begin(9600);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(1000);
        }

        Serial.println("\nConnected to the WiFi network");
        Serial.print("ESP32 IP: ");
        Serial.println(WiFi.localIP());
    }

public:
    void connectToBroker(WiFiClient wifiClient)
    {
        PubSubClient client(wifiClient);
        const char *domain = "broker.emqx.io";
        const char *topic = getTopic();

        while (!client.connected())
        {
            client.setServer(domain, 1883);
            if (client.connect("C1201_Card_Scanner"))
            {
                if (topic != NO_CLASS_MESSAGE)
                {
                    client.subscribe(topic);
                    Serial.println("Connected");
                    // client.setCallback(getPayload); The function to call
                }
                else
                {
                    // TODO : RED LIGHT
                }
            }
            else
            {
                delay(3000);
            }
        }
    };

private:
    const char *getTopic()
    {
        for (int i = 0; i < NUM_CLASSES; i++)
        {
            if (isOngoingClass(i))
            {
                static char buffer[256];
                sprintf(buffer, "C1201/%S", this->classes[i].getClassName());

                return buffer;
            }
        }
        return this->NO_CLASS_MESSAGE;
    }
    bool isOngoingClass(int i)
    {
        return isInClassRange(this->classes[i], getCurrentHour()) && isSameDay(this->classes[i], getWeekDay());
    };

private:
    boolean isInClassRange(Course course, int currentHour)
    {
        return course.getStartingHour() <= currentHour && course.getEndHour() > currentHour;
    };

private:
    boolean isSameDay(Course course, int weekDay)
    {
        return weekDay == course.getWeekDay();
    }

private:
    int getWeekDay()
    {
        //{"Sunday = 0", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        time_t currentTime = time(0);
        tm *now = localtime(&currentTime);

        return now->tm_wday;
    };

private:
    int getCurrentHour()
    {
        time_t currentTime = time(0);
        tm *now = localtime(&currentTime);

        return now->tm_hour;
    };
};
#endif