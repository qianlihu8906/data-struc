/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月10日 11时18分13秒
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
#include "list.h"
#include "student.h"

static struct list_head *S = NULL;

struct student{
	int id;
	char *name;
	listen_func_t listen;
	struct list_head list;
};

int student_init()
{
	struct list_head *h = malloc(sizeof(*h));
	assert(h != NULL);
	INIT_LIST_HEAD(h);
	S = h;
	return 0;
}


struct student *student_create(int id,const char *name,listen_func_t listen)
{
	struct student *s = malloc(sizeof(*s));
	struct list_head *lh = S;
	assert(s != NULL);

	s->id = id;
	s->name = strdup(name);
	s->listen = listen;
	list_add(&s->list,lh);

	return NULL;
}

void student_release(struct student *s)
{
	assert(s != NULL);
	list_del(&s->list);
	free(s->name);
	free(s);
}

struct student *student_find_by_id(int id)
{
	struct list_head *pos,*head;
	head = S;
	list_for_each(pos,head){
		struct student *s = list_entry(pos,struct student,list);
		if(s->id == id)
			return s;
	}

	return NULL;
}

const char *student_get_name(struct student *s)
{
	assert(s != NULL);
	return s->name;
}

int student_get_id(struct student *s)
{
	assert(s != NULL);
	return s->id;
}

int student_listen(struct student *s,const char *cls)
{
	assert(s != NULL);
	if(s->listen)
		return s->listen(s,cls);
	else
		return -1;
	
}

void student_debug()
{
	struct list_head *pos,*head;
	head = S;
	list_for_each(pos,head){
		struct student *s = list_entry(pos,struct student,list);
		printf("student(%s,%d)\n",s->name,s->id);
	}
}
