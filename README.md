# Introduction

This project provides a demo application written by Dasudian DataHub C SDK for Embedded device.

This document introduces the structure of current directory, how to run this demo, how to develop your application. If you want to know the details of APIs, please read file [API.md](./docs/API.md).

## Directory Introduction

There are 4 directories: docs, lib, project_template, ucosII.

### docs directory

[API.md](./docs/API.md) introduces APIs, including network APIs.

### lib directory

*dasudian-datahub-STM32Fx-vx.x.x.lib* is the static library of Datahub C SDK.

*DataHubClient.h* defines DataHub C APIs beginning with "datahub_".

*DatahubNetwork.h* defines network APIs used to connect the internet/Internet, which should be implemented by users.

*DatahubTimer.h* defines timer APIs used to make sure whether it's timeout when connecting/disconnecting to server or writing/receive data to/from network, which should be implemented by users.

*ignore other unimportant files or directories*

### project_template directory

This directory contains a template project users use to create their own projects quickly for different MCUs.

Enter into one of these directories starting with STM32F, and you will see:

*DATAHUB_API* directory contains all APIs, including the network and timer APIs users should implement.

*DATAHUB_LIB* directory contains the static library of Dasudian C SDK.

*pro_template.uvprojx* is the project file of this template, opened by keil.

*USER/DatahubNetwork.c* is the implementation of network APIs(empty), which are defined in [DatahubNetwork.h](./lib/DatahubNetwork.h)

*USER/DatahubTimer.c* is the implementation of timer APIs(empty), which are defined in [DatahubTimer.h](./lib/DatahubTimer.h)

*USER/main.c* contains the function main().

*ignore other unimportant files or directories*

### ucosII directory

This directory contains the codes of this demo application, created by template project. It is based on LWIP-1.4.1, ucosII and STM32F4 EXPLORER.

#### core files and directories

*DATAHUB_API* directory contains all APIs, including the network APIs user should implement.

*DATAHUB_LIB* directory contains the static library of Dasudian C SDK.

*demo.uvprojx* is the project file of this demo, which can be opened by keil.

[DatahubNetwork.c](./ucosII/USER/DatahubNetwork.c) is the implementation of network APIs.

[DatahubTimer.c](./ucosII/USER/DatahubTimer.c) is the implementation of timer APIs.

[datahub_demo.c](./ucosII/USER/datahub_demo.c) shows how to use DataHub C SDK.

[main.c](./ucosII/USER/main.c) starts this demo.

#### other files and directories

*ucosII/CORE* contains core files to start up STM32F407ZG.

*ucosII/LWIP* contains source and header files of LWIP.

*ucosII/LWIP_APP/* contains initialization of TCP/IP and DHCP.

*ucosII/MALLOC* implements a memory pool.

*ucosII/STM32F4xx_StdPeriph_Driver* contains drivers of STM32F407ZG.

*ucosII/STM32_CONF/* contains configure files to start up STM32F407ZG.

*ucosII/STM32F4x7_ETH_Driver* contains drivers of ethernet.

*ucosII/SYSTEM* contains an implementation of delay, initialization of system clock and serial port.

*ucosII/UCOSII* contains transplanted codes of OS 'uCOSII'.

## How to compile and run this demo application

Compiler: keil 5.22

Environment: windows 7 x64 Home Basic

In-circuit debugger and programmer: ST-Link

Board: STM32F4 EXPLORER from ALIENTEK

use keil to open ucosII/demo.uvprojx, build this project, and download to STM32F4 EXPLORER. It should work only on STM32F4 EXPLORER.

*If you want to transplant this demo to your board, you have to spend some time and have patience.*

## How to develop your own application

0.Compile fireware of your board successfully. It is one of the basic step.

1.Transplant uCOSII, FreeRTOS or other OSs to your board successfully. It is one of the basic step.

2.Implement APIs defined in file [DatahubNetwork.h](./lib/DatahubNetwork.h), including ConnectNetwork(), ucos_read(), ucos_write(), ucos_disconnect(). These APIs are used to connect/disconnect internet/Internet and send/reveive data. Without them, compilation will occur errors.(Demo implements these APIs in file [DatahubNetwork.c](./ucosII/USER/DatahubNetwork.c))

3.Implement APIs defined in file [DatahubTimer.h](./lib/DatahubTimer.h), including InitTimer(), expired(), countdown_ms(), countdown(), left_ms(). These APIs are used by SDK. Without them, compilation will occur errors.(Demo implements these APIs in file [DatahubTimer.c](./ucosII/USER/DatahubTimer.c))

3.Use APIs defined in file [DataHubClient.h](./lib/DataHubClient.h) to connect and send data to IoT DataHub of Dasudian.(Demo connects and sends data to IoT DataHub of Dasudian in file [datahub_demo.c](./ucosII/USER/datahub_demo.c))

4.Build your project and download it to your board, your application should work now.
