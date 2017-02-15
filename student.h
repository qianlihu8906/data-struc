/*
 * =====================================================================================
 *
 *       Filename:  student_list.h
 *
 *
 *        Version:  1.0
 *        Created:  2017年02月13日 10时37分09秒
 *
 *         Author:  wangzhiqiang (), wangzqbj@inspur.com
 * 
 *    Description:  
 *
 * =====================================================================================
 */

#ifndef STUDENT_H__
#define STUDENT_H__ 

/* basic define */
struct student;
typedef int (*listen_func_t)(struct student *s,const char *cls);

/* model init */
int student_init();

/* constructor */
struct student *student_create(int id,const char *name,listen_func_t listen);
void student_release(struct student *s);

/*property*/
const char *student_get_name(struct student *s);
//int student_set_name(struct student *s,const char *name);
int student_get_id(struct student *s);
//int student_set_id(struct student *s,int id);

/* method */
int student_listen(struct student *s,const char *cls);

/* algorithm */
struct student *student_find_by_id(int id);


/* utils */
void student_debug();

#endif
