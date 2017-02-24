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

*timer* directory contains implementations of timer for a few OSs. You can use them or implement timer yourself.

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

## Q&A

Q: Program stops at 'BKPT 0xAB' when debugging. Besides, when you check the calling stack, you see function _sys_open(), freopen(), _initio() and main().

A: This could happen when using version 1.x.x of SDK, which uses printf() to print out debug information. This is a bug of SDK. There are two way to solve this problem:1) update SDK to version 2.0.0. 2) rewrite function fputc() then click "Use MicroLib"(Options for Target --> Target --> Code Generation --> click "Use MicroLib")

Q:

...

Error: L6367E: datahubclient.o attributes are not compatible with the provided attributes . Tag_CPU_arch = ARM v7E-M (=13)

...

A: This is because the arch of your CPU doesn't match the library. Please find the appropriate library of your CPU in [lib](./lib). If you don't find what you need, please contact us(support@dasudian.com).

## Support

If there are other problems or advice, Please send a email to support@dasudian.com.
