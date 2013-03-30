/*
	Created by Peleg
*/

#include <Windows.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
  
	IMAGE_DOS_HEADER* image_dos_header;
	char* pebase = NULL;
	int i=0;
	HANDLE pe, pemap;
	char *buf,signature[2];

	if(argc < 2)
	{
		printf("Usage: %s %s", argv[0], argv[1]);
		return 0;
	}

	printf(":: PE Signature Checker ::\n");
	printf("by Peleg\n");

	buf = _strdup(argv[1]);
	pe = CreateFile(buf,GENERIC_READ | GENERIC_WRITE,
		0, NULL , OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL ,NULL);

	pemap = CreateFileMapping(pe, NULL, PAGE_READWRITE, 0, 0, NULL);

	image_dos_header = (IMAGE_DOS_HEADER*)MapViewOfFile(pemap,FILE_MAP_ALL_ACCESS,0,0,0);

	printf("Base Address: %p\n", image_dos_header);
	printf("Signature: ");
	pebase = (char*)image_dos_header;
	for(i=0; i<2; i++)
	{
		printf("%c", *pebase);
		signature[i] = *pebase;
		pebase++;
	}
	printf("\n");

	if(strcmp(signature,"MZ"))
	{
		printf("File's Signature is Invalid!");
	}

	else 
	{
		printf("File's Signature is OK!\n");
	}

	CloseHandle(pe);
	CloseHandle(pemap);
	free(buf);
	fflush(stdin);
	getchar();
}
