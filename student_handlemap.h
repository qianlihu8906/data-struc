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
#include "handlemap.h"
#include "student.h"

void student_handlemap_init();
handleid student_handlemap_create(int id,const char *name,listen_func_t listen);
void student_handlemap_release(handleid id);
const char *student_handlemap_get_name(handleid id);
int student_handlemap_get_id(handleid id);
int student_handlemap_listen(handleid id,const char *cls);
handleid student_handlemap_find_by_id(int id);
#endif
