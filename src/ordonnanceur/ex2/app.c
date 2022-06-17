/**
 * Copyright 2015 University of Applied Sciences Western Switzerland / Fribourg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Module:	MA-CSEL1 - Building Embedded Systems under Linux
 *
 * Abstract: 	Introduction to the Embedded Linux Development Environment
 *
 * Purpose:	Fibonacci numbers: a simple demo program to exercice debugging
 * 		facilities on embedded Linux system.
 *
 *		-->> tracing with printf
 *
 * Autĥor:	Daniel Gachet
 * Date:	28.08.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <syslog.h>


#include <sys/types.h>
#include <sys/socket.h>

#include <signal.h>

#define MEM_TO_ALLOC 50000000

#define SUB_MEM "/sys/fs/cgroup/memory/mem/tasks"
#define LIMIT_MEM "/sys/fs/cgroup/memory/mem/memory.limit_in_bytes"
#define LIMIT_MEM_VAL "20M" 

/**
 * Main program computing the Fibonacci numbers for a given sequence starting
 * from 0 to a number specified at the command line.
 */
int main(int argc, char **argv)
{
    char  buf[1024];


    printf("Ordonnanceur Ex 2 \n");

    snprintf(buf, sizeof(buf), "%d", getpid());

    printf("Pid : %s\n", buf);

    // subscribe pid to mem cgroup
    int f = open(SUB_MEM, O_WRONLY);
    write(f, buf, strlen(buf));
    close(f);

    // limit mem cgroup to 20M
    f = open(LIMIT_MEM, O_WRONLY);
    write(f, LIMIT_MEM_VAL, strlen(LIMIT_MEM_VAL));
    close(f);

    int *pointer = (int *)malloc(MEM_TO_ALLOC);

    if(pointer != NULL){
        printf("Allocation works %d\n", sizeof(pointer));
        free(pointer);
    }
    else{
        printf("Allocation doesn't works\n");
    }

    return 0;
}
