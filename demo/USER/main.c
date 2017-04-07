#include "DataHubClient.h"
#include "DatahubNetwork.h"
#include "lwip_comm.h"
#include "malloc.h"
#include "datahub_demo.h"
#include "usart.h"
#include "delay.h"
#include "ucos_ii.h"

//START任务
//任务优先级
#define START_TASK_PRIO		2
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata); 


#define DATAHUB_CLIENT_PRIO        6
#define DATAHUB_CLIENT_STK_SIZE    300
OS_STK DATAHUB_CLIENT_TASK_STK[DATAHUB_CLIENT_STK_SIZE];

INT8U create_datahub_task(void)
{
    INT8U res;
    OS_CPU_SR cpu_sr;

    OS_ENTER_CRITICAL();
    res = OSTaskCreate(data_thread,(void*)0,(OS_STK*)&DATAHUB_CLIENT_TASK_STK[DATAHUB_CLIENT_STK_SIZE-1],DATAHUB_CLIENT_PRIO);
    OS_EXIT_CRITICAL();

    return res;
}
int main()
{
		delay_init(168);       	//延时初始化
		uart_init(115200);    	//串口波特率设置
	
		printf("init memory pool\r\n");
		mymem_init(SRAMIN);  	//初始化内部内存池

        printf("init uCOS\r\n");
		OSInit(); 					//UCOS初始化
		printf("init LWIP\r\n");
		while(lwip_comm_init()); 	//lwip初始化
		printf("create task of DataHub\r\n");
		while (create_datahub_task());
		printf("create dhcp task\r\n");
		OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
		OSStart(); //开启UCOS

    return 0;
}

//start任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OS_ENTER_CRITICAL();  	//关中断
#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //创建DHCP任务
#endif
	OSTaskSuspend(OS_PRIO_SELF); //挂起start_task任务
	OS_EXIT_CRITICAL();  //开中断
}
