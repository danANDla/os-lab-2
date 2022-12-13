#include <string.h>
#include <stdio.h>

#define OUTPUT_BUFFER_SIZE 2000
#define KMODULE "/proc/sreader"

int main() {

    char output[OUTPUT_BUFFER_SIZE];

    FILE *r_file;

    r_file = fopen(KMODULE, "r");
    while (fgets(output, 256, r_file) != NULL) {
        printf("%s", output);
    }
    fclose(r_file);

    return 0;
}
