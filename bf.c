/*
 * Brainfuck Interpreter in C99
 * Balduin Dettling, 2015-07-15
 *
 * *** Usage ***
 *
 * - compile: $ gcc -o bf bf.c
 *
 * - If you have a text file containing a bf program, specify the
 *   file as 1st command line argument. For example:
 *   $ ./bf program.bf
 *   $ ./bf programs/brainfuck/program.bf
 *
 * - If no file is specified, you will be asked to enter a program
 *   into the standard input.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define MEMORY_SIZE 256 // Memory size in bytes
#define PROGRAM_SIZE 80000 // Max. program length in chars

// Allocate specified amount of memory

void interpret(char* program, char* memory)
{
	int ptr = 0;
	int instruction_ptr = 0;

	while(instruction_ptr < (int) strlen(program))
	{
		switch(program[instruction_ptr])
		{
			// Simple instructions
			case '>':
				ptr++;
				break;
			case '<':
				ptr--;
				break;
			case '+':
				memory[ptr]++;
				break;
			case '-':
				memory[ptr]--;
				break;

			// I/O instructions
			case '.':
				printf("%c", memory[ptr]);
				break;
			case ',':
				printf("\nEnter char to save in cell %d: ", ptr);
				scanf("%c\n", memory + ptr);
				break;

			// Loop/branch instructions
			case '[':
				if(memory[ptr] == 0)
				// Jump forward to matching ]
				{
					int bracket_counter = 1;
					while(bracket_counter)
					{
						instruction_ptr++;
						if(program[instruction_ptr] == '[')
						{
							bracket_counter++;
						}

						if(program[instruction_ptr] == ']')
						{
							bracket_counter--;
						}
					}
					instruction_ptr--;
				}
				break;

			case ']':
				// Jump back to matching [
				if(memory[ptr] != 0)
				{
					int bracket_counter = 1;
					while(bracket_counter)
					{
						instruction_ptr--;
						if (program[instruction_ptr] == ']')
						{
							bracket_counter++;
						}
						if (program[instruction_ptr] == '[')
						{
							bracket_counter--;
						}
					}
				}
				break;

			case 'p':
				print_memory(memory);
				break;
		}

		instruction_ptr++;
	}
}

void print_memory(char* memory)
{
	puts("\nThe program modified memory as follows:");
	for (int i = 0; i < MEMORY_SIZE; i += 2)
	{
		// Formatting - extra \n after 8 lines
		if(i%128 == 0 && i > 0)
		{
			printf("\n");
			if (i%256 == 0)
			{
				printf("\n");
			}
		}

		// Print memory positions
		if(i%16 == 0)
		{
			printf("\n0x%04x:  ", i);
		}

		// Print a byte of memory
		printf("%02x%02x ", (char) memory[i], (char) memory[i+1]);
	}
	printf("\n");
}

int main(int argc, char** argv)
{
	char* program = calloc(PROGRAM_SIZE, sizeof(char));

	// Parse command line options
	int opt;
	int opt_print_memory;
	int opt_print_time;

	while ((opt = getopt(argc, argv, "mt")) != -1)
	{
		switch(opt)
		{
			case 'm':
				opt_print_memory = 1;
				break;
			case 't':
				opt_print_time = 1;
				break;
			default:
				printf("Usage: %s [-mt] [<file>]\n", argv[0]);
				printf(" -m : Print memory contents left behind by program\n");
				printf(" -t : Print elapsed time\n");
				exit(1);
		}
	}

	// Adjust arguments
	argc -= optind;
	argv += optind;

	// Program input
	if (argc)
	{
		// If the user has specified a file, read it
		FILE* inputFile = fopen(argv[0], "r");
		if(inputFile == NULL)
		{
			printf("The file \"%s\" could not be opened. Does it exist?\n",
					argv[0]);
			return(1);
		}
		fread(program, 1, PROGRAM_SIZE, inputFile);
	}
	else
	{
		// If no argument was passed, ask the user to enter some code
		printf("Enter a BF program: ");
		scanf("%s", program);
	}

	char* memory = calloc(MEMORY_SIZE, sizeof(char));

	// Start time measurement
	struct timeval t1, t2;
	if (opt_print_time)
	{
		gettimeofday(&t1, NULL);
	}

	// Do the magic
	interpret(program, memory);

	if (opt_print_time)
	{
		gettimeofday(&t2, NULL);

		int usec = t2.tv_usec - t1.tv_usec;
		int sec  = t2.tv_sec - t1.tv_sec;
		float total = sec + ((float) usec) / 10E6;

		printf("\nElapsed time: %f seconds\n", total);
	}

	// For debugging
	if (opt_print_memory)
	{
		print_memory(memory);
	}

	free(memory);
	free(program);

	return 0;
}
