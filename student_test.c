/*
 * =====================================================================================
 *
 *       Filename:  student_test.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月14日 17时02分53秒
 *
 *         Author:  wangzhiqiang (), wangzqbj@inspur.com
 *
 *    Description:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "student.h"

static int listen(struct student *s,const char *cls)
{
	printf("sudent(%s,%d) listen %s\n",student_get_name(s),
			student_get_id(s),cls);
	return 0;
}

int main(int argc,char **argv)
{
	if(argc != 2){
		printf("Usage %s max\n",argv[0]);
		exit(-1);
	}
	int max = atoi(argv[1]);

	student_init();


	int i,id = random();
	student_create(id,"john",listen);

	for(i=0;i<max;i++){
		student_create(random(),"john",listen);
	}

	struct student *s = student_find_by_id(id);
	student_listen(s,"english");
	
	return 0;
}
