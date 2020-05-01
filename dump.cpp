#include <stdio.h>
#include <stdlib.h>

#include <string>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "usage: ./dump [-n <bytes>] filename\n");
		exit(1);
	}

	char* name = argv[1];

	size_t cols = 16;
	size_t count = SIZE_MAX;
	if(strcmp("-n", argv[1]) == 0)
	{
		if(argc < 3)
		{
			fprintf(stderr, "expected number after '-n'\n");
			exit(1);
		}

		count = std::stol(argv[2]);

		if(argc < 4)
		{
			fprintf(stderr, "expected filename\n");
			exit(1);
		}
		name = argv[3];
	}

	auto f = fopen(name, "r+");
	if(f == nullptr)
	{
		perror("failed to open file");
		exit(1);
	}

	size_t did = 0;
	while(count && !feof(f))
	{
		printf("%02x ", fgetc(f));
		count--;

		did++;
		if(did % cols == 0)
			printf("\n");
	}

	fclose(f);
}
