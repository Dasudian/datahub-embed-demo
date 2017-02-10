#ifndef DATAHUB_TIMER_H
#define DATAHUB_TIMER_H

typedef struct Timer {
    unsigned int end_time;
}Timer;

void InitTimer(Timer* timer);
char expired(Timer* timer);
void countdown_ms(Timer* timer, unsigned int timeout);
void countdown(Timer* timer, unsigned int timeout);
int left_ms(Timer* timer);

#endif
