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

#include "handlemap.h"
#include "student.h"

struct handlemap *H = NULL;

void student_handlemap_init()
{
	H = handlemap_init();
}

handleid student_handlemap_create(int id,const char *name,listen_func_t listen)
{
	struct student *s = student_create(id,name,listen);
	return handlemap_new(H,s);	
}
void student_handlemap_release(handleid id)
{
	struct student *s = handlemap_grab(H,id);
	if( handlemap_release(H,id) != NULL){
		student_release(s);
	}
}

const char *student_handlemap_get_name(handleid id)
{
	struct student *s = handlemap_grab(H,id);
	const char *name = student_get_name(s);
	handlemap_release(H,id);
	return name;
}
int student_handlemap_get_id(handleid id)
{
	struct student *s = handlemap_grab(H,id);
	int sid = student_get_id(s);
	handlemap_release(H,id);
	return sid;

}
int student_handlemap_listen(handleid id,const char *cls)
{
	struct student *s = handlemap_grab(H,id);
	int res = student_listen(s,cls);
	handlemap_release(H,id);
	return res;
}

//TODO  
handleid student_handlemap_find_by_id(int id)
{
	(void)id;
	return 0;
}
# if 0
int main()
{
}
#endif 
