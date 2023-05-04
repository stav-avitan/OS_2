#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 256
#define BUFFER_SIZE 4096


int copy_file(const char *source_file, const char *target_file, int verbose, int force) {
    FILE *source, *target;
    char buffer[BUFFER_SIZE];
    int bytes_read, bytes_written;
    int success = 0;

    // Check if target file already exists
    if (!force && access(target_file, F_OK) == 0 ) {
        if (access(source_file, F_OK) == 0 && access(target_file, F_OK) == 0) {
			//printf("%");
            if (verbose) {
                //printf("target file exists\n");
            }
			target = fopen(target_file, "ab");
            //return 1; // Target file already exists
			
        }
		else
		{
			//printf("%%%%");
			return 1;
		}
    }
	else
	{
		// Check if both files exist
		if (access(source_file, F_OK) == 0 && access(target_file, F_OK) == 0) {
			// Both files exist, append source to target
			if (verbose) {
				//printf("appending %s to %s\n", source_file, target_file);
			}
			target = fopen(target_file, "wb");
		} else {
			// One or both files do not exist, create target file
			target = fopen(target_file, "wb");
		}
	}

    // Open source file for reading
    source = fopen(source_file, "rb");
    if (source == NULL) {
        return 1; // Error opening source file
    }

    // Copy contents of source file to target file
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, source)) > 0) {
        bytes_written = fwrite(buffer, 1, bytes_read, target);
        if (bytes_written != bytes_read) {
            success = 1;
            break;
        }
    }

    // Close files
    fclose(source);
    fclose(target);

    if (success) {
        if (verbose) {
            printf("general failure\n");
        }
        return 1; // Copy failed
    } else {
        if (verbose) {
            printf("success\n");
        }
        return 0; // Copy succeeded
    }
}

int main(int argc, char **argv) {
    char source_file[MAX_FILENAME_LENGTH];
	char target_file[MAX_FILENAME_LENGTH];
    int verbose = 0, force = 0;
	//printf("****");
	int i;
	for (i =3; i < argc; i++) {
		//printf("****");
		if (strcmp(argv[i], "-v") == 0) {
			verbose = 1;
		} else if (strcmp(argv[i], "-f") == 0) {
			force = 1;
		} /*else if (i == argc - 2) {
			strncpy(source_file, argv[i], MAX_FILENAME_LENGTH);
		} else if (i == argc - 1) {
			strncpy(target_file, argv[i], MAX_FILENAME_LENGTH);
		}*/ 
		else
		{
			//none
		}
		//printf("****");
	}

	i=1;
    if (i + 1 >= argc) {
        printf("Usage: %s <file1> <file2>  [-v] [-f] \n", argv[0]);
        return 1;
    }
	
    // Get source and target filenames
    strncpy(source_file, argv[i], MAX_FILENAME_LENGTH);
    strncpy(target_file, argv[i+1], MAX_FILENAME_LENGTH);

    // Copy the file
    int result = copy_file(source_file, target_file, verbose, force);

    return result;
}