/*
 * =====================================================================================
 *
 *       Filename:  student_rbtree.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月13日 16时05分51秒
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
#include "rbtree.h"
#include "student.h"

static struct rb_root RB = RB_ROOT;

struct student {
	int id;
	char *name;
	listen_func_t listen;
	struct rb_node node;
};


int student_init()
{
	return 0;
}	

static int _insert(struct rb_root *root,struct student *s)
{
	struct rb_node **new = &(root->rb_node),*parent=NULL;

	while(*new){
		struct student *this = container_of(*new,struct student,node);
		parent = *new;
		if(s->id > this->id)
			new = &((*new)->rb_left);
		else if(s->id < this->id)
			new = &((*new)->rb_right);
		else
			return 0;
	}

	rb_link_node(&s->node,parent,new);
	rb_insert_color(&s->node,root);

	return 1;
}

struct student *student_create(int id,const char *name,listen_func_t listen)
{
	struct student *s = malloc(sizeof(*s));
	assert(s != NULL);

	s->id = id;
	s->name = strdup(name);
	s->listen = listen;
	
	_insert(&RB,s);

	return s;
}

void student_release(struct student *s)
{
	assert(s != NULL);

	rb_erase(&s->node,&RB);
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
	return -1;
}

struct student *student_find_by_id(int id)
{
	struct rb_node *node = RB.rb_node;

	while(node){
		struct student *s = container_of(node,struct student,node);
		
		if(id > s->id)
			node = node->rb_left;
		else if(id < s->id)
			node = node->rb_right;
		else
			return s;
	}
	return NULL;
}

void student_debug()
{

}
