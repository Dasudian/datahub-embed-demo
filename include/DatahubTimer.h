#ifndef DATAHUB_TIMER_H
#define DATAHUB_TIMER_H

/**
 * @brief 描述定时器的结构体
 *
 */
typedef struct Timer {
    /** 用于存储定时器的超时时刻 */
    unsigned long end;
}Timer;

/**
 * @brief 初始化定时器
 *
 * @param timer 指向待初始化的Timer结构体
 * @return 无返回值
 */
void InitTimer(Timer* timer);

/**
 * @brief 判断定时器是否超时
 *
 * @param timer 定时器指针
 * @return 1 表示超时, 0 表示未超时
 *
 */
char expired(Timer* timer);

/**
 * @brief 增加定时器的时间
 *
 * @param timer 指向Timer结构体
 * @param timeout 增加的时间, 单位为毫秒
 * @return 无返回值
 */
void countdown_ms(Timer* timer, unsigned int timeout);

/**
 * @brief 增加定时器的时间
 *
 * @param timer 指向Timer结构体
 * @param timeout 增加的时间, 单位为秒
 * @return 无返回值
 */
void countdown(Timer* timer, unsigned int timeout);

/**
 * @brief 定时器从当前时间开始，剩余多少时间超时
 *
 * @param timer 指向Timer结构体
 * @return 剩余的时间，单位为毫秒
 */
int left_ms(Timer* timer);

#endif
