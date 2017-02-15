/*
 * =====================================================================================
 *
 *       Filename:  student_list.h
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月13日 11时20分05秒
 *
 *         Author:  wangzhiqiang (), wangzqbj@inspur.com
 * 
 *    Description:  
 *
 * =====================================================================================
 */
#ifndef STUDENT_LIST_H__
#define STUDENT_LIST_H__

#include "student.h"
#include "list.h"

struct student{
	int id;
	char *name;
	listen_func_t listen;
	struct list_head list;
};

#endif
