/****************************************************************************
 * A small program that enables the keypad input without pressing enter which 
 * can be used in a smaller robotic projects for wireless interface and motion 
 * with keypad.

  -Rahul Sharma 
  -Inkers Technology Pvt Ltd
****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
struct termios oldtermios;

//Important commands referenced from http://man7.org/linux/man-pages/man3/termios.3.html
int ttyraw(int fd)
{
    struct termios newtermios;
    if(tcgetattr(fd, &oldtermios) < 0)
        return(-1);
    newtermios = oldtermios;
    newtermios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    newtermios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    newtermios.c_cflag &= ~(CSIZE | PARENB);
    newtermios.c_cflag |= CS8;
    /* Set 8 bits per character. */
    newtermios.c_oflag &= ~(OPOST);
    /* This includes things like expanding tabs to spaces. */
    newtermios.c_cc[VMIN] = 1;
    newtermios.c_cc[VTIME] = 0;
    /* The change occurs after all output written to the file descriptor has been transmitted, and all input so far received but not read is discarded before the change is made. */
    if(tcsetattr(fd, TCSAFLUSH, &newtermios) < 0)
        return(-1);
    return(0);
}

int ttyreset(int fd)
{
    if(tcsetattr(fd, TCSAFLUSH, &oldtermios) < 0)
        return(-1);
    return(0);
}

void sigcatch(int sig)
{
    ttyreset(0);
    exit(0);
}

int i;
char c;

int main()
{
	if(ttyraw(0) < 0)
    	{
        	fprintf(stderr,"Can't go to raw mode.\n");
        	exit(1);
    	}

	while(1){
                //keypad input
                if((i=read(0, &c, 1)) == 1){ 
                        if((c&=255)==3){
				break;
                        }
			else if((c&=255)=='a'){
				printf("Automode Enabled\n\r");
			}
			else if((c&=255)=='m'){
				printf("Manual Mode Enabled\n\r");
			}
	                else if((c&=255)==65){ //up arrow key
        	                printf("Forward\n\r");
        	        }
        	        else if((c&=255)==66){ //back arrow key
        	                printf("backword\n\r");
        	        }
        	        else if((c&=255)==68){ //left arrow key
        	                printf("Left\n\r");
        	        }
        	        else if((c&=255)==67){ //right arrow key
        	                printf("Right\n\r");
        	        }
		}
        }
        if(ttyreset(0) < 0)
         {
                fprintf(stderr, "Cannot reset terminal!\n");
                exit(-1);
        }

        if( i < 0)
        {
                fprintf(stderr,"Read error.\n");
                exit(-1);

        }

	printf("Exit Keyboard interface\n");
	return 0;
}
