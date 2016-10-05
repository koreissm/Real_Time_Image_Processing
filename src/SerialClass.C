#include "SerialClass.h"

#include <sys/ioctl.h>


int serialport_init(const char* serialport, int baud)
{
        struct termios toptions;
        int fd;
                
        fd = open(serialport, O_RDWR | O_NONBLOCK );
                    
        if (fd == -1)  {
            perror("serialport_init: Unable to open port ");
            return -1;
        }
                        
        speed_t brate = baud;
        if (tcgetattr(fd, &toptions) < 0) {
            perror("serialport_init: Couldn't get term attributes");
            return -1;
        }
        cfsetispeed(&toptions, brate);
        cfsetospeed(&toptions, brate);

        toptions.c_cflag &= ~PARENB;
        toptions.c_cflag &= ~CSTOPB;
        toptions.c_cflag &= ~CSIZE;
        toptions.c_cflag |= CS8;
        toptions.c_cflag &= ~CRTSCTS;




        toptions.c_cc[VMIN]  = 0;
        toptions.c_cc[VTIME] = 0;
                                                                    
        tcsetattr(fd, TCSANOW, &toptions);
        if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
            perror("init_serialport: Couldn't set term attributes");
            return -1;
        }

        return fd;
}

int serialport_close( int fd )
{
        return close( fd );
}

int serialport_writebyte( int fd, uint8_t b)
{
        int n = write(fd,&b,1);
            if( n!=1)
                        return -1;
                return 0;
}

int serialport_write(int fd, const char* str)
{
        int len = strlen(str);
            int n = write(fd, str, len);
                if( n!=len ) {
                            perror("serialport_write: couldn't write whole string\n");
                                    return -1;
                                        }
                    return 0;
}

int serialport_read_until(int fd, char* buf, char until, int buf_max, int timeout)
{
    char b[1];
        int i=0;
            do { 
                int n = read(fd, b, 1);  // read a char at a time
                        if( n==-1) return -1;    // couldn't read
                        if( n==0 ) {
                                        timeout--;
                                                    if( timeout==0 ) return -2;
                                                                continue;
                                                                        }
#ifdef SERIALPORTDEBUG  
#endif
                                buf[i] = b[0]; 
                                        i++;
                                            } while( b[0] != until && i < buf_max && timeout>0 );
buf[i] = 0;
                return 0;
}

int serialport_flush(int fd)
{
    sleep(2);
        return tcflush(fd, TCIOFLUSH);
}
