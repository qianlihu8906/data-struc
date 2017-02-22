/*
 * =====================================================================================
 *
 *       Filename:  student_adlist.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月22日 08时39分49秒
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

#include "adlist.h"
#include "student.h"

static list *L = NULL;

struct student{
	int id;
	char *name;
	listen_func_t listen;
};

static struct student *_student_create(int id,const char *name,listen_func_t listen)
{
	struct student *s = malloc(sizeof(*s));
	assert(s != NULL);
	s->id = id;
	s->name = strdup(name);
	s->listen = listen;
	printf("student create %s,%d(%p)\n",s->name,s->id,s);
	return s;
}

static void _student_release(void *p)
{
	struct student *s = p;
	printf("student release %s,%d(%p)\n",s->name,s->id,s);
	free(s->name);
	free(s);	
}

int student_init()
{
	list *l = listCreate();
	listSetFreeMethod(l,_student_release);
	assert(l != NULL);

	L = l;
	return 0;
}


struct student *student_create(int id,const char *name,listen_func_t listen)
{
	list *l = L;
	struct student *s = _student_create(id,name,listen);
	listAddNodeHead(l,s);
	return s;
}

static struct listNode *_student_find(int (*match)(void *ptr,void *key) ,void *key)
{
	list *l = L;
	listSetMatchMethod(l,match);
	return listSearchKey(l,key);
}


struct student  *student_find(int (*match)(void *ptr,void *key) ,void *key)
{
	listNode *node = _student_find(match,key);
	if(node == NULL)
		return NULL;
	return listNodeValue(node);
}
static int match_name(void *ptr,void *key)
{
	struct student *s = ptr;
	const char *name = key;
	if(s && s->name)
		return strcmp(s->name,name) == 0;
	return 0;
}

struct student *student_find_by_name(const char *name)
{
	return student_find(match_name,(void *)name);
}

void student_release(struct student *s)
{
	list *l = L;
	listSetMatchMethod(l,NULL);
	listNode *node = listSearchKey(l,s);
	assert(node != NULL);
	listDelNode(l,node);
}

void student_release_by_name(const char *name)
{
	list *l = L;
	listNode *node = _student_find(match_name,(void *)name);
	if(node)
		listDelNode(l,node);
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
	list *l = L;
	listIter iter;
	listNode *node;

	listRewind(l,&iter);
	while( (node=listNext(&iter)) != NULL ){
		struct student *s = listNodeValue(node);
		printf("student name=%s,id=%d\n",s->name,s->id);
	}
}

#if 1
static int listen(struct student *s,const char *cls)
{
	printf("%s listen %s\n",s->name,cls);
	return 0;
}

int match_id(void *ptr,void *key)
{
	struct student *s = ptr;
	int *id = key;
	return s->id == *id;
}


int main()
{
	student_init();

	struct student *john = student_create(1,"john",listen);
	student_create(2,"jim",listen);
	student_debug();
	struct student *s = student_find_by_name("john");
	student_listen(s,"english");
	
	int id = 2;
	struct student *s1 = student_find(match_id,&id);
	student_listen(s1,"math");

	student_release(john);
	student_release_by_name("jim");
}
#endif
