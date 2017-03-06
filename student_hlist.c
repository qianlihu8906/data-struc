/*
 * =====================================================================================
 *
 *       Filename:  student_hlist.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月13日 14时13分22秒
 *
 *         Author:  wangzhiqiang (), wangzqbj@inspur.com
 *
 *    Description:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "student.h"

#define HASH_SIZE	10
static struct hlist_head *H = NULL;

struct student{
	int id;
	char *name;
	listen_func_t listen;
	struct hlist_node node;
};


int student_init()
{
	struct hlist_head *h = malloc(sizeof(*h) * HASH_SIZE);
	assert(h != NULL);
	int i;
	for(i=0;i<HASH_SIZE;i++)
		INIT_HLIST_HEAD(&h[i]);
	H = h;
	return 0;
}

struct student *student_create(int id,const char *name,listen_func_t listen)
{
	struct student *s = malloc(sizeof(*s));	
	assert(s != NULL);

	s->id = id;
	s->name = strdup(name);
	s->listen = listen;

	struct hlist_head *h = &H[id%HASH_SIZE];
	hlist_add_head(&s->node,h);

	return s;
}

void student_release(struct student *s)
{
	assert(s != NULL);
	hlist_del(&s->node);
	free(s->name);
	free(s);

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
struct student *student_find_by_id(int id)
{
	struct hlist_head *h = &H[id%HASH_SIZE];
	struct hlist_node *pos;

	hlist_for_each(pos,h){
		struct student *s = hlist_entry(pos,struct student,node);
		if(s->id == id)
			return s;
	}
	return NULL;
}

void student_debug()
{

}
