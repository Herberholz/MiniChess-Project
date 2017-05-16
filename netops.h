//Cody Herberholz
//Cherber2@pdx.edu
//Minichess

#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>



//
class Netops
{
    public:
        Netops();
        void startlog(void);
        FILE * netopen(char * host, int port);
        char * expectcmd(FILE * s, int fail, ...);
        void sendcmd(FILE * s, char * fmt, ...);
        char * getnet(FILE * f, char * state);
        void logmsg(char * fmt, ...);
        void logchar(char ch);

    private:
        FILE * logfile;
        char buf[1024];
};
