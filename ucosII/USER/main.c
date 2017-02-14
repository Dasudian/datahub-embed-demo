#include "DataHubClient.h"
#include "DatahubNetwork.h"
#include "lwip_comm.h"
#include "malloc.h"
#include "datahub_demo.h"
#include "usart.h"
#include "delay.h"

//START任务
//任务优先级
#define START_TASK_PRIO		2
//任务堆栈大小
#define START_STK_SIZE		128
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata); 

int main()
{
		delay_init(168);       	//延时初始化
		uart_init(115200);    	//串口波特率设置
	
		printf("init memory pool\n");
		mymem_init(SRAMIN);  	//初始化内部内存池
		mymem_init(SRAMEX);  	//初始化外部内存池
		mymem_init(SRAMCCM); 	//初始化CCM内存池
	
		printf("init uCOS\n");
		OSInit(); 					//UCOS初始化
		printf("init LWIP\n");
		while(lwip_comm_init()); 	//lwip初始化
		printf("create task of DataHub\n");
		while (create_datahub_task());
		printf("create dhcp task\n");
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