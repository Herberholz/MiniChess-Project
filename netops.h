//Cody Herberholz
//Cherber2@pdx.edu
//Minichess

//The code for the following class is not of my own work. It can be referenced at
//https://github.com/BartMassey/imcs/tree/master/client/c

#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>



//Netops class holds all the support to connect to IMCS server
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
