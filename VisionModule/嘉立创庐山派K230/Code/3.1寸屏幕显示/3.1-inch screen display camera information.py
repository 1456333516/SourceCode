import time, os, sys

import utime
from media.sensor import *
from media.display import *
from media.media import *


try:
    # 构造一个具有默认配置的摄像头对象
    sensor = Sensor()
    # 重置摄像头sensor
    sensor.reset()


    #设置帧大小800x480,LCD专用
    sensor.set_framesize(width=800,height=480)
    # 设置通道0的输出像素格式为RGB565  RGB888也行~
    sensor.set_pixformat(Sensor.RGB565)

    # 通过庐山派 3.1寸mipi显示屏显示图像
    Display.init(Display.ST7701, to_ide=False)
    # 初始化媒体管理器
    MediaManager.init()
    # 启动传感器
    sensor.run()

    while True:
        os.exitpoint()

        # 捕获通道0的图像
        img = sensor.snapshot()
        # 显示捕获的图像
        Display.show_image(img)


except KeyboardInterrupt as e:
    print("用户停止: ", e)
except BaseException as e:
    print(f"异常: {e}")
finally:
    # 停止传感器运行
    if isinstance(sensor, Sensor):
        sensor.stop()
    # 反初始化显示模块
    Display.deinit()
    os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
    time.sleep_ms(100)
    # 释放媒体缓冲区
    MediaManager.deinit()
