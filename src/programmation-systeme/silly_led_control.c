 /**
 * Copyright 2018 University of Applied Sciences Western Switzerland / Fribourg
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
 * Project:	HEIA-FR / HES-SO MSE - MA-CSEL1 Laboratory
 *
 * Abstract: System programming -  file system
 *
 * Purpose:	NanoPi silly status led control system
 *
 * Autĥor:	Daniel Gachet
 * Date:	07.11.2018
 */
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

/*
 * status led - gpioa.10 --> gpio10
 * power led  - gpiol.10 --> gpio362
 */
#define GPIO_EXPORT   "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_LED      "/sys/class/gpio/gpio10"
#define LED           "10"
#define K1            "0"
#define K2            "2"    
#define K3            "3"
#define GPIO_K1     "/sys/class/gpio/gpio0"
#define GPIO_K2     "/sys/class/gpio/gpio2"
#define GPIO_K3     "/sys/class/gpio/gpio3"

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
    return f;
}

int main(int argc, char* argv[])
{
    fd_set fd_in, fd_out, fd_except;

    setlogmask(LOG_UPTO(LOG_INFO));

    openlog("log_interval", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);


    long duty   = 2;     // %
    long period = 1000;  // ms
    if (argc >= 2) period = atoi(argv[1]);
    period *= 1000000;  // in ns
    long time_interval = period / 2;

    // compute duty period...
    long p1 = period / 100 * duty;
    long p2 = period - p1;
    printf("Hey jhajsja\n");
    int led = open_gpio(LED, "out", GPIO_LED);
    int k1 = open_gpio(K1, "in", GPIO_K1);
    int k2 = open_gpio(K2, "in", GPIO_K2);
    int k3 = open_gpio(K3, "in", GPIO_K3);
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    struct itimerspec spec = {
        .it_interval = {0, 0},
        .it_value = {0, time_interval}
    };
    timerfd_settime(tfd, 0, &spec, NULL);
    printf("Hey 3\n");


    // monitor fd1 for input events and fd2 for output events
    //FD_SET(k1, &fd_in);
    //FD_SET(k2, &fd_in);
    //FD_SET(k3, &fd_in);
    //FD_SET(fd2, &fd_out);
    // find out which fd has the largest numeric value
    int largest_fd = (k1 > k2) ? k1 : k2;
    largest_fd = (largest_fd > k3) ? largest_fd : k3;
    largest_fd = (largest_fd > tfd) ? largest_fd : tfd;
    pwrite(led, "1", sizeof("1"), 0);

    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    char buff[10] = "";
    int k = 0;
    while (1) {
        /*
        struct timespec t2;
        clock_gettime(CLOCK_MONOTONIC, &t2);

        long delta =
            (t2.tv_sec - t1.tv_sec) * 1000000000 + (t2.tv_nsec - t1.tv_nsec);

        int toggle = ((k == 0) && (delta >= p1)) | ((k == 1) && (delta >= p2));
        pread(k1, buff, sizeof("1"), 0);
        //printf("0 : %x\n", buff[0]);
        //printf("1 : %x\n", buff[1]);
        //printf("Value : %s",buff);
        if (buff[0]=='0'){
            if (toggle) {
                t1 = t2;
                k  = (k + 1) % 2;
                if (k == 0)
                    pwrite(led, "1", sizeof("1"), 0);
                else
                    pwrite(led, "0", sizeof("0"), 0);
            }
        }*/
            // wait for events
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

                time_interval -= 100000000;

                syslog(LOG_INFO, "Time interval increased : %d", time_interval);

                //FD_CLR(k1, &fd_except);
                }
            if (FD_ISSET(k2, &fd_except)) {
                pread(k2, buff, sizeof("1"), 0);
                printf("K2 pressed\n");

                time_interval = period / 2;

                syslog(LOG_INFO, "Time interval reset : %d", time_interval);

                //FD_CLR(k2, &fd_except);
                }
            if (FD_ISSET(k3, &fd_except)) {
                pread(k3, buff, sizeof("1"), 0);
                printf("K3 pressed\n");

                time_interval += 100000000;

                syslog(LOG_INFO, "Time interval decreased : %d", time_interval);

                //FD_CLR(k3, &fd_except);
            }
            if (FD_ISSET(tfd, &fd_in)) {
                pread(tfd, buff, 8, 0);
                printf("timer occurred %d\n", time_interval);
                
                spec.it_value.tv_nsec = time_interval;

                timerfd_settime(tfd, 0, &spec, NULL);

                // led
                k  = (k + 1) % 2;
                if (k == 0)
                    pwrite(led, "1", sizeof("1"), 0);
                else
                    pwrite(led, "0", sizeof("0"), 0);

                //FD_CLR(k3, &fd_except);
            }
        } 
    }

    closelog();

    return 0;
}
