
#ifndef SERVICES_H
#define SERVICES_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class Services
{
private:
    const char *ssid = "";
    const char *password = "";

public:
    Services(const char *ssid, const char *password)
    {
        this->ssid = ssid;
        this->password = password;

        connectToNetWork(ssid, password);
    };

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
        const char *topic = getTopic(); // the topic will vary in function of the ongoing class

        while (!client.connected())
        {
            client.setServer(domain, 1883);
            Serial.println("Attempting Connection");
            if (client.connect("Christian"))
            {
                client.subscribe(topic);
                Serial.println("Connected");
                // client.setCallback(getPayload); //The function to call
            }
            else
            {
                Serial.println("Not Connected :(");
                //
                delay(3000);
            }
        }
    };

private:
    const char *getTopic()
    {
        int weekday = getWeekDay();
        const char *stringWd = getWeekDayAsString(weekday);
    };

private:
    int getWeekDay()
    {
        time_t currentTime = time(0);
        tm *now = localtime(&currentTime);

        return now->tm_wday;
    }

private:
    const char *getWeekDayAsString(int wd)
    {
        const char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        return days[wd];
    };
};

#endif