/*
 * Brainfuck Interpreter in C99
 * Balduin Dettling, 2015-07-15
 *
 * ~~ Usage ~~
 *
 * - compile: $ gcc -o bf bf.c
 *
 * - If you have a text file containing a bf program, specify the
 *   file as 1st command line argument. For example:
 *   $ ./bf program.bf
 *   $ ./bf programs/brainfuck/program.bf
 *
 * - If no file is specified, you will be asked to enter a program
 *   using scanf().
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define MEMORY_SIZE 1024 // Memory size in bytes
#define PROGRAM_SIZE 80000 // Max. program length in chars

// Allocate specified amount of memory
unsigned char memory[MEMORY_SIZE] = {0};

void interpret(char* program)
{
    int ptr = 0;
    int instruction_ptr = 0;
    int length = strlen(program);

    while (instruction_ptr < length)
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
                scanf(" %c", memory + ptr);
                break;

            // Loop/branch instructions
            case '[':
                if (memory[ptr] == 0)
                {
                    // Jump forward to matching ]
                    int bracket_counter = 1;
                    while (bracket_counter)
                    {
                        instruction_ptr++;
                        if (program[instruction_ptr] == '[')
                        {
                            bracket_counter++;
                        }

                        if (program[instruction_ptr] == ']')
                        {
                            bracket_counter--;
                        }
                    }
                    instruction_ptr--;
                }
                break;

            case ']':
                if (memory[ptr])
                {
                    // Jump back to matching [
                    int bracket_counter = 1;
                    while (bracket_counter)
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
                } // end if
                break;
        } // end switch

        instruction_ptr++;
    } // end while
}

void print_memory()
{
    puts("\nThe program modified memory as follows:");
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        // Formatting - extra \n after 8 lines
        if (i%128 == 0 && i > 0)
        {
            printf("\n");
        }

        // Print memory positions
        if (i%16 == 0)
        {
            printf("\n0x%04x:  ", i);
        }

        // Print a byte of memory
        printf("%02x ", memory[i]);
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    char* program = calloc(PROGRAM_SIZE, 1);

    // Program input
    if (argc > 1)
    {
        // If the user has specified a file, read it
        FILE* inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) 
        {
            printf("The file \"%s\" could not be opened. Does it exist?\n",
                argv[1]);
            return 1;
        }
        fread(program, 1, PROGRAM_SIZE, inputFile);
    }
    else if (argc == 1)
    {
        // If no argument was passed, ask the user to enter some code
        printf("Enter a BF program: ");
        scanf("%s", program);
    }



    // Start time measurement
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    // Do the magic
    interpret(program);

    // Complete time measurement
    gettimeofday(&t2, NULL);

    // For debugging
    // print_memory();
    if (argc == 3)
    {
        if (strcmp(argv[2], "m") == 0)
        {
            print_memory();
        }
    }

    int usec = t2.tv_usec - t1.tv_usec;
    int sec  = t2.tv_sec - t1.tv_sec;
    float total = sec + ((float) usec) / 10E6;

    printf("\nElapsed time: %f seconds\n", total);


    return 0;
}
