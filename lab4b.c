#include <pthread.h>
#include <stdio.h> //for printing
#include <stdlib.h> 
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h> 
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <math.h>
#include <rc/button.h>
#include <rc/time.h>
#include <rc/gpio.h>
#include <rc/adc.h>
#include <time.h>


int period_interval = 1;
int use_farenheight = 1;
int should_stop = 0; 
int should_start = 1;
//print tempature to 1 
int log_fd = -1;

void* thread_temperature() {


    return NULL;
}

int main(int argc, char *argv[]) {

    srand(time(0));


    int curr_option;
    const struct option options[] = {
        { "scale",  required_argument, NULL,  's' },
        { "period", required_argument, NULL,  'p' },
     { "log", required_argument, NULL, 'l'},
        { 0, 0, 0, 0}
    };


    char* log_name = NULL;
    while((curr_option = getopt_long(argc, argv, "c:p:s:t:l:o", options, NULL)) != -1)  {
        switch(curr_option) {
            case 's':
                if(*optarg == 'F') {
                    use_farenheight = 1;
                } else if (*optarg == 'C') {
                    use_farenheight = 0;
                } else {
                    fprintf(stderr, "You can only specify f or c for scale ");
                    exit(1);
                }
                break;
            case 'p':
                period_interval = atoi(optarg);
                break;
            case 'l':
                log_name = optarg;
            default:
                fprintf(stderr, "Use the options --iterations --threads");
                exit(1);
                break;
        }
    }

    // int output_init = rc_gpio_init (1, 18, GPIOHANDLE_REQUEST_INPUT);
    // printf("The output of init is %d \n", output_init);


    int button_fd =  rc_gpio_init_event(1, 18, 0, GPIOEVENT_REQUEST_RISING_EDGE);
    if(button_fd  == -1) {
        fprintf(stderr, "Failed init event \n");
    }
    printf("The button_fd is %d \n", button_fd);

    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    printf("Current local time and date: %s", asctime(info));
    printf("Current minutes is %d:%d:%d \n", info->tm_hour, info->tm_min, info->tm_sec);


    int nfds = 2;
    struct pollfd poll_fds[nfds];

    poll_fds[1].fd = button_fd;
    poll_fds[1].events = POLLIN;
    poll_fds[0].fd = 0;
    poll_fds[0].events = POLLIN;


    while(1) {
        int ret = poll(poll_fds, nfds, -1);
        if (ret <= 0) {
            printf("Polling failed\r\n");
            exit(1); 
        }
        for (int input_fd = 0; input_fd < nfds; input_fd++) {
            if (poll_fds[input_fd].revents & POLLIN) {
                char buffer[1000]; 
                int how_much_read = read(poll_fds[input_fd].fd, buffer, 1000);
                if (input_fd == 0) {
			printf("The buffer is %s \n", buffer);
		}else  {
			printf("The button is pressed \n");
		}
            }

            if (poll_fds[input_fd].revents & POLLERR || poll_fds[input_fd].revents & POLLHUP) {


                printf("Polling failed");
                exit(1);
            }
        }
    }
}
