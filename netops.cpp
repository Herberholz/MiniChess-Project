//Cody Herberholz
//cherber2@pdx.edu
//Minichess

//The follow code is not my own work. The code can be referenced at 
//https://github.com/BartMassey/imcs/tree/master/client/c

#include "netops.h"


//
Netops::Netops()
{
    logfile = NULL;
}


//Task:
void Netops::startlog(void)
{
    logfile = fopen("deepblue.log", "w");
    if(!logfile)
    {
        perror("logfile");
        exit(0);
    }
    setlinebuf(logfile);
}


//Task:
FILE * Netops::netopen(char * host, int port)
{
    struct hostent * hp = gethostbyname(host);
    if(!hp)
    {
        herror("gethostbyname");
        exit(0);
    }
    int s = socket(hp->h_addrtype, SOCK_STREAM, 0);
    if(s == -1)
    {
        perror("socket");
        exit(0);
    }
    char ** a;
    for(a = hp->h_addr_list; *a; a++)
    {
        struct sockaddr_in sa;
        sa.sin_family = hp->h_addrtype;
        sa.sin_port = htons(port);
        memcpy(&sa.sin_addr, *a, hp->h_length);
        if(connect(s, (struct sockaddr *)&sa, sizeof(sa)) != -1)
            break;
    }
    if(!*a)
    {
        perror("connect");
        exit(0);
    }

    FILE * f = fdopen(s, "a+");
    if(!f)
    {
        perror("fdopen");
        exit(0);
    }
    setlinebuf(f);

    return f;
}


//Task:
char * Netops::expectcmd(FILE * s, int fail, ...)
{
    char * r = fgets(buf, sizeof(buf), s);
    if(r == 0 || strlen(buf) >= sizeof(buf) - 1)
    {
        perror("fgets");
        exit(0);
    }
    printf("%s", buf);
    if(logfile)
        fprintf(logfile, "%s", buf);
    va_list args;
    va_start(args, fail);
    int code;
    while((code = va_arg(args, int)) != 0)
    {
        assert(code > 0 && code <= 999);
        char cstr[4];
        sprintf(cstr, "%d", code);
        if(!strncmp(cstr, buf, 3))
        {
            va_end(args);
            return buf;
        }
    }
    va_end(args);
    if(fail)
    {
        char * error = (char *) "expect: got unexpected code";
        logmsg(error);
        exit(0);
    }
    return 0;
}


//Task:
void Netops::sendcmd(FILE * s, char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(s, fmt, args);
    fprintf(s, "\r\n");
    va_end(args);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");

}


//Task:
char * Netops::getnet(FILE * nf, char * state)
{
    char * r = fgets(buf, sizeof(buf), nf);
    if(!r || strlen(r) >= (sizeof(buf) - 1))
    {
        perror("fgets");
        fprintf(stderr, "fgets fails in state '%s'\n", state);
        exit(0);
    }
    printf("%s", r);
    if(logfile)
        fprintf(logfile, "%s", r);

    return r;
}


//Task:
void Netops::logmsg(char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    if(logfile)
    {
        va_start(args, fmt);
        vfprintf(logfile, fmt, args);
        va_end(args);
        fprintf(logfile, "\n");
    }
}


//Task:
void Netops::logchar(char ch)
{
    putchar(ch);
    if(logfile)
        fputc(ch, logfile);
}
