# 提交的改变

## 11-30 新建FreeRTOS项目

- 新建项目
- 新建bsp_log文件实现简单的串口打印
- 将日志系统完善了

### 目前自定义任务：

==Log_Task  堆栈大小：1024  优先级：1==



## 12-1 加入TFT底层

- 加入TFT显示底层
- 加入Encoder的底层和任务
- 加入读取光敏电阻adc值的任务

### 目前自定义任务：

==Log_Task  堆栈大小：1024  优先级：1==

==Encoder_Task 堆栈大小：128 优先级：osPriorityNormal2==

==Sensor_Task 堆栈大小：64 优先级：osPriorityNormal1==



## 12-3 加入DHT11模块驱动

- 加入DHT11底层和任务
- 加入命令队列控制继电器任务

### 目前自定义任务：

==Log_Task  堆栈大小：1024  优先级：1==

==Encoder_Task 堆栈大小：128 优先级：osPriorityNormal2==

==Sensor_Task 堆栈大小：64 优先级：osPriorityNormal1==

==DHT11_Task 堆栈大小：128 优先级：osPriorityNormal2==

==Relay_Task 堆栈大小：128 优先级：osPriorityNormal2==



## 12-4 加入ESP8266模块

- 非阻塞发送命令接收应答
- 测试可以连接上WIFI



