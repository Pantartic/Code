#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#define MAX_PROCESSES 200
#define MAX_SIZE 200

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
		printf("Process doesn't exist.\n");
		return;
	}
	TerminateProcess(h,0);
}

void psprint() {
	HANDLE h;
	DWORD returned,pids[500];
	int i=0;
	LPSTR buffer = (LPSTR)calloc(MAX_PROCESSES,sizeof(CHAR));

	
	if(!EnumProcesses(pids,sizeof(pids),&returned))
	{
		printf("ERROR, can't show processes list.");
		return;
	}
	printf("PID\tImage Name\n");
	for(i=0; i<returned/sizeof(DWORD); i++) 
	{
		memset(buffer,0, sizeof buffer);	//	buffer now points at a different address
		h = OpenProcess(PROCESS_QUERY_INFORMATION,TRUE,pids[i]);
		GetProcessImageFileName(h,buffer,MAX_PROCESSES);
		
		if(*buffer==0) continue;
		printf("%d \t%s\n", pids[i], strrchr(buffer,92)+1);
	}
	free(buffer);
}
void help() 
{
	printf("Available Commands: \nSUB , MUL , ADD , PRINT , PSKILL , PSPRINT , QUIT\n");
}

int checkcommand(char *command, int *a, int *b) {

	if(strcmp(command,"SUB")==0) {  scanf("%d, %d", a, b); last+=sub(a,b); return 0; }
	else if(strcmp(command,"MUL")==0) {  scanf("%d, %d", a, b); last+=mul(a,b); return 0; }
	else if(strcmp(command,"ADD")==0) {  scanf("%d, %d", a, b); last+=add(a,b); return 0; }
	else if(strcmp(command,"PRINT")==0) { print(); return 0; }
	else if(strcmp(command,"PSKILL")==0) { scanf("%d", a); pskill(*a); return 0; }
	else if(strcmp(command,"PSPRINT")==0) { psprint(); return 0; }
	else if(strcmp(command,"HELP")==0 || strcmp(command,"?")==0) { help(); return 0; }
	else if(strcmp(command,"QUIT")==0) return 1;
	else if(strcmp(command,"DEADLY")==0) {printf("KAKI\n"); return 0; }
	else { printf("%s: Invalid Command.\n", command); return 0; }
}


int main(void) {
	
	int a=0,b=0,check=0,i=0;
	char command[100];
	system("COLOR C");
	printf("\t\t\tScripter By Peleg\n\n");

	while(check==0) {
		fflush(stdin);
		printf("> ");
		scanf("%s",command);
		check = checkcommand(&command[0],&a,&b);
	}
	
	return 0;
}