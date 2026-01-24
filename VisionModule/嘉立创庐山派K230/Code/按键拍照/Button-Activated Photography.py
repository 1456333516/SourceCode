import time, os, sys
import utime
from media.sensor import *
from media.display import *
from media.media import *
from machine import Pin, FPIOA

# 修改照片存储路径
PHOTO_DIR = '/data/photo'

# FPIOA引脚配置
fpioa = FPIOA()
fpioa.set_function(52, FPIOA.GPIO52)  # LED引脚
fpioa.set_function(21, FPIOA.GPIO21)  # KEY引脚

'''
RGB_LED_R——BANK5_GPIO62
RGB_LED_G——BANK5_GPIO20
RGB_LED_B——BANK5_GPIO63
低电平点亮
按键53
按下高电平，使用下拉电阻
'''
# 初始化LED和按键
LED = Pin(20, Pin.OUT)  # LED初始熄灭
KEY = Pin(53, Pin.IN, Pin.PULL_DOWN)  # 按键使用上拉电阻

def ensure_photo_dir():
    """确保照片存储目录存在"""
    try:
        # 首先检查/data目录是否存在
        if not 'data' in os.listdir('/'):
            print("未找到data目录")
            return False

        # 确保photo目录存在
        if not 'photo' in os.listdir('/data'):
            try:
                os.mkdir(PHOTO_DIR)
                print(f"成功创建目录: {PHOTO_DIR}")
            except Exception as e:
                print(f"创建photo目录失败: {e}")
                return False
        return True
    except Exception as e:
        print(f"检查目录失败: {e}")
        return False

def get_next_photo_number():
    """获取下一个照片编号"""
    try:
        files = os.listdir(PHOTO_DIR)
        # 过滤出.jpg文件
        jpg_files = [f for f in files if f.endswith('.jpg')]
        if not jpg_files:
            return 0
        # 提取所有文件名中的数字
        numbers = [int(f.split('.')[0]) for f in jpg_files]
        # 返回最大数字+1
        return max(numbers) + 1
    except:
        return 0

def get_current_max_photo():
    """获取当前最大的照片编号"""
    try:
        files = os.listdir(PHOTO_DIR)
        jpg_files = [f for f in files if f.endswith('.jpg')]
        if not jpg_files:
            return "无照片"
        numbers = [int(f.split('.')[0]) for f in jpg_files]
        return f"{max(numbers)}.jpg"
    except:
        return "读取错误"

def save_image(img, photo_number):
    """保存图片"""
    try:
        photo_path = f"{PHOTO_DIR}/{photo_number}.jpg"
        # 直接使用save方法保存图片
        img.save(photo_path)
        return True
    except Exception as e:
        print(f"保存图片失败: {e}")
        return False

# 在try块外初始化sensor为None
sensor = None

try:
    # 确保photo文件夹存在
    if not ensure_photo_dir():
        raise Exception("初始化失败")

    # 获取下一个照片编号
    photo_number = get_next_photo_number()

    # 构造一个具有默认配置的摄像头对象
    sensor = Sensor()
    # 重置摄像头sensor
    sensor.reset()

    #设置帧大小800x480,LCD专用
    sensor.set_framesize(width=800,height=480)
    #sensor.set_framesize(Sensor.VGA)
    # 设置通道0的输出像素格式为RGB565
    sensor.set_pixformat(Sensor.RGB565)

    # 通过K230 3.1寸mipi显示屏显示图像
    Display.init(Display.ST7701, to_ide=False)

    # 初始化媒体管理器
    MediaManager.init()
    # 启动传感器
    sensor.run()

    # 添加时钟对象用于计算帧率
    clock = time.clock()
    # 确保LED熄灭
    LED.value(1)

    while True:
        os.exitpoint()
        clock.tick()  # 更新时钟

        # 捕获通道0的图像
        original_img = sensor.snapshot()  # 保存用的原始图像
        display_img = original_img.copy()  # 显示用的图像副本

        # 在右上角显示当前最大图片名称和帧率（只在显示图像上绘制）
        current_max = get_current_max_photo()
        # 使用绿色显示文字 (R=0, G=255, B=0)
        display_img.draw_string_advanced(400, 20, 30, f"最新: {current_max}", color=(0, 255, 0))
        # 显示帧率
        display_img.draw_string_advanced(400, 60, 30, f"FPS: {clock.fps():.1f}", color=(0, 255, 0))

        # 打印帧率到控制台
        print(f"FPS: {clock.fps():.1f}")

        # 检测按键
        if KEY.value() == 1:  # 按键按下
            time.sleep_ms(10)  # 消除抖动
            if KEY.value() == 1:  # 确认按键被按下
                print("检测到按键按下")
                LED.value(0)  # 拍照时点亮LED

                # 保存原始图片（不含文字）
                if save_image(original_img, photo_number):
                    print(f"已保存照片: {photo_number}.jpg")
                    photo_number += 1

                LED.value(1)  # 拍照完成后熄灭LED

                # 等待按键松开
                while KEY.value():
                    pass

        Display.show_image(display_img)

except KeyboardInterrupt as e:
    print("用户停止: ", e)
except BaseException as e:
    print(f"异常: {e}")
finally:
    # 停止传感器运行
    if sensor:
        sensor.stop()
    # 反初始化显示模块
    Display.deinit()
    os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
    time.sleep_ms(100)
    # 释放媒体缓冲区
    MediaManager.deinit()
    # 确保LED熄灭
    LED.value(1)
