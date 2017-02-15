/*
 * =====================================================================================
 *
 *       Filename:  slab.c
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月14日 14时20分19秒
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
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include "list.h"

struct slab {
	struct list_head freelist;
	int used;
};

struct student{
	int id;
	char name[16];
	int age;
	char address[16];

	int english;
	int math;
	
	//TODO
};

static struct student *student_alloc(struct slab *slab)
{
	struct list_head *h = &slab->freelist;
	if(list_empty(h))
			return NULL;
	struct list_head *tmp = h->next;
	list_del(tmp);
	slab->used++;
	return (struct student *)tmp;
}

static void student_free(struct slab *slab,struct student *s)
{
	struct list_head *lh = (struct list_head *)s;
	list_add(lh,&slab->freelist);
	slab->used--;
}

static struct slab *slab_alloc(int pagesize,int nodesize)
{
	struct slab *slab = mmap(NULL,pagesize,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	INIT_LIST_HEAD(&slab->freelist);
	slab->used = 0;

	int num = (pagesize-sizeof(struct slab))/nodesize;  //84
	void *start = slab + 1;
	int i;
	for(i=0;i<num;i++){
		struct list_head *h;
		h = (struct list_head *)(start + i*nodesize);
		list_add_tail(h,&slab->freelist);
	}
	return slab;
}

struct slab *S = NULL;

int student_init()
{
	int pagesize = getpagesize();
	struct slab *slab = slab_alloc(pagesize,sizeof(struct student));
	S = slab;

	return 0;
}

struct student *student_create(int id,const char *name)
{
	struct student *s = student_alloc(S);
	assert(s != NULL);
	
	s->id = id;
	strncpy(s->name,name,sizeof(s->name));
	return s;
}

struct student *student_create_malloc(int id,const char *name)
{
	struct student *s = malloc(sizeof(*s));
	s->id = id;
	strncpy(s->name,name,sizeof(s->name));
	return s;
}
void student_release(struct student *s)
{
	assert(s != NULL);
	student_free(S,s);
}
void student_release_free(struct student *s)
{
	assert(s != NULL);
	free(s);
}

void student_debug(struct student *s)
{
	printf("student(%d,%s)\n",s->id,s->name);
}

static int MAX = 10000;

void work_thread1()
{
	int max = MAX;
	while(max--){
		int len = random()%2345;
		char *p = malloc(len);
		if(len > 0){
			p[len-1] = '1';
		}
		usleep(random()%10);
		free(p);
	}
}

void work_thread2()
{
	struct student *student[80];
	int max = MAX;
	while(max--){
		int r = random()%80;
		int i;
		for(i=0;i<r;i++){
			student[i] = student_create_malloc(i,"john");
			usleep(random()%5);
		}
		for(i=0;i<r;i++){
			student_release_free(student[i]);
		}
	}
}

void work_thread3()
{
	struct student *student[80];
	int max = MAX;
	student_init();
	while(max--){
		int r = random()%80;
		int i;
		for(i=0;i<r;i++){
			student[i] = student_create(i,"john");
			usleep(random()%5);
		}
		for(i=0;i<r;i++){
			student_release(student[i]);
		}
	}
}

int main(int argc,char **argv)
{
	if(argc != 2){
		printf("Usage %s max\n",argv[0]);
		exit(-1);
	}

	MAX = atoi(argv[1]);

	pthread_t pid1,pid2,pid3;

	pthread_create(&pid1,NULL,(void*)work_thread1,NULL);
	pthread_create(&pid2,NULL,(void*)work_thread2,NULL);
	pthread_create(&pid3,NULL,(void*)work_thread3,NULL);

	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	pthread_join(pid3,NULL);

#if 0
	student_init();

	struct student *john = student_create(1,"john");
	struct student *jim = student_create(2,"jim");
	struct student *lily = student_create(3,"lily");
	student_debug(john);
	student_debug(jim);
	student_debug(lily);

	printf("list %d\n",S->used);
	student_release(jim);
	printf("list %d\n",S->used);
#endif

	return 0;
}
