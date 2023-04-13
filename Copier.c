#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> //found how to use the last 4 librays stack overflow
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PROGRESS_BAR_LENGTH 50


void error(const char *message) { //give error message and exit the program
    perror(message);
    exit(EXIT_FAILURE);
}

void print_bar(long long current, long long total) { //prints the progress bar and clears the buffer every time it is called
    int completed = (int)(((double)current / total) * PROGRESS_BAR_LENGTH);
    printf("\r[");
    for (int i = 0; i < PROGRESS_BAR_LENGTH; i++) {
        if (i < completed) {
            printf("#");
        } else {
            printf(" ");
        }
    }
    printf("] %3.1f%%", (double)current / total * 100);
    fflush(stdout); 
}

int main(int argc, char *argv[]) {
    if (argc != 3) {//check arguments
        printf("Usage: ./Copier src_file dst_file\n");
        return 1;
    }

    char *source_name = argv[1];
    char *final_path = argv[2];

    int source_file = open(source_name, O_RDONLY);
    if (source_file == -1) {//check if source file opend
        error("Error opening source file");
        return 1;
    }

    int final_file = open(final_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);//found on stack overflow
    if (final_file == -1) {//check if copy file was created
        close(source_file);
        error("Error creating copy file");
        return 1;
    }

    struct stat st;
    if (stat(source_name, &st) == -1) {// check source file size
        close(source_file);
        close(final_file);
        error("Error finding file size");
        return 1;
    }

    long long total_bytes = st.st_size;
    long long copied_bytes = 0;
    ssize_t read_bytes;
    ssize_t written_bytes;
    char buffer[BUFFER_SIZE];

    while ((read_bytes = read(source_file, buffer, BUFFER_SIZE)) > 0) {
        char *write_ptr = buffer;
        do {
            written_bytes = write(final_file, write_ptr, read_bytes);
            if (written_bytes >= 0) {
                read_bytes = read_bytes - written_bytes;
                write_ptr = write_ptr + written_bytes;
                copied_bytes = copied_bytes + written_bytes;
                print_bar(copied_bytes, total_bytes);
            } else { //error in coping file
                close(source_file);
                close(final_file);
                error("Error writing to copy file");
                return 1;
            }
        } while (read_bytes > 0);
    }

    if (read_bytes == -1) {
        close(source_file);
        close(final_file);
        error("Error reading from source file");
        return 1;
    }

    printf("\nFile copied successfully.\n");
    close(source_file);
    close(final_file);
    return 0;
}
