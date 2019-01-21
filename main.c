#include <stdio.h>
#include <stdlib.h>

void convert_base(int, int, char*);

int main() {
    convert_base(256, 16, "");

    return EXIT_SUCCESS;
}

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