# Introduction

This project provides a demo application written by Dasudian DataHub C SDK for Embedded device.

This document introduces the structure of current directory, how to compile and run demo and Q&A for help. If you want to know the details of APIs, please read file *docs/API.md* or header file *lib/DataHubClient.h*.

## Directory Introduction

There are 3 directories: *docs*, *lib*, *ucosII*.

### docs directory

*API.md* introduces APIs. You can also read file *lib/DataHubClient.h* instead.

### lib directory

*dasudian-datahub-vx.x.lib* is the static library which contains all the APIs.

*DataHubClient.h* is the header file of general APIs beginning with "datahub_".

*DatahubNetwork.h* defines APIs using to connect the internet/Internet, which will be implemented by user.

*ignore other unimportant files*

### ucosII directory

This directory contains all the files to build demo application. It is based on LWIP-1.4.1, ucosII. It is developed on STM32F407ZG.

*ucosII/CORE* contains core files to start up STM32F407ZG.

*ucosII/FWLIB* contains source and header files of STM32F407ZG's firmware.

*ucosII/HARDWARE* contains drivers of hardwares, such as key, LED and ethernet netcard.

*ucosII/LWIP* contains source and header files of LWIP.

**ucosII/LWIP_APP/lwip_comm/** contains drivers of ethernet.

*ucosII/LWIP_APP/datahub_demo/* contains the most important codes of this demo, which calls DATAHUB APIs to send data.

*ucosII/MALLOC* implements a memory pool.

*ucosII/SYSTEM* contains an implementation of delay, initialization of system clock and initialization of serial port.

*ucosII/UCOSII* contains all the codes of OS ucosII.

*ucosII/USER/* contains the function main() in main.c.

## How to compile and run demo applications

Compiler: keil 5.22

Environment: windows 7 x64 Home Basic

In-circuit debugger and programmer: ST-Link

Board: STM32F4 EXPLORER from ALIENTEK

use keil to open ucosII/USER/Template.uvprojx, build, and then download. It should work, otherwise see Q&A for help.

## Q&A
