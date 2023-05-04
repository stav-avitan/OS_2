#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

int compare_files(FILE *file1, FILE *file2, int verbose, int ignore_case) {
    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    //int line_number = 1;
    int result = 0;
	
	//printf("verbos:%d , case:%d",verbose, ignore_case);

    while (fgets(line1, MAX_LINE_LENGTH, file1) != NULL && fgets(line2, MAX_LINE_LENGTH, file2) != NULL) {
        if (ignore_case) {
            int i;
            for (i = 0; line1[i] != '\0' && line2[i] != '\0'; i++) {
                if (tolower(line1[i]) != tolower(line2[i])) {
                    result = 1;
                    break;
                }
            }
            if (line1[i] != line2[i]) {
                result = 1;
                break;
            }
        } else {
            if (strcmp(line1, line2) != 0) {
                result = 1;
                break;
            }
        }
    }

    if (result == 0 && fgets(line1, MAX_LINE_LENGTH, file1) == NULL && fgets(line2, MAX_LINE_LENGTH, file2) == NULL) {
        if (verbose) {
            printf("equal\n");
        }
        return 0; // The files are identical
    } else {
        if (verbose) {
            printf("not equal\n");
        }
        return 1; // The files are different
    }
}

int main(int argc, char **argv) {
    FILE *file1, *file2;
    int verbose = 0;
    int ignore_case = 0;
    int i;
	
	// Parse command-line arguments
for (i = argc-1; i > 2; i--) {
    if (strcmp(argv[i], "-v") == 0) {
        verbose = 1;
    } else if (strcmp(argv[i], "-i") == 0) {
        ignore_case = 1;
    } else if (argv[i][0] == '-') {
        printf("Error: Unknown option '%s'\n", argv[i]);
        return 1;
    } else {
        break;
    }
	
	//printf("verbos:%d , case:%d\n",verbose, ignore_case);
}

// Check that we have two filenames as arguments
if (i + 1 >= argc) {
    printf("Usage: %s <file1> <file2> [-v] [-i]\n", argv[0]);
    return 1;
}

// Open the files
//printf("*%d*",i);
file1 = fopen(argv[i-1], "r");
if (file1 == NULL) {
    printf("Error: Could not open file '%s'\n", argv[1]);
    return 1;
}
file2 = fopen(argv[i], "r");
if (file2 == NULL) {
    printf("Error: Could not open file '%s'\n", argv[2]);
    fclose(file1);
    return 1;
}

// Compare the files
int result = compare_files(file1, file2, verbose, ignore_case);

// Close the files
fclose(file1);
fclose(file2);

return result;
}