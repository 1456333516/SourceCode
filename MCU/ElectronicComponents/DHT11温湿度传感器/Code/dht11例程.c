
#include "dht11.h"

float    temperature;
float    humidity;

int main(void)
{			
	DHT11_Init();
	
	while (1)
	{

	}
}

void Get_Temp(void)
{
	DHT11_Data_TypeDef DHT11_Data;

	if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS)
	{
		//printf("读取DHT11成功!-->湿度为%.1f ％RH ，温度为 %.1f℃ \n",DHT11_Data.humidity,DHT11_Data.temperature);
		temperature = DHT11_Data.temperature;
		humidity = DHT11_Data.humidity;
	}
	else
	{
		//printf("失败 \n");
		;
	}	
}

