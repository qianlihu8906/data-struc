/*
 * =====================================================================================
 *
 *       Filename:  student_handlemap.h
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月14日 16时20分56秒
 *
 *         Author:  wangzhiqiang (), wangzqbj@inspur.com
 * 
 *    Description:  
 *
 * =====================================================================================
 */

#ifndef STUDENT_HANDLEMAP_H__
#define STUDENT_HANDLEMAP_H__
#include <stddef.h>
#include "handlemap.h"

typedef int (*listen)(handleid id,const char *class);

void student_init();
handleid student_new(const char *name,listen func);
void student_release(handleid id);
int student_get_name(handleid id,char *buffer,size_t size);
int student_listen(handleid id,const char *class);

#endif
