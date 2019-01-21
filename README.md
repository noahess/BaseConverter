# Base Conversions in C
This repo is one of the most painful and inefficient way to print a `signed int` in base 2 through 32.
If the only external function allowable was `fputc` and everything in `math.h` was off-limits,
what is a _"good"_ way to write base conversion code (optimizing for line length)? Without doing anything too insane, this is the result:

## Setup
Remember, we are only allowed to use `fputc`
```c
#include <stdio.h>
```
We want to be able to pass in a number, a base, and an identifier string that leads the number (something like `0b`)

```c
void convert_base(int n_in, int base, char* identifier) {
```

Next, we want to be able to work with any signed input, so we're going to need to correct
the sign of any input, so we use a ternary expression - if `n_in` is negative, make it positive, 
else leave it alone.

```c
int n = (n_in < 0) ? (-1 * n_in) : n_in
```

We also want to print a negative sign if the number is negative, so we add it to the expression:

```c
int n = (n_in < 0) ? (fputc(45, stdout), (-1 * n_in)) : n_in
```

Finally, because we would get an overflow error if we tried to find the absolute value of the 
most negative integer, we must type cast to an unsigned int:

```c
unsigned int n = (unsigned int) (n_in < 0 ? fputc(45, stdout), -1 * n_in : n_in);
```

Next, we want to print out the base identifier (if it exists). We just loop until we hit the null character:
```c
for (int i = 0; identifier[i]; i++) {
    fputc(identifier[i], stdout);
}
```

Putting it all together at this point, we have:
```c
#include <stdio.h>

void convert_base(int n_in, int base, char* identifier) {
    unsigned int n = (unsigned int) (n_in < 0 ? fputc(45, stdout), -1 * n_in : n_in);
    for (int i = 0; identifier[i]; i++) {
        fputc(identifier[i], stdout);
    }
}
```

##Base Conversion

The big idea for the base conversion section is to loop through all digits and print out the digit required.

We're going to take advantage of the initialization part of the for loop and declare a variable `divisor` which will help
us decompose `n`. We will pull out the largest factor we can from `n` and repeat the same process with the remainders 
until `n` is exactly zero. This means that our loop will run when `n` is nonzero and at every iteration, we will take
`n = n % divisor` to calculate the remainder.
```c
for (int divisor; n; n = n % divisor) {
```

Next, we want to figure out the largest multiple of the `base` such that it is smaller than `n` so we can find the
next most significant digit. We will take advantage of another for loop to calculate this number and store it in 
`divisor`:

```c
for (divisor = 1; divisor <= n / base; divisor *= base) {}
```

Now that we know the next most important digit `n / divisor`, we can print it out. Remember that `0` is ASCII 
number `48` and `97` is ASCII letter `a` which represents `10`, so we take the value `a` and subtract `10` to get our
offset, `87`:

```c
fputc(n / divisor <= 9 ? (n / divisor) + 48 : (n / divisor) + 87, stdout);
```

Putting it together, we have:

```c
#include <stdio.h>

void convert_base(int n_in, int base, char* identifier) {
    unsigned int n = (unsigned int) (n_in < 0 ? fputc(45, stdout), -1 * n_in : n_in);
    for (int i = 0; identifier[i]; i++) {
        fputc(identifier[i], stdout);
    }
    for (int divisor; n; n = n % divisor) {
        for (divisor = 1; divisor <= n / base; divisor *= base) {}
        fputc(n / divisor <= 9 ? (n / divisor) + 48 : (n / divisor) + 87, stdout);
    }
}
```

We're not quite done, however, because this algorithm will ignore placeholder zeros. We need to print zeros on a loop,
reducing `divisor` until either: A) `divisor` becomes smaller than `base` (implying a decimal), or B) the result
of the next `divisor` reduction will result in a nonzero number. We can take advantage of the `for` loop to 
accomplish this task.

```c
for( ; divisor / base && !((n % divisor) / (divisor / base)); divisor = divisor / base) {
    fputc(48, stdout);
}
```

At last, we can put everything together into:

```c
#include <stdio.h>

void convert_base(int n_in, int base, char* identifier) {
    unsigned int n = (unsigned int) (n_in < 0 ? fputc(45, stdout), -1 * n_in : n_in);
    for (int i = 0; identifier[i]; i++) {
        fputc(identifier[i], stdout);
    }
    for (int divisor; n; n = n % divisor) {
        for (divisor = 1; divisor <= n / base; divisor *= base) {}
        fputc(n / divisor <= 9 ? (n / divisor) + 48 : (n / divisor) + 87, stdout);
        for( ; divisor / base && !((n % divisor) / (divisor / base)); divisor = divisor / base) {
            fputc(48, stdout);
        }
    }
}
```

## Results

This is extraordinarily painful and borderline unreadable but `cloc` counts only 12 sloc. 
A test file `main.c` can be compiled to verify that the code works.