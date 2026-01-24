#include "esp01s.h"

void esp_init(void)
{
    my_printf(&huart3,"AT+RST\r\n");
    HAL_Delay(1000);
    my_printf(&huart3,"AT+CWMODE=1\r\n");
    HAL_Delay(1000);
    my_printf(&huart3,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI_SSID,WIFI_PWD);
    HAL_Delay(4000);
    my_printf(&huart3,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n",HUAWEI_MQTT_USERNAME,HUAWEI_MQTT_PASSWORD);
    HAL_Delay(1000);
    my_printf(&huart3,"AT+MQTTCLIENTID=0,\"%s\"\r\n",HUAWEI_MQTT_ClientID);
    HAL_Delay(1000);
    my_printf(&huart3,"AT+MQTTCONN=0,\"%s\",1883,1\r\n",HUAWEI_MQTT_ADDRESS);
    HAL_Delay(1000);
}

void esp_report(void)
{
    my_printf(&huart3,"AT+MQTTPUB=0,\"%s\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"BasicData\\\"\\,\\\"properties\\\":{\\\"spO2\\\":%d\\,\\\"density\\\":%f\\,\\\"heart_rate\\\":%d\\,\\\"fall_flag\\\":%d\\,\\\"collision_flag\\\":%d}}]}\",0,0\r\n",HUAWEI_MQTT_PUBLISH_TOPIC,dis_spo2,ppm,dis_hr,fall_flag,collision_flag);
    HAL_Delay(500);
    my_printf(&huart3,"AT+MQTTPUB=0,\"%s\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"BasicData\\\"\\,\\\"properties\\\":{\\\"longitude\\\":%f\\,\\\"latitude\\\":%f\\,\\\"temperature\\\":%d\\,\\\"humidity\\\":%d}}]}\",0,0\r\n",HUAWEI_MQTT_PUBLISH_TOPIC,longitude,latitude,temp,humi);
    HAL_Delay(500);
}
