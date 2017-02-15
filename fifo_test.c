/*
 * =====================================================================================
 *
 *       Filename:  fifo_test.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月14日 10时07分25秒
 *
 *         Author:  wangzhiqiang (), wangzqbj@inspur.com
 *
 *    Description:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "kfifo.h"

struct message{
	int id;
	int type;
};

static DECLARE_KFIFO(fifo,struct message,128);

int main()
{
	INIT_KFIFO(fifo);	
	int buffer[] = {
		1,2,
		3,4,
		5,6,
	};

	__kfifo_in(&fifo.kfifo,buffer,sizeof(buffer));

    struct message m1;
	kfifo_get(&fifo,&m1);
	printf("m1.id=%d,m1.type=%d\n",m1.id,m1.type);
	kfifo_get(&fifo,&m1);
	printf("m1.id=%d,m1.type=%d\n",m1.id,m1.type);
	kfifo_get(&fifo,&m1);
	printf("m1.id=%d,m1.type=%d\n",m1.id,m1.type);

	return 0;
}
