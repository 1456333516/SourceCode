#include "atgm336.h" 

float longitude; // 经度
float latitude;  // 纬度

//以下解析程序放到串口解析程序中 
void atgm336h_process(unsigned char *uartReadBuff)
{
    // 查找字符串 "$GNGGA" 和 "\r\n$GNGLL"，分别表示开始和结束的标志
    char *start = strstr((const char *)uartReadBuff, "$GNGGA");
    char *end = strstr((const char *)uartReadBuff, "\r\n$GNGLL");

    // 如果没有找到这两个标志，说明没有找到有效的 GPS 数据
    if (start == NULL || end == NULL)
    {
        // 如果需要调试，可以打印出来
        // printf("[GPS]$GNGGA line NOT found\r\n");
    }
    else
    {
        // 成功找到开始和结束标志，提取数据
        // 创建一个足够存储 $GNGGA 数据的缓冲区
        char gngga[100];

        // 从 uartReadBuff 中提取从 $GNGGA 到 $GNGLL 之间的数据
        strncpy(gngga, start, end - start);
        gngga[end - start] = '\0'; // 确保字符串以 NULL 结尾

        // 打印提取的 GNGGA 数据（调试用）
//        printf("---%s\r\n", gngga);

        // 定义分隔符和一个数组来存储解析出的字段
        char *token;
        token = strtok(gngga, ","); // 使用逗号分隔每个字段
        char *nmea_fields[15];      // 最多支持 15 个字段
        int i = 0;

        // 逐个字段提取并存入 nmea_fields 数组中
        while (token != NULL)
        {
            nmea_fields[i] = token;
            token = strtok(NULL, ","); // 获取下一个字段
            i++;
            if (i >= 15)
                break; // 限制字段数为 15
        }

        // 如果提取到的字段大于 6，说明数据有效
        if (i > 6)
        {
            // 成功提取到数据，处理经纬度
//            printf("[GPS]data found\r\n");

//            // 将提取到的经度和纬度数据拼接到字符串中
//            sprintf(longitude, "%s %s", nmea_fields[2], nmea_fields[3]);
//            sprintf(latitude, "%s %s", nmea_fields[4], nmea_fields[5]);

//            // 打印经纬度数据（调试用）
//            printf("[GPS]%s %s,%s %s\r\n", nmea_fields[2], nmea_fields[3], nmea_fields[4], nmea_fields[5]);
            
            // 解析纬度
            int lat_deg = (int)(atof(nmea_fields[2]) / 100); // 取出度数（例如：2056.122314 -> 20）
            double lat_min = atof(nmea_fields[2]) - (lat_deg * 100); // 取出分数（例如：2056.122314 - 20*100 = 56.122314）

            // 计算纬度
            float latitude_decimal = lat_deg + (lat_min / 60);
            if (nmea_fields[3][0] == 'S') // 如果是南纬，取负
                latitude_decimal = -latitude_decimal;

            // 解析经度
            int lon_deg = (int)(atof(nmea_fields[4]) / 100); // 取出度数（例如：11002.398438 -> 110）
            double lon_min = atof(nmea_fields[4]) - (lon_deg * 100); // 取出分数（例如：11002.398438 - 110*100 = 2.398438）

            // 计算经度
            float longitude_decimal = lon_deg + (lon_min / 60);
            if (nmea_fields[5][0] == 'W') // 如果是西经，取负
                longitude_decimal = -longitude_decimal;
            
            longitude=longitude_decimal; // 经度
			latitude=latitude_decimal;  // 纬度
            
             // 打印转换后的经纬度数据（调试用）
//            printf("[GPS]Latitude: %.6f, Longitude: %.6f\r\n", latitude_decimal, longitude_decimal);
        }
        else
        {
            // 如果字段数不够，说明数据无效
//            printf("[GPS]data NOT found\r\n");
        }
    }
    // printf("---GPS_END---\r\n"); // 可选的调试输出
}

