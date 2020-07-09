#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

struct thread_info {    /* Used as argument to thread_start() */
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
    char     *argv_string;      /* From command-line argument */
};

int pthread_create_from_mach_thread(pthread_t *thread, 
	const pthread_attr_t *attr,
	void *(*start_routine)(void *), 
	void *arg);

static void * test(void *arg) {
	return NULL;
}

int main(int argc, char* argv[]){
	int num_threads;
	struct thread_info *tinfo;

	num_threads = argc - optind;
	tinfo = calloc(num_threads, sizeof(struct thread_info));
	if (tinfo == NULL)
		printf("%s\n", "null");

	pthread_create_from_mach_thread(NULL, NULL, &test, NULL);
	return 0;
}