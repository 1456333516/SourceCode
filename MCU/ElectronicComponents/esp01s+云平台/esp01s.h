#ifndef _ESP01S_H
#define _ESP01S_H

#include "bsp_system.h"

#define WIFI_SSID	            		"N504"      //wifi√˚≥∆ 
#define WIFI_PWD                		"12345679" //wifi√‹¬Î

#define HUAWEI_MQTT_ADDRESS      		"e3d70f6c17.st1.iotda-device.cn-south-1.myhuaweicloud.com"
#define HUAWEI_MQTT_ClientID            "67bec08580c3872914ade8c0_smart_helmet_2_0_0_2025031808"
#define HUAWEI_MQTT_USERNAME            "67bec08580c3872914ade8c0_smart_helmet_2"
#define HUAWEI_MQTT_PASSWORD            "e3e791c784ae70eb9350668b81ecb88c0caa8ed48ff62fbd71be0be3f9767d64"
#define HUAWEI_MQTT_PORT				"1883"
#define	HUAWEI_MQTT_PUBLISH_TOPIC 		"$oc/devices/67bec08580c3872914ade8c0_smart_helmet_2/sys/properties/report"

void esp_init(void);
void esp_report(void);


#endif
