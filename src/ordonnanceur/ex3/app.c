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

#include <sys/types.h>
#include <sys/socket.h>

#include <signal.h>


#define DATA "Hello world from child"
#define EXIT_MSG "exit"

#define MAX_MSG 5

int parent(){

    while (1)
    {



    }

    return 0;
}

int child(){


    while (1)
    {

    }

    return 0;
}

/**
 * Main program computing the Fibonacci numbers for a given sequence starting
 * from 0 to a number specified at the command line.
 */
int main(int argc, char **argv)
{


    printf("Ordonnanceur Ex 3\n");


    pid_t pid = fork();
    if (pid == 0){
        /* code de l’enfant */
        exit(child());
    }
    else if (pid > 0){
        /* code du parent */
        exit(parent());
    }
    else{
        /* error */
    }


    return 0;
}
