# BFInterpreter

Brainfuck interpreter in C

    ./bf programs/test.bf
    Lorem ipsum dolor sit amet.

There are two flags you can give:

- `-m` Print a hexadecimal representation of the memory block left behind
by the program
- `-t` Measure and print the time needed to execute the program (including
waiting for user input)

Or, if you want to enter some code quickly, not unlike any other console:

    ./bf -t
    Enter a BF program: -[------->+<]>-.-[->+++++<]>++.+++++++..+++.[--->+<]>-----.---[->+++<]>.-[--->+<]>---.+++.------.--------.-[--->+<]>.
    Hello World!
    Elapsed time: 0.000009 seconds

