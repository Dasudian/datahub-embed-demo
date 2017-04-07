#include "DataHubClient.h"
#include "DatahubNetwork.h"
#include "lwip_comm.h"
#include "malloc.h"
#include "datahub_demo.h"
#include "usart.h"
#include "delay.h"
#include "ucos_ii.h"

//START����
//�������ȼ�
#define START_TASK_PRIO		2
//�����ջ��С
#define START_STK_SIZE		128
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
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
		delay_init(168);       	//��ʱ��ʼ��
		uart_init(115200);    	//���ڲ���������
	
		printf("init memory pool\r\n");
		mymem_init(SRAMIN);  	//��ʼ���ڲ��ڴ��

        printf("init uCOS\r\n");
		OSInit(); 					//UCOS��ʼ��
		printf("init LWIP\r\n");
		while(lwip_comm_init()); 	//lwip��ʼ��
		printf("create task of DataHub\r\n");
		while (create_datahub_task());
		printf("create dhcp task\r\n");
		OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);
		OSStart(); //����UCOS

    return 0;
}

//start����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr;
	pdata = pdata ;
	
	OS_ENTER_CRITICAL();  	//���ж�
#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //����DHCP����
#endif
	OSTaskSuspend(OS_PRIO_SELF); //����start_task����
	OS_EXIT_CRITICAL();  //���ж�
}
