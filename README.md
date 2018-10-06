# Shell implementation in C

This is a basic implementation of shell in C using syscalls.

### How to run?
* Run make on your terminal 
* Run ./shell

### Files
* headers.h - contains all the header files
* functions.h - contains all function prototypes
* main.c  - Driver program. Contains functions to show prompt, read commands and run cd command 
* current_path.c - returns the name current working directory
* echo.c - Function to run echo command
* ls.c - ls command
* pinfo.c - Contains the function to print the status of a process
* clock.c - clock command
* pwd.c - prints current working directory
* remindme.c - remindme command
* bg.c - changes a stopped background job to a running background job
* fg.c - brings a running or a stopped background job with given job number to foreground.
* jobs.c - prints a list of all running background processes
* kjob.c - sends signal a job with given job number
* overkill.c - kills all background processes
* setenv.c - creates and removes environment variables
* redirect.c - input outout redirection
* pipe.c  - piping, piping + redirection