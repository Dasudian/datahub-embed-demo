#ifndef DATAHUB_TIMER_H
#define DATAHUB_TIMER_H

typedef struct Timer {
    /* 用于存储定时器的超时时刻 */
    unsigned long end;
}Timer;

/*
 * 描述: 初始化数据结构Timer
 * 参数:
 *     timer: 指向待初始化的Timer结构体
 * 返回值:
 *    无返回值
 *
 */
void InitTimer(Timer* timer);

/*
 * 描述: 判断定时器是否超时
 * 参数:
 *     timer: 定时器指针
 * 返回值:
 *     1 表示超时，0表示未超时
 *
 */
char expired(Timer* timer);

/*
 * 描述: 增加定时器的时间
 * 参数:
 *     timer: 指向Timer结构体
 *     timeout: 增加的时间，单位为毫秒
 * 返回值:
 *    无返回值
 *
 */
void countdown_ms(Timer* timer, unsigned int timeout);

/*
 * 描述: 增加定时器的时间
 * 参数:
 *     timer: 指向Timer结构体
 *     timeout: 增加的时间，单位为秒
 * 返回值:
 *    无返回值
 *
 */
void countdown(Timer* timer, unsigned int timeout);

/*
 * 描述: 定时器从当前时间开始，剩余多少时间超时
 * 参数:
 *     timer: 指向Timer结构体
 * 返回值:
 *    剩余的时间，单位为毫秒
 *
 */
int left_ms(Timer* timer);

#endif
