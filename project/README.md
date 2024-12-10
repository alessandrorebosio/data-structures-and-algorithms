# Project of Algorithms and Data Structures     
University of Bologna, Bachelor's Degree in Computer Engineering and Computer Science  
Academic Year 2023/2024  

*Version 1.0, May 6, 2024*

## Instructions
The project involves creating a program in ANSI C (also known as C89 or C90). This document outlines the specifications and how the project should be completed and evaluated.

## Project Execution Mode
The programs must comply with ANSI C, and they will be compiled using the GCC compiler with the following command:

```bash
gcc -std=c90 -Wall -Wpedantic file.c -o file
```

(where the file name will be replaced by the source file submitted; further details below). The compiler should not produce any warnings, especially those related to easily fixable issues such as unused variables/functions, variables used before being initialized, non-void functions that don't return a result, etc. Some specific warnings from Visual Studio (e.g., warnings related to replacing `fopen` with `fopen_s` and similar) will be allowed and should be ignored, as `fopen_s` is not part of ANSI C.

The `main()` function should return 0 (or `EXIT_SUCCESS`) upon successful completion; it is allowed to return an error code if the program terminates abnormally. However, abnormal terminations should never occur, as all inputs provided will be guaranteed to be correct.

Programs should output results to the screen **strictly adhering to the format specified in this document and in the example outputs**. The programs will be initially tested semi-automatically and rejected if the output does not match the required format.

Programs **must not interact with the user** in any way: no system commands (e.g., `system("pause")`), nor should they request user input via keyboard or other means.

Programs must not contain out-of-bounds accesses or any other invalid memory accesses. Any memory allocated with `malloc()` must be explicitly freed with `free()` before program termination. These aspects are crucial because they can cause issues that may not be immediately obvious (e.g., out-of-bounds accesses could cause crashes with some combinations of compiler and OS, but not with others), so maximum attention is required. To verify correct memory usage, the `valgrind` program will be used in a Linux environment. Specifically, the programs will be examined with the following command:

```bash
valgrind ./executable_name input_file
```

which must not show any errors. Don't forget to close the input file using `fclose()`, as otherwise `valgrind` will report memory not freed (this is memory reserved internally by `fopen()` and freed by `fclose()`).

Some input files with corresponding expected results are provided; in some cases, there may be multiple optimal solutions, and the programs will be considered correct if any one of them is calculated (more information will follow). However, **a program that produces the correct result with the provided inputs is not necessarily correct**. The programs will also be tested with inputs different from those provided.

Programs must adhere to the guidelines outlined in the manual [*Programming: A Brief Style Guide*](https://www.moreno.marzolla.name/teaching/programming-style.pdf) available on the lab page, which forms an integral part of these specifications. Non-functional aspects (efficiency, code clarity, etc.) will also be evaluated, and failure to meet these criteria could lead to a negative evaluation and the requirement to modify the source code and resubmit it.

A forum has been set up on the Virtual platform where you can ask questions about the project specifications (i.e., this document); all questions should be asked exclusively on the forum. Since the project is part of the final exam, it should be treated seriously: consequently, **we will not debug code or answer C programming questions**. It is assumed that anyone taking this course already knows how to program in C (as mentioned at the start of the course, this is a prerequisite for this lab).

The project must be done **individually**; discussing the project or solutions with other students or third parties is not allowed. Similarities between programs will be checked using automatic tools, and if confirmed, this will lead to the annulment of the submission for **all** involved students, who will have to submit a new project with new specifications starting from the next exam session.

You are allowed to use the code provided by the instructor during the lab, including the solutions to the proposed exercises. **It is not allowed to use any other code, even if freely available on the internet**. It should be noted that although great care has been taken in developing the programs distributed in class, their correctness is not guaranteed. Each student is **entirely responsible** for the code submitted and will assume responsibility for any errors, even those present in the code provided by the instructor.

## Submission Mode
The project must be submitted through the "Virtual" platform in a single source file, named with your student number (e.g., `0000123456.c`). All functions must be defined within this file, except those from the standard C library. The file must begin with a comment containing your first and last name, student number, group (A or B), and email address (@studio.unibo.it).

The submission deadlines are listed on the Virtual platform, and are generally 7 days before each written exam. After each deadline, new submissions are not possible until the exam date, after which submissions will be reopened until the next deadline, and so on until the last exam session of the academic year.

## Evaluation
After the submission deadline, the programs will receive a binary evaluation (0 = insufficient, 1 = sufficient). Projects that are evaluated positively allow the student to take the written exam in any subsequent exam session, even in later academic years. Projects evaluated positively never expire, so there is no need to resubmit the project even if the exam is taken in later years.

If the evaluation is negative, the program must be modified and resubmitted when submissions are reopened.

## Checklist
Here is a list of points to check before submitting:

1. Was the program submitted in a single file named with the student number?
2. Does the program contain an initial comment with the first and last name, student number, group (A/B), and email address of the author?
3. Is the program compliant with ANSI C (C89 or C90)?
4. Were the suggestions in the guide followed in writing the code (as much as possible)?
5. Does the program compile without warnings?
6. Is the program conforming to the specifications? Does the output for the test cases match exactly the expected format given in this document and the example files?
7. Does the program free all memory allocated with `malloc()` before terminating? Has the absence of memory leaks and out-of-bounds accesses been checked (as much as possible)?

## The Road
A certain geographical area is described by a map divided into a regular grid of *n* rows and *m* columns. The matrix *H*[0..*n* - 1, 0..*m* - 1] represents the average height above sea level of each cell: *H*[*i*, *j*] indicates the average height of the cell (*i*, *j*) located in row *i* and column *j*. The heights are integers, which can also be negative, as portions of land can lie below sea level.

We want to build a road that starts at cell (0, 0) (top-left) and ends at cell (*n* - 1, *m* - 1) (bottom-right). The road must pass through adjacent cells, i.e., cells that share a common side. An example of a valid road is represented in the figure below; the road passes through the cells highlighted in gray.

## Constraints:
1. The first cell (*x*0, *y*0) must be (0, 0).
2. The last cell (*xk* - 1, *yk* - 1) must be (*n* - 1, *m* - 1).
3. Each cell must share a side with the next one. Formally, for every *i* = 0, ..., *k* - 2, cell (*xi*, *yi*) must have a side in common with (*xi+1*, *yi+1*).
4. The road must not pass through the same cell more than once.
5. The road must not exit the grid's boundaries.

The construction of the road has two components to its cost:
1. For each cell traversed, there is a fixed cost *Ccell* (non-negative integer).
2. For each pair of adjacent cells traversed, (*xi*, *yi*) and (*xi+1*, *yi+1*), an additional cost is paid, equal to *C* multiplied by the square of the elevation difference, i.e., *C* × (*H*[*xi*, *yi*] - *H*[*xi+1*, *yi+1*])².

The goal is to design and implement an efficient algorithm to determine the cells for constructing a road with the minimal total cost, adhering to the constraints. If there are multiple optimal roads, any of them can be chosen. The optimal solution does not necessarily correspond to the road with the fewest cells.

### Input
The program should accept a single parameter in the command line, which is the name of a text file with the following structure:

- The first line contains the cost *Ccell*.
- The second line contains the cost *Cheight*.
- The third line contains the number of rows *n* of the matrix.
- The fourth line contains the number of columns *m* of the matrix.
- The

 following *n* lines each contain *m* integers, representing the height values *H[i][j]* of each cell.

### Output
The program must output:

1. The coordinates of the cells traversed by the road, one per line, with each coordinate pair separated by a space.
2. A line with `-1 -1` indicating the end of the path.
3. The total cost of the road, calculated using the formula provided, as an integer (no decimal part).

## Examples

### Input:
```
50
1000
6
5
100 123 129 98 100
99 124 108 102 101
98 99 127 105 104
102 112 100 102 103
105 107 182 198 104
105 106 170 207 100
```

### Expected Output:
```
0 0
1 0
2 0
3 0
4 0
5 0
5 1
4 1
3 1
3 2
3 3
3 4
4 4
5 4
-1 -1
220700
```

### Input:
```
100
80
5
5
100 100 100 100 100
100 100 100 100 100
100 100 100 100 100
100 100 100 100 100
-10 -10 -10 -10 -10
```

### Expected Output:
```
0 0
1 0
2 0
3 0
4 0
4 1
4 2
4 3
4 4
-1 -1
968900
```