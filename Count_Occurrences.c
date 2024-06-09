#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#define NUM_FILES 100
#define MAX_FILENAME_LENGTH 100
int count_occurrences(int x, char filenames[MAX_FILENAME_LENGTH]) {
    FILE* file = fopen(filenames, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filenames);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int count = 0;
    int num;
    while (fscanf(file, "%d", &num) != EOF) {
        if (num == x) {
            count++;
        }
    }
    fclose(file);
    return count;
}
int main(int argc, char** argv) {
    int rank, size;
    char directory[MAX_FILENAME_LENGTH];
    int x;
    char filenames[NUM_FILES][MAX_FILENAME_LENGTH];
    int counts[NUM_FILES];
    int total_count = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand (time(NULL));
    if (rank == 0) {
        printf("Enter the directory for the files: ");
        scanf("%s", directory);

        printf("Enter number x: ");
        scanf("%d", &x);
        // Create files and add their names to the array
        for (int i = 0; i < NUM_FILES; i++) {
            sprintf(filenames[i], "%s/file%d.txt", directory, i);
            FILE* file = fopen(filenames[i], "w");
            if (file == NULL) {
                printf("Error creating file: %s\n", filenames[i]);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            //int value = (i < 10) ? 1 : 2; // Set value based on file index
            /*
            int value;
            if(i<10)
            {
                value = 1;
            }
            else if(10<=i && i<20)
            {
                value = 2;
            }
            else{
                value = 3;
            }*/
            for (int j = 0; j < 100; j++) {
                int value = rand() % 100 + 1;
                fprintf(file, "%d\n", value);
            }
            fclose(file);
        }

        

        
        int num_files = NUM_FILES / size;
        for (int i = 1; i < size; i++) {
            MPI_Send(&num_files, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            for (int j = 0; j < num_files; j++) {
                MPI_Send(filenames[j + i * num_files], MAX_FILENAME_LENGTH, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
        for (int i = 0; i < num_files; i++) {
            counts[i] = count_occurrences(x, filenames[i]);
            total_count += counts[i];
            

        }
                //handle the remaining files
        for (int i = num_files * size; i < NUM_FILES; i++) {
            counts[i] = count_occurrences(x, filenames[i]);
            total_count += counts[i];
        }
        

        printf("P%d: Total number of occurrences = %d\n", rank, total_count);
        fflush(stdout);
        for (int i = 1; i < size; i++) {
            int count;
            MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count += count;
        }

        printf("\nTotal count of %d occurrences in all 100 files: %d\n", x, total_count);

    }
    else {
        int num_files;
        MPI_Recv(&num_files, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < num_files; i++) {
            MPI_Recv(filenames[i], MAX_FILENAME_LENGTH, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            counts[i] = count_occurrences(x, filenames[i]);
            total_count += counts[i];
        }
        
        printf("P%d: Total number of occurrences = %d\n", rank, total_count);
        fflush(stdout);
        MPI_Send(&total_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
    }

    MPI_Finalize();

}