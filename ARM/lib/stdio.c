#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

uint32_t putstr(const char* s)
{
	uint32_t c = 0;  // 글자 수를 세기위한 변수
	while(*s)
	{
		Hal_uart_put_char(*s++);
		c++;
	}
	return c;
}
