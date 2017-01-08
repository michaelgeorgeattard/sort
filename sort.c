#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* read(char *filename)
{
	FILE* f;

	f = fopen(filename, "rb");

	fseek(f, 0, SEEK_END);

	long fsize = ftell(f);

	fseek(f, 0, SEEK_SET);

	char *buffer = malloc(fsize + 1);

	size_t size_read = fread(buffer, fsize, 1, f);

	fclose(f);
	
	if (size_read == 0) 
	{
		printf("%s", "Error reading input");
	}

	buffer[fsize] = 0;
	
	return buffer;
}

unsigned int parse(char *str, int **buffer, unsigned int buffer_size, char delimiter)
{	
	int i = -1;
	
	unsigned int number_of_tokens = 0;
	
	int previous_delimiter_idx = -1;

	long len = strlen(str);
	
	while (i < len) {
		if (str[++i] == delimiter) {		
			if (buffer_size <= number_of_tokens) // buffer too small, we need to increase its size
			{			
				*buffer = realloc(*buffer, (buffer_size *= 2) * sizeof((*buffer)[0])); // double buffer size
				
				if (*buffer == NULL)
					printf("Error expanding buffer");
			}
			
			int size_of_token = i - previous_delimiter_idx;
			
			char token[size_of_token]; // create temporary token variable
			
			memcpy(token, &str[previous_delimiter_idx+1], size_of_token-1); // copy token data into variable
			
			token[size_of_token-1] = 0;
			
			previous_delimiter_idx = i;
			
			(*buffer)[number_of_tokens++] = atoi(token);
		}
	}
	
	return number_of_tokens;
}

void bubble_sort(int *buffer, unsigned int number_of_tokens)
{
	int i;
	
	int no_operations;
			
	do {
		no_operations = 1;
		
		for (i = 0; i < number_of_tokens - 1; ++i)
		{			
			if (buffer[i] > buffer[i+1])
			{
				int temp = buffer[i];
				
				buffer[i] = buffer[i+1];
				buffer[i+1] = temp;
				
				no_operations = 0;
			}
		}
	} while (!no_operations);
}

void optimized_bubble_sort(int *buffer, unsigned int number_of_tokens)
{
	int i;
	
	int no_operations;
	
	int rounds = 0;
			
	do {
		no_operations = 1;
		
		for (i = 0; i < number_of_tokens - 1 - rounds; ++i)
		{			
			if (buffer[i] > buffer[i+1])
			{
				int temp = buffer[i];
				
				buffer[i] = buffer[i+1];
				buffer[i+1] = temp;
				
				no_operations = 0;
			}
		}
		
		++rounds;
	} while (!no_operations);
}

void print(int *buffer, unsigned int number_of_tokens)
{
	int i;
	for (i = 0; i < number_of_tokens; ++i)
	{
		printf("%d\n", buffer[i]);
	}
}

int main()
{
	char *str = read("sample.txt");
	
	unsigned int buffer_size = 1;
	
	int *buffer = malloc(buffer_size * sizeof(buffer)); // allocate initial output buffer
	
	unsigned int number_of_tokens = parse(str, &buffer, buffer_size, 0x0a);
	
	int *work_space = malloc(sizeof(work_space)*number_of_tokens);
	
	memcpy(work_space, buffer, number_of_tokens); // clone buffer
	
	optimized_bubble_sort(buffer, number_of_tokens);
	
	//print(buffer, number_of_tokens);
	
	free(work_space);
	
	free(buffer);
	
	return 0;
}