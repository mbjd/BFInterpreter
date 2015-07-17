# BFInterpreter

Brainfuck interpreter in C

Still goofs out when asked for input, but everything else works fine. 1024 * 1 Byte (unsigned char) memory cells.
  
    ./bf programs/test.bf
    Lorem ipsum dolor sit amet.
    Elapsed time: 0.000022 seconds

Or, if you want to enter some code quickly, not unlike any other console:

    ./bf
    Enter a BF program: -[------->+<]>-.-[->+++++<]>++.+++++++..+++.[--->+<]>-----.---[->+++<]>.-[--->+<]>---.+++.------.--------.-[--->+<]>.
    Hello World!
    Elapsed time: 0.000008 seconds
