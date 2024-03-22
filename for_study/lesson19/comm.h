#include <stdio.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>

#define PATH_NAME "/home/tiefxblau/lesson19"
#define PROJ_ID 0x7777
#define SIZE (1024 * 4)
