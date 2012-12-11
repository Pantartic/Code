/* Created by Peleg */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#define MAX_PROCESSES 100

static int last=0;

int add(int *a, int *b) {
	return ( (*a) + (*b) );
}

int mul(int *a, int *b) {
	return ( (*a) * (*b) );
}

int sub(int *a, int *b) 
{ 
	return ( (*b) - (*a) );
}

void print() 
{
	printf("%d\n", last);
	last=0;
}

void pskill(int pid) {
	HANDLE h;
	h = OpenProcess(PROCESS_TERMINATE,FALSE,pid);
	if(!h) {
		printf("Process doesn't exist.");
		return;
	}
	TerminateProcess(h,0);
}

void psprint() {
	HANDLE h;
	DWORD returned,pids[500];
	int i=0;
	LPSTR buffer = (LPSTR)calloc(MAX_PROCESSES,sizeof(CHAR));
	char *buffer2;

	
	if(!EnumProcesses(pids,sizeof(pids),&returned))
	{
		printf("ERROR, can't show processes list.");
		return;
	}
	printf("PID\tImage Name\n");
	for(i=0; i<returned/sizeof(DWORD); i++) 
	{
		h = OpenProcess(PROCESS_QUERY_INFORMATION,TRUE,pids[i]);
		GetProcessImageFileName(h,buffer,MAX_PROCESSES);
		buffer2 = strrchr((char*)buffer, '\'');
		printf("%d %s\n", pids[i], buffer2);
	}
	free(buffer);
	free(buffer2);
}


void checkcommand(char *command, int *a, int *b) {

	if(strcmp(command,"SUB")==0) last+=sub(a,b);
	else if(strcmp(command,"MUL")==0) last+=mul(a,b); 
	else if(strcmp(command,"ADD")==0) last+=add(a,b);
	else if(strcmp(command,"PRINT")==0) print();
	else if(strcmp(command,"PSKILL")==0) pskill(*a);
	else if(strcmp(command,"PSPRINT")==0) psprint();
	else printf("%s: Invalid Command.\n", command);

}

int main(int argc, char** argv) {
	
	int a=0,b=0,i=0;
	char command[100];
	FILE * fp;

	if(argc==1) {
		printf("Usage: %s file.pel");
		return 1;
	}

	fp = fopen(argv[1], "rt");
	if(fp==NULL) {
		printf("ERROR! file doesn't exist...");
		return 1;
	}
	
	
	while (fscanf(fp,"%s %d, %d", &command, &a, &b) !=EOF) {
		checkcommand(command,&a,&b);
	}

	getchar();
	fclose(fp);
	return 0;
}