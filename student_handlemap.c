/*
 * =====================================================================================
 *
 *       Filename:  student_handlemap.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月14日 15时50分00秒
 *
 *         Author:  wangzhiqiang (), wangzqbj@inspur.com
 *
 *    Description:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "handlemap.h"
#include "student_handlemap.h"

struct student;
struct handlemap *H = NULL;

struct student{
	handleid id;
	char *name;
	listen listen;
};

void student_init()
{
	H = handlemap_init();
	assert(H != NULL);
}

handleid student_new(const char *name,listen func)
{
	struct student *s = malloc(sizeof(*s));
	printf("student alloc s=%p\n",s);
	assert(s != NULL);

	s->id = handlemap_new(H,s);
	s->name = strdup(name);
	assert(s->name != NULL);

	s->listen = func;

	return s->id;
}

static struct student *student_grab(handleid id)
{
	return handlemap_grab(H,id);
	
}

void student_release(handleid id)
{
	struct student *s = handlemap_release(H,id);
	if(s){
		printf("student free s=%p\n",s);
		free(s->name);
		free(s);
	}
}

int student_get_name(handleid id,char *buffer,size_t size)
{
	struct student *s = student_grab(id);
	if(s == NULL)
		return -1;
	strncpy(buffer,s->name,size);	
	student_release(id);

	return 0;
}

int student_listen(handleid id,const char *class)
{
	struct student *s = student_grab(id);
	if(s == NULL)
		return -1;

	int r = s->listen(id,class);

	student_release(id);

	return r;
}

static int listen_func(handleid id,const char *class)
{
	char name[256];
	student_get_name(id,name,sizeof(name));

	printf("student(%s) listen %s\n",name,class);

	return 0;
}


int main()
{
	student_init();

	handleid id = student_new("john",listen_func);
	student_listen(id,"english");
	student_release(id);
}
