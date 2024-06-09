# Count-Occurrences-of-a-certain-number
This is a parallel C program using MPI to find out the number of occurrences of a certain number in some files.

## Parallelization Scenario:
### Master Process:
- The master process first creates 100 files in the directory given from the user, then adds the files’ name in an array.
- Fill those 100 files with 100 random numbers (from 1 to 100).
- Take an integer number x from the user as an input.
- Broadcast the number x to all the slave processes.
- Scatter the array of the files’ names among all processes (including the master).
- If there are any remaining files, let the master handle them.
- Receive the number of occurrences of number x from all processes.
- Calculate the total number of occurrences of x in all the 100 files and print it out.
### Slave Process:
- Receive x and its portion of the array.
- Calculate the number of occurrences of x in each file in the position array.
- Find out the number of occurrences of x in each file.
- Calculate the total number of occurrences of x in all the given files.
- Send the total number of occurrences to the master process.


## Example: //if we run the problem with 10 processes
### Input:
- Enter the directory for the files:
D:\HPC\A2
- Enter number x:
3
### Output:
P0: Total number of occurrences = 6 <br>
P1: Total number of occurrences = 4 <br>
P2: Total number of occurrences = 2 <br>
P3: Total number of occurrences = 11<br>
P4: Total number of occurrences = 5<br>
P5: Total number of occurrences = 1<br>
P6: Total number of occurrences = 9<br>
P7: Total number of occurrences = 3<br>
P8: Total number of occurrences = 0<br>
P9: Total number of occurrences = 1<br>
Total number of occurrences in all 100 files = 42
