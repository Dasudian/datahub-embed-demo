#include "Ucos_ii.h"
#include "DatahubTimer.h"

// ʱ���Ƿ����
char expired(Timer* timer)
{
	unsigned int now = OSTimeGet();
	int res = timer->end - now;
	//printf("ʱ���Ƿ���ڣ�expired:end(%d) - now(%d) = %d\r\n", timer->end, now, res);
	return res <= 0;
}

// ����ʱ���ӵ�ǰʱ�������timeout����
void countdown_ms(Timer* timer, unsigned int timeout)
{
	unsigned int now;
	unsigned int count = (timeout * OS_TICKS_PER_SEC) / 1000;
	if (count == 0) {
		count = 1;
	}
	now = OSTimeGet();
	timer->end = now + count;
	//printf("��ʱ�������:countdown_ms:now=%d, count=%d, end=%d\r\n", now, count, timer->end);
}

// ����ʱ���ӵ�ǰʱ�������timeout��
void countdown(Timer* timer, unsigned int timeout)
{
	unsigned int count = timeout * OS_TICKS_PER_SEC;
	timer->end = OSTimeGet() + count;
	//printf("countdown:timer->end = %d\r\n", timer->end);
}

// ���ػ��ж���ms��ʱ
int left_ms(Timer* timer)
{
	unsigned int now = OSTimeGet();
	int res = timer->end - now;
	//printf("���ж���ms��ʱ��left_ms:res = %d\r\n", res);
	return (res < 0) ? 0 : res * (1000 / OS_TICKS_PER_SEC);
}

void InitTimer(Timer* timer)
{
	timer->end = 0;
}
