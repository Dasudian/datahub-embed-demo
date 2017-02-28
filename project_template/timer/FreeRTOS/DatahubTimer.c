#include "FreeRTOS.h"
#include "DatahubTimer.h"

void InitTimer(Timer* timer)
{
    timer->end = 0;
}

// 时间是否过期
char expired(Timer* timer)
{
    unsigned long now;
    int res;

    now = xTaskGetTickCount();

    res = timer->end - now;
    return res <= 0;
}

// 将定时器从当前时间往后加timeout毫秒
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

// 将定时器从当前时间往后加timeout秒
void countdown(Timer* timer, unsigned int timeout)
{
    countdown_ms(timer, timeout * 1000);
}

// 返回还有多少ms超时
int left_ms(Timer* timer)
{
    unsigned int now;
    int res;

    now = xTaskGetTickCount();
    res = timer->end - now;

    return (res < 0) ? 0 : res * (1000 / configTICK_RATE_HZ);
}
