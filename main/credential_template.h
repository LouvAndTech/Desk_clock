#ifndef __CREDENTIAL_H__
#define __CREDENTIAL_H__
typedef struct {
    char* ssid;
    char* password;
}Credential;

const Credential credentials[6] = {
    {"SSID_1","PASSWORD_1"},
    {"SSID_2","PASSWORD_2"},
    {"SSID_3","PASSWORD_3"},
    {"SSID_4","PASSWORD_4"}
};

//You need to configure wich api you're using in the "config.h"
#define WEATHER_TOKEN "YOUR_API_TOKEN"

#endif