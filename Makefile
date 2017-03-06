
all:list_test hlist_test rbtree_test slab_test fifo_test

list_test:
	cc -Wall student_list.c student_test.c -o student_list_test -pg -std=gnu99

hlist_test:
	cc -Wall student_hlist.c student_test.c -o student_hlist_test -pg

rbtree_test:
	cc -Wall student_rbtree.c rbtree.c student_test.c -o student_rbtree_test -pg

slab_test:
	cc -Wall slab.c -o slab_test -lpthread -pg

fifo_test:
	cc -Wall kfifo.c fifo_test.c -o fifo_test

clean:
	rm student_list_test student_hlist_test student_rbtree_test slab_test fifo_test
