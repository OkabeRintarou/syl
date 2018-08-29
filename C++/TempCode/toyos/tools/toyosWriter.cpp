/**
*  file:toyosWriter.cpp
*  Uasge:toyosWriter.exe target_filename source_filename
*  Uses:copy the first sector from source file to target file
**/

#include <stdio.h>

int main(int argc, char * argv[])
{
	if (argc < 3){
		printf("Please input target & source file.\n");
		return (-1);
	}

	FILE * reader = 0;
	fopen_s(&reader,argv[2], "rb");
	if (reader == 0){
		printf("open file %s failed.\n", argv[2]);
		return -1;
	}

	unsigned char buffer[512] = { 0 };
	fread(buffer, sizeof(char), 512, reader);
	fclose(reader);

	FILE * writer;
	fopen_s(&writer, argv[1], "rb+");
	if (writer == 0){
		printf("open file %s failed.\n", argv[1]);
		return -1;
	}
	fseek(writer, 0, SEEK_SET);
	fwrite(buffer, sizeof(char), 512, writer);
	fclose(writer);

	printf("copy successfully.\n");
	return 0;
}
