#include <libproc.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	int n = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);	
	printf("procs: %d\n",n);
	return 0;
}
