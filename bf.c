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
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MEMORY_SIZE 256    // Memory size in bytes
#define PROGRAM_SIZE 80000 // Max. program length in chars


// Print a hexdump-like representation of the memory block
// [start, end)
void
print_memory(unsigned char* start, unsigned char* end)
{
	int i             = 0;
	unsigned char* it = start;

	while (it < end) {

		// Formatting - extra \n after 8 lines
		if (i % 128 == 0 && i > 0) {
			putchar('\n');
			if (i % 256 == 0) {
				putchar('\n');
			}
		}

		// Print memory positions at the start of each line
		if (i % 16 == 0) {
			printf("\n0x%04x:  ", i);
		}

		// Print 2 bytes of memory
		printf("%02x%02x ", *(it), *(it + 1));

		i += 2;
		it += 2;
	}

	printf("\n");
}

// Run the program with the given memory range
void
interpret(char* const program_start, char* const program_end,
          unsigned char* const memory_start, unsigned char* const memory_end)
{
	char* instruction_ptr = program_start;
	unsigned char* data_ptr        = memory_start;

	while (instruction_ptr < program_end) {
		switch (*instruction_ptr) {
			// Simple instructions
			case '>':
				data_ptr++;
				break;
			case '<':
				data_ptr--;
				break;
			case '+':
				(*data_ptr)++;
				break;
			case '-':
				(*data_ptr)--;
				break;

			// I/O instructions
			case '.':
				putchar(*(data_ptr));
				break;
			case ',':
				printf("\nEnter char to save in cell %ld: ",
				       (data_ptr - memory_start));
				*(data_ptr) = getchar();
				// scanf("%c\n", memory + ptr);
				break;

			// Loop/branch instructions
			case '[':
				if (*(data_ptr) == 0) {
					// Jump forward to matching ]

					int bracket_counter = 1;

					while (bracket_counter) {
						instruction_ptr++;

						if (instruction_ptr >= program_end) {
							goto out_of_program;
						}

						if (*instruction_ptr == '[') {
							bracket_counter++;
						}

						if (*instruction_ptr == ']') {
							bracket_counter--;
						}
					}
					instruction_ptr--;
				}
				break;

			case ']':
				// Jump back to matching [
				if (*(data_ptr) != 0) {

					int bracket_counter = 1;

					while (bracket_counter) {

						if (instruction_ptr < program_start) {
							goto out_of_program;
						}

						instruction_ptr--;

						if (*instruction_ptr == ']') {
							bracket_counter++;
						}

						if (*instruction_ptr == '[') {
							bracket_counter--;
						}
					}
				}
				break;

			case 'p':
				print_memory(memory_start, memory_end);
				break;
			out_of_program:
				fprintf(stderr, "Invalid data pointer address\n");
				exit(1);
		}

		instruction_ptr++;
	}
}

int
main(int argc, char** argv)
{
	char* program = calloc(PROGRAM_SIZE, sizeof(char));

	// Parse command line options
	int opt;
	int opt_print_memory;
	int opt_print_time;

	while ((opt = getopt(argc, argv, "mt")) != -1) {
		switch (opt) {
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

	size_t program_length;

	// Program input
	if (argc) {
		// If the user has specified a file, read it
		FILE* input_file = fopen(argv[0], "r");
		if (input_file == NULL) {
			printf("The file \"%s\" could not be opened. Does it exist?\n",
			       argv[0]);
			return (1);
		}
		program_length = fread(program, 1, PROGRAM_SIZE, input_file);
	}
	else {
		// If no argument was passed, ask the user to enter some code
		fread(program, 1, PROGRAM_SIZE, stdin);
	}

	unsigned char* memory = calloc(MEMORY_SIZE, sizeof(char));

	// Start time measurement
	struct timeval t1, t2;
	if (opt_print_time) {
		gettimeofday(&t1, NULL);
	}

	// Do the magic
	interpret(program, program + program_length, memory, memory + MEMORY_SIZE);

	if (opt_print_time) {
		gettimeofday(&t2, NULL);

		int usec    = t2.tv_usec - t1.tv_usec;
		int sec     = t2.tv_sec - t1.tv_sec;
		float total = sec + ((float) usec) / 10E6;

		printf("\nElapsed time: %f seconds\n", total);
	}

	// For debugging
	if (opt_print_memory) {
		print_memory(memory, memory + MEMORY_SIZE);
	}

	free(memory);
	free(program);

	return 0;
}
