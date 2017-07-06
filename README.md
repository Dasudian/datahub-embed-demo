# 介绍

该项目提供一个基于嵌入式C SDK的Demo程序.

本文档介绍了当前目录结构和每个文件的作用,如何编译和运行demo程序, 以及如何开发你自己的程序. 如果你想知道API的细节,请阅读docs目录下的API文档或者头文件[DataHubClient.h](./include/DataHubClient.h)

## 目录结构说明

有五大目录：docs, include, lib, project_template, demo

### docs目录

API.pdf 包含了SDK的所有API, 包括需要用户实现的网络接口和定时器接口

### include目录

*DataHubClient.h*定义了Datahub的接口,接口以"datahub_"开头

*DatahubNetwork.h*定义了网络接口. 这些接口需要用户来实现并由SDK调用

*DatahubTimer.h*定义了定时器接口. 这些接口用于同步超时的接口和函数. 需要用户实现并由SDK调用

*inc*目录下是SDK使用的其他头文件, 用户不需要关心这些文件, 只需要保证这些文件包含在项目中即可

### lib目录

该目录下有最新的SDK库

### project_template 目录

该目录包含了一个模板工程, 用于帮助用户快速创建工程

进入以"STM32F"开头的目录:

*DATAHUB_API*目录包含了所有的API, 包括用户需要实现的网络接口和定时器接口

*pro_template.uvprojx* 是模板的工程文件(基于Keil)

*USER/DatahubNetwork.c* 实现了网络通信接口(目前为空), 这些接口在[DatahubNetwork.h](./include//DatahubNetwork.h)定义.

*USER/DatahubTimer.c* 实现了定时器接口(目前为空), 这些接口在[DatahubTimer.h](./include/DatahubTimer.h)定义.

*USER/main.c* 为程序的入口函数main()

*timer* 目录包含了针对某些操作系统的定时器接口的实现. 用户可以参考其实现

### demo目录

该demo基于LWIP-1.4.1, ucosII, 和板子STM32F4 EXPLORER

#### 核心文件和目录

*DATAHUB_API*目录包含了所有的API, 包括用户需要实现的网络接口和定时器接口

*demo.uvprojx* 是模板的工程文件(基于Keil)

[DatahubNetwork.c](./demo/USER/DatahubNetwork.c) 实现了网络通信接口

[DatahubTimer.c](./demo/USER/DatahubTimer.c) 实现了定时器接口

[datahub_demo.c](./demo/USER/datahub_demo.c) 向用户展示如何使用SDK

[main.c](./demo/USER/main.c) 为程序的入口函数main()

*忽略其他目录和文件*

## 编译和运行demo

编译器: keil 5.22

环境: windows 7 x64 Home Basic

调试器: ST-Link

板子: ALIENTEK的STM32F4 EXPLORER

使用Keil打开demo.uvprojx(demo/demo.uvprojx)工程目录文件,直接编译下载即可. 只能在STM32F4 EXPLORER使用

*如果你想把该demo移植到你的板子上, 请空出时间并耐心移植*

## 如何开发自己的程序

- 成功编译你板子的固件. 这是最基础的步骤之一.

- 移植uCOSII, FreeRTOS 或者其他的嵌入式操作系统. 这是最基础的步骤之一.

- 实现定义在[DatahubNetwork.h](./include/DatahubNetwork.h)的接口, 包括ConnectNetwork(), ucos_read(), ucos_write(), ucos_disconnect(). 这些API用于连接/断开网络及从网络中收发数据. 如果没有实现这些接口, 编译器将会报错.(demo在文件[DatahubNetwork.c](./demo/USER/DatahubNetwork.c)实现了这些接口)

- 实现定义在[DatahubTimer.h](./include/DatahubTimer.h)的接口, 包括InitTimer(), expired(), countdown_ms(), countdown(), left_ms(). 这些接口由SDK调用. 如果没有实现这些接口, 编译器将会报错.(demo在文件[DatahubTimer.c](./demo/USER/DatahubTimer.c)实现了这些接口)

- 使用[DataHubClient.h](./include/DataHubClient.h)定义的接口连接服务器并收发数据.(demo在[datahub_demo.c](./demo/USER/datahub_demo.c)实现了这些功能)

- 为了便于调试, 我们调用datahub_printf()输出了很多调试信息, 但是需要用户实现该函数. 如果重定向了输出, 可以取消注释'vprintf(format, ap);'

```
#include <stdarg.h>
void datahub_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    /* 如果重定向了输出,可以取消注释*/
//  vprintf(format, ap);
    va_end(ap);
}
```

- build工程并下载到板子中, 程序应该就能运行起来

## 帮助

请查看API文档的FAQ章节
