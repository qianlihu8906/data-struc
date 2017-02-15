# linux内核数据结构应用实例
这里主要介绍内核中数据结构的应用和基本编程思路，会详细介绍 list hashlist kfifo rbtree的使用，slab分配器和idr的设计思路
##list
###算法复杂度
添加和删除操作复杂度为O(1),查找复杂度为O(n)
###实现
内核中链表的实现非常经典，不再做过多介绍，详细了解可以参考[深入分析linux内核链表](https://www.ibm.com/developerworks/cn/linux/kernel/l-chain/).移植时需要从include/linux/kernel.h中拷贝container_of的实现（证明其基础性），从include/linux/types.h中拷贝struct list_head的声明（证明了list在内核中使用非常普遍），从include/linux/poison.h中拷贝LIST_POISION1、LIST_POISION2的宏定义(需要稍微修改)。
###接口API
```c
static inline void list_add_tail(struct list_head *new, struct list_head *head);
static inline void list_del(struct list_head *entry);
list_for_each(pos, head);
list_for_each_entry(pos, head, member);
list_entry(ptr, type, member);
```
我们再看另一种链表的典型实现，代码来自[redis](https://github.com/antirez/redis)。
```c
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

typedef struct listIter {
    listNode *next;
    int direction;
} listIter;

typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    unsigned long len;
} list;

list *listInsertNode(list *list, listNode *old_node, void *value, int after);
void listDelNode(list *list, listNode *node);
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);
```
这两种实现方案，在我个人看来主要优缺点如下：

1. redis方案，可以实现动态类型扩展，而内核方案只能是编译期确定节点类型
2. redis方案需要做链表单独管理单元，而内核是侵入式设计，节点即包含在结构中，删除操作非常方便。
3. redis代码为BSD license。

###典型应用
链表的应用非常广泛，绝大多数数据结构可以靠链表进行组织。
##hlist
###算法复杂度
添加、删除、查找的复杂度都为O(1)。
###实现
内核中哈希链表在list.h中实现，为了节省空间，单独设计了表头，表头中只有指向头结点没有指向尾节点的指针，能在海量的HASH表存储中减少一半的空间消耗，移植时，需要从include/linux/types.h中拷贝struct hlist_head的声明。
###接口API
```c
#define HLIST_HEAD_INIT { .first = NULL }
#define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h);
static inline void hlist_del(struct hlist_node *n);
```
需要注意的是，链表头和节点数据类型并不一致。
###典型应用
```c
//define
struct node{
  int msg;
  struct hlist_node node;
}
//init
struct hlist_head hash[HASH_SIZE];
//add
struct node *node = node_create();
struct hlist_head *h = &hash[hash%HASH_SIZE];
hlist_add_head(node->node,h);  
```
##rbtree
###算法复杂度
rbtree的添加、删除、查找复杂度均为O(logn)。
###实现
需要拷贝出 include/linux/rbtree.h include/linux/rbtree_augmented.h lib/rbtree.c 文件。删除掉rbtree.c包含的头文件，增加stddef头文件，去掉EXPORT_SYMBOLE。[详解Linux内核红黑树算法的实现](http://blog.csdn.net/npy_lp/article/details/7420689)和[手把手实现红黑树](http://blog.csdn.net/chen19870707/article/details/39585277)两篇文章详细说明了其实现
###接口API
```c
#define RB_ROOT	(struct rb_root) { NULL, }
extern void rb_insert_color(struct rb_node *, struct rb_root *);
extern void rb_erase(struct rb_node *, struct rb_root *);
static inline void rb_link_node(struct rb_node * node, struct rb_node * parent,
				struct rb_node ** rb_link);
```
###典型应用
```c
//define
struct node{
  int msg;
  struct rb_node node;
};
//init
struct rb_root root = RB_ROOT;
//insert(add)
static int insert(struct node *node){
  struct rb_node **new = &(root.rb_node),*parent=NULL;
  while(*new){
    struct node *this = container_of(*new,struct node,node);
    parent = *new;
    if(node->msg > this->msg)
      new = &((*new)->rb_left);
    else if(node->msg) < this->msg)
      new = &((*new)->rb_right);
    else
      return 0;
  }
  rb_link_node(&node->node,parent,new);
  rb_insert_color($node->node,&root);
}
//search
...
```

##三种结构性能对比
###数据结构的选择原则
- 遍历操作为主时，优先考虑链表；（没有数据结构能提供比线性算法复杂度更好的算法去遍历元素）
- 排除性能因素，当需要相对较少数据项时，优先考虑链表；
- 当需要与其它选择链表的代码交互时，优先考虑链表；
- 需要大小不明的数据集合，优先选择链表；
- 代码架构复合"生产者/消费者"模式，优先选择队列；
- 当需要一个定长的缓冲，选择队列；
- 如果需要映射一个UID到一个对象，选择映射；
- 如果需要存储大量数据，并且快速检索，选择红黑树；

###发现另一个性能优化点
性能优化需要谨慎进行，代码结构良好，数据结构符合生产环境的情况下，可以优化内存分配。
####slab分配器
#####实现
并没有移植内核中的slab分配器，只是考虑了其实现思想，实现了一个最简版本。
#####性能对比
参考代码 slab.c

###封装性
c语言可以实现面向对象编程范式，在student.h头文件中，只是做了struct student的extern声明，并没有其定义，这样的话，其引用者，student_test.c便不能看到其内部定义，这样便实现了类的私有化。很多C语言实现利用了这种技巧。但是这样也有一个不足之处，我们的sudent_create函数返回值仍然是一个指针，很难保证引用者不利用其做加减运算，进而破坏我们的模块代码。为了解决这个问题，引入了id到指针的映射结构，linux中为idr，但是idr的实现主要需要考虑并发，这里我们参考其实现思路，实现了另外一个线程安全的用户空间的idr。代码来自于[云风的blog](http://blog.codingnow.com/2015/04/handlemap.html)。  
####对比
*student.h*
```c
struct student;
struct student *student_create();
struct student *studnet_find_by_id(id);
```
*student_handlemap.h(idr)*
```c
typdef unsigned int handleid;
handleid student_create();
handleid student_find_by_id(id);
```
idr结构同时维护了对象的引用计数，方便做内存管理。
###另外两大特性
面向对象的继承性，多态性，在kfifo结构中有较好的体现。
```c
struct __kfifo {
	unsigned int	in;
	unsigned int	out;
	unsigned int	mask;
	unsigned int	esize;
	void		*data;
};

#define __STRUCT_KFIFO_COMMON(datatype, recsize, ptrtype) \
	union { \
		struct __kfifo	kfifo; \
		datatype	*type; \
		char		(*rectype)[recsize]; \
		ptrtype		*ptr; \
		const ptrtype	*ptr_const; \
	}

#define __STRUCT_KFIFO(type, size, recsize, ptrtype) \
{ \
	__STRUCT_KFIFO_COMMON(type, recsize, ptrtype); \
	type		buf[((size < 2) || (size & (size - 1))) ? -1 : size]; \
}

#define STRUCT_KFIFO(type, size) \
	struct __STRUCT_KFIFO(type, size, 0, type)

#define __STRUCT_KFIFO_PTR(type, recsize, ptrtype) \
{ \
	__STRUCT_KFIFO_COMMON(type, recsize, ptrtype); \
	type		buf[0]; \
}

#define STRUCT_KFIFO_PTR(type) \
	struct __STRUCT_KFIFO_PTR(type, 0, type)
```
利用宏实现了继承关系。
```c
#define	kfifo_put(fifo, val) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	typeof((val) + 1) __val = (val); \
	unsigned int __ret; \
	const size_t __recsize = sizeof(*__tmp->rectype); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	if (0) { \
		typeof(__tmp->ptr_const) __dummy __attribute__ ((unused)); \
		__dummy = (typeof(__val))NULL; \
	} \
	if (__recsize) \
		__ret = __kfifo_in_r(__kfifo, __val, sizeof(*__val), \
			__recsize); \
	else { \
		__ret = !kfifo_is_full(__tmp); \
		if (__ret) { \
			(__is_kfifo_ptr(__tmp) ? \
			((typeof(__tmp->type))__kfifo->data) : \
			(__tmp->buf) \
			)[__kfifo->in & __tmp->kfifo.mask] = \
				*(typeof(__tmp->type))__val; \
			smp_wmb(); \
			__kfifo->in++; \
		} \
	} \
	__ret; \
})
```
利用typeof和unin结构实现了多态，而没有浪费多余的内存空间。
####kfifo典型应用
参考代码 fifo_test.c
