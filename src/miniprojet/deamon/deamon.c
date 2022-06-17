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
 * Purpose:	This module implements a simple daemon to be launched
 *		from a /etc/init.d/S??_* script
 *
 * Autĥor:	Daniel Gachet
 * Date:	28.08.2015
 */
#define _XOPEN_SOURCE 600
#define _DEFAULT_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <syslog.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

#define GPIO_EXPORT   "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define K1            "0"
#define K2            "2"    
#define K3            "3"
#define GPIO_K1     "/sys/class/gpio/gpio0"
#define GPIO_K2     "/sys/class/gpio/gpio2"
#define GPIO_K3     "/sys/class/gpio/gpio3"

#define UNUSED(x) (void)(x)

static int signal_catched = 0; 

static int open_gpio(char* gpio_n, char* direction, char* path){
    char dest[50] = "";
    // unexport pin out of sysfs (reinitialization)
    int f = open(GPIO_UNEXPORT, O_WRONLY);
    write(f, gpio_n, strlen(gpio_n));
    close(f);

    // export pin to sysfs
    f = open(GPIO_EXPORT, O_WRONLY);
    write(f, gpio_n, strlen(gpio_n));
    close(f);

    // config pin
    strcpy(dest, path);
    strcat(dest, "/direction");
    f = open(dest, O_WRONLY);
    write(f, direction, 3);
    close(f);
    if (strcmp(direction, "in")==0)
    {
        printf("Went into the rising for %s\n", gpio_n);
        // config pin
        strcpy(dest, path);
        strcat(dest, "/edge");
        f = open(dest, O_WRONLY);
        write(f, "rising", strlen("rising"));
        close(f);
    }


    // open gpio value attribute
    strcpy(dest, path);
    strcat(dest, "/value");
    f = open(dest, O_RDWR);
	syslog(LOG_INFO, "Open GPIO\n");
    return f;
}

static void catch_signal (int signal)
{
	syslog (LOG_INFO, "signal=%d catched\n", signal);
	signal_catched++;
}

static void fork_process()
{
	pid_t pid = fork();
	switch (pid) {
	case  0: break; 	// child process has been created
	case -1: syslog (LOG_ERR, "ERROR while forking"); exit (1); break;	
	default: exit(0);  	// exit parent process with success
	}
}

int main(int argc, char* argv[])
{
	UNUSED(argc); UNUSED(argv);

	// 1. fork off the parent process
	fork_process();

	// 2. create new session
	if (setsid() == -1) {
		syslog (LOG_ERR, "ERROR while creating new session"); 
		exit (1);
	}

	// 3. fork again to get rid of session leading process 
	fork_process();

	// 4. capture all required signals
	struct sigaction act = {.sa_handler = catch_signal,};
	sigaction (SIGHUP,  &act, NULL);  //  1 - hangup
	sigaction (SIGINT,  &act, NULL);  //  2 - terminal interrupt
	sigaction (SIGQUIT, &act, NULL);  //  3 - terminal quit
	sigaction (SIGABRT, &act, NULL);  //  6 - abort
	sigaction (SIGTERM, &act, NULL);  // 15 - termination
	sigaction (SIGTSTP, &act, NULL);  // 19 - terminal stop signal

	// 5. update file mode creation mask
	umask(777);

	// 6. change working directory to appropriate place
	if (chdir ("/") == -1) {
		syslog (LOG_ERR, "ERROR while changing to working directory"); 
		exit (1);
	}

	// 7. close all open file descriptors
	for (int fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
		close (fd);
	}

	// 8. redirect stdin, stdout and stderr to /dev/null
	if (open ("/dev/null", O_RDWR) != STDIN_FILENO) {
		syslog (LOG_ERR, "ERROR while opening '/dev/null' for stdin");
		exit (1);
	}
	if (dup2 (STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) {
		syslog (LOG_ERR, "ERROR while opening '/dev/null' for stdout");
		exit (1);
	}
	if (dup2 (STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO) {
		syslog (LOG_ERR, "ERROR while opening '/dev/null' for stderr");
		exit (1);
	}

	// 9. option: open syslog for message logging
	openlog (NULL, LOG_NDELAY | LOG_PID, LOG_DAEMON); 
	syslog (LOG_INFO, "Daemon has started...");
 
	// 10. option: get effective user and group id for appropriate's one
	/*
	struct passwd* pwd = getpwnam ("daemon");
	if (pwd == 0) {
		syslog (LOG_ERR, "ERROR while reading daemon password file entry");
		exit (1);
	}

	// 11. option: change root directory
	if (chroot (".") == -1) {
		syslog (LOG_ERR, "ERROR while changing to new root directory");
		exit (1);
	}

	// 12. option: change effective user and group id for appropriate's one
	if (setegid (pwd->pw_gid) == -1) {
		syslog (LOG_ERR, "ERROR while setting new effective group id");
		exit (1);
	}
	if (seteuid (pwd->pw_uid) == -1) {
		syslog (LOG_ERR, "ERROR while setting new effective user id");
		exit (1);
	}
	*/

	// 13. implement daemon body...

	//Init
    fd_set fd_in, fd_out, fd_except;

    setlogmask(LOG_UPTO(LOG_INFO));

    openlog("log_interval", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    int k1 = open_gpio(K1, "in", GPIO_K1);
    int k2 = open_gpio(K2, "in", GPIO_K2);
    int k3 = open_gpio(K3, "in", GPIO_K3);
	syslog(LOG_INFO, "Done GPIO\n");
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    long period = 1000;  // ms
    period *= 1000000;  // in ns
    long time_interval = period / 2;

    struct itimerspec spec = {
        .it_interval = {0, 0},
        .it_value = {0, time_interval}
    };
    timerfd_settime(tfd, 0, &spec, NULL);


    int largest_fd = (k1 > k2) ? k1 : k2;
    largest_fd = (largest_fd > k3) ? largest_fd : k3;
    largest_fd = (largest_fd > tfd) ? largest_fd : tfd;

    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    char buff[10] = "";
    int k = 0;
	//Infinity loop
	while(1){
        FD_ZERO(&fd_in);
        FD_ZERO(&fd_out);
        FD_ZERO(&fd_except);
        FD_SET(k1, &fd_except);
        FD_SET(k2, &fd_except);
        FD_SET(k3, &fd_except);
        FD_SET(tfd, &fd_in);
        int ret = select (largest_fd+1, &fd_in, &fd_out, &fd_except, NULL);
        // check if select actually succeed
        if (ret == (-1)) {
        // report error and abort
        } 
        else if (ret == 0) {
        // timeout; no event detected
        } else {
            if (FD_ISSET(k1, &fd_except)) {
                pread(k1, buff, sizeof("1"), 0);
                printf("K1 = 1\n");
                syslog(LOG_INFO, "Time interval increased : %d", time_interval);
                }
            if (FD_ISSET(k2, &fd_except)) {
                pread(k2, buff, sizeof("1"), 0);
                printf("K2 pressed\n");
                syslog(LOG_INFO, "Time interval reset : %d", time_interval);
                }
            if (FD_ISSET(k3, &fd_except)) {
                pread(k3, buff, sizeof("1"), 0);
                printf("K3 pressed\n");
                syslog(LOG_INFO, "Time interval decreased : %d", time_interval);
            }
            if (FD_ISSET(tfd, &fd_in)) {
                pread(tfd, buff, 8, 0);
                printf("timer occurred %d\n", time_interval);
                //spec.it_value.tv_nsec = time_interval;
                timerfd_settime(tfd, 0, &spec, NULL);
				syslog(LOG_INFO, "Timer occured.\n");
            }
        } 
	}

	syslog (LOG_INFO, "daemon stopped V2. Number of signals catched=%d\n", signal_catched);
	closelog();

	return 0;
}