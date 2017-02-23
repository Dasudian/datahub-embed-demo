#include "FreeRTOS.h"
#include "DatahubTimer.h"
#include "datahub_demo.h"

void InitTimer(Timer* timer)
{
    timer->end = 0;
}

// ʱ���Ƿ����
char expired(Timer* timer)
{
    unsigned long now;
    int res;

    now = xTaskGetTickCount();

    res = timer->end - now;
    return res <= 0;
}

// ����ʱ���ӵ�ǰʱ�������timeout����
void countdown_ms(Timer* timer, unsigned int timeout)
{
    unsigned int now;
    unsigned int count;

    count = (timeout * configTICK_RATE_HZ) / 1000;

    if (count == 0) {
        count = 1;
    }

    now = xTaskGetTickCount();
    timer->end = now + count;

}

// ����ʱ���ӵ�ǰʱ�������timeout��
void countdown(Timer* timer, unsigned int timeout)
{
    countdown_ms(timer, timeout * 1000);
}

// ���ػ��ж���ms��ʱ
int left_ms(Timer* timer)
{
    unsigned int now;
    int res;

    now = xTaskGetTickCount();
    res = timer->end - now;

    return (res < 0) ? 0 : res * (1000 / configTICK_RATE_HZ);
}
