#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>

int main(int argc, char *argv[])
{
    char *program = *argv++;
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s <includepath_file> <dest>\n", program);
        return 1;
    }

    char *file_path = *argv++;
    char *dest_path = *argv++;

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Could not open file '%s'. ERRNO: %d\n", file_path, errno);
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&line, &len, fp)) != -1) {
        line[strcspn(line, "\n")] = 0;

        char *rsync = "rsync -a ";
        char command[strlen(rsync) + strlen(line) + 1 + strlen(dest_path)];
        strcpy(command, rsync);
        strcat(command, line);
        strcat(command, " ");
        strcat(command, dest_path);

        printf("INFO: Executing command: '%s'\n", command);
        system(command);
    }
    
    fclose(fp);

    return 0;
}
