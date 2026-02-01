## STM32F407VET6 MainBoard V2.0 示例工程集

STM32F407VET6 MainBoard V2.0是一款基于ARM Cortex-M4内核的高性能微控制器开发板，运行频率高达168MHz。本项目包含16个完整的示例工程，涵盖了嵌入式开发中的常用外设和功能模块，包括模板工程、LED控制、按键输入、串口通信、OLED显示、灰度传感器、电机控制、编码器、多串口通信、PID控制算法、步进电机控制、九轴传感器、温湿度传感器、Flash存储等应用。所有工程均基于STM32 HAL库开发，提供了完整的硬件驱动、任务调度器和应用示例，适用于各种传感、控制、通信和数据采集应用场景。

### 目录

  - [说明](#说明)
  - [硬件平台](#硬件平台)
  - [工程列表](#工程列表)
    - [00_Template](#00_template)
    - [01_LED](#01_led)
    - [02_KEY](#02_key)
    - [03_UART](#03_uart)
    - [04_OLED](#04_oled)
    - [05_Grayscale](#05_grayscale)
    - [06_Motor](#06_motor)
    - [07_Encoder](#07_encoder)
    - [08_ICM20608](#08_icm20608)
    - [09_more_uart](#09_more_uart)
    - [10_PID](#10_pid)
    - [11_ZDT_Step_Motor](#11_zdt_step_motor)
    - [12_BNO08X](#12_bno08x)
    - [13_AHT20](#13_aht20)
    - [14_FLASH](#14_flash)
    - [Test_Example](#test_example)
  - [通用组件](#通用组件)
  - [快速开始](#快速开始)
  - [工程结构](#工程结构)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

本项目为STM32F407VET6 MainBoard V2.0开发板提供了16个完整的示例工程，每个工程都专注于特定的功能模块和外设应用。

### 硬件平台

- **主控芯片**: STM32F407VET6 (ARM Cortex-M4, 168MHz)
- **Flash**: 512KB
- **SRAM**: 192KB
- **开发板**: Mcore MainBoard V2.0
- **开发环境**: Keil MDK-ARM
- **调试接口**: SWD
- **电源**: 5V

### 工程列表

#### 00_Template

基础模板工程，提供完整的开发框架和任务调度器。

**主要特性**:
- 任务调度器框架 (Scheduler)
- 系统时钟配置
- GPIO基础配置
- 标准化工程结构
- 调试接口配置

**硬件连接**:
- 标准STM32F407VET6最小系统
- SWD调试接口

#### Test_Example

综合测试示例，集成菜单框架与多种外设功能验证。

**主要特性**:
- 全外设功能测试
- 系统稳定性验证
- 性能基准测试
- 自动化测试流程

### 通用组件

**Scheduler**: 任务调度器组件，提供时间片轮转和优先级调度功能。

**User Driver**: 用户驱动层，包含各种传感器和执行器的底层驱动。

**User App**: 用户应用层，实现具体的功能逻辑和算法。

**User Module**: 用户模块层，提供可重用的功能模块和中间件。

**MyDefine**: 用户定义头文件，包含全局宏定义和配置参数。

### 快速开始

参考每个工程目录下的具体说明，完成硬件连接和软件配置。

#### 环境准备

```bash
# 安装Keil MDK-ARM开发环境
# 准备STM32F407VET6开发板
# 连接ST-Link调试器
```

#### 工程编译

```bash
# 进入对应工程目录
cd 00_v2_RGB_Blink

# 使用Keil打开工程文件
# 双击 MDK-ARM/*.uvprojx 文件

# 编译工程
# 按F7或点击编译按钮
```

#### 程序下载

```bash
# 连接ST-Link到开发板
# 在Keil中配置调试器
# 按F8下载程序到Flash
```

## 开发指南

### 开发环境搭建
1. **安装Keil MDK-ARM**: 版本5.29或更高
2. **安装STM32CubeMX**: 用于配置外设和生成代码
3. **安装ST-Link驱动**: 用于程序下载和调试
4. **准备开发板**: STM32F407VET6 MainBoard V2.0

### 编译和下载流程
```bash
# 1. 打开Keil工程
# 双击对应工程目录下的 MDK-ARM/*.uvprojx 文件

# 2. 配置工程
# Project -> Options for Target -> Debug
# 选择ST-Link Debugger

# 3. 编译工程
# Project -> Build Target (F7)

# 4. 下载程序
# Flash -> Download (F8)

# 5. 运行调试
# Debug -> Start/Stop Debug Session (Ctrl+F5)
```

### 移植指南
如需移植到其他STM32F4系列芯片：
1. 修改STM32CubeMX配置文件(.ioc)
2. 重新生成代码
3. 检查引脚定义和时钟配置
4. 调整外设参数和任务调度器配置
5. 更新用户定义文件(MyDefine.h)
6. 重新编译和测试

## 技术支持

### 常见问题解答
1. **编译错误**: 检查Keil版本和HAL库配置，确认用户代码路径正确
2. **下载失败**: 检查ST-Link连接和驱动，验证调试器配置
3. **程序不运行**: 检查时钟配置、复位电路和任务调度器初始化
4. **外设不工作**: 检查硬件连接、引脚配置和驱动初始化
5. **任务调度异常**: 检查任务优先级设置和时间片分配

### 调试技巧
- 使用串口输出调试信息和任务状态
- 利用Keil调试器观察变量和任务执行
- 使用逻辑分析仪检查信号时序
- 添加LED指示程序和任务运行状态
- 利用任务调度器的统计功能监控系统负载

### 文档

**离线文档**: /doc目录包含开发板数据手册和驱动指南

**在线资源**:
- [STM32F407参考手册](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
- [STM32F4 HAL库文档](https://www.st.com/resource/en/user_manual/dm00105879.pdf)
- [STM32CubeMX用户手册](https://www.st.com/resource/en/user_manual/dm00104712.pdf)

### 贡献

欢迎提交代码贡献和改进建议：
1. Fork本项目
2. 创建功能分支
3. 提交代码更改
4. 发起Pull Request

- 
