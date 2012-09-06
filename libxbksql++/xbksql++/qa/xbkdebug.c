#ifndef DEBUGME_
#define DEBUGME_

#include<stdio.h>
#include <stdlib.h>
/*

Verbesserungen an dieser Unit nur erlaubt
in /usr/x-itec/xbkdebug/
    



Debugging-Makros
(C) X-ITEC Boris Koester 2001,2002
www.x-itec.de

Anwendungsbeispiel:
#include <xbkdebug.c>

//Folgendes setzen, sonst wird ueberhaupt kein Debug-Log erzeugt
//und es kommt auch nicht zu irgendwelchem Datenmuell
#define DEBUGME

//Debugging an verschiedenen Stellen einschaltbar:

EDEBUG           
DEBUG("Hier ist Debugging-Text");
DEBUG("Sogar mit Parameter %i",x);
DDEBUG

*/
#ifdef DEBUGME 

static int debugme;
static int debugme_line=0;
#define EDEBUG debugme=1;
#define DDEBUG debugme=0;

#define DEBUG(format, args...) {if(debugme==1){FILE *f; f=fopen("debug.log","a+"); fprintf(f,"%i:DEBUG:%s:%s[%i]:",debugme_line++,__FILE__,__FUNCTION__,__LINE__);fprintf(f,format, ## args);fprintf(f,"\n");fclose(f);}}
#else  
#define DEBUG(format, args...) {}
#define EDEBUG {}
#define DDEBUG {}
#endif 


#ifdef ERR_DEBUGME
static int err_debugme;
static int err_debugme_line=0;
#define ERR_EDEBUG err_debugme=1;
#define ERR_DDEBUG err_debugme=0;
#define ERR_DEBUG(format, args...) {if(err_debugme==1){FILE *f; f=fopen("debug.log","a+"); fprintf(f,"%i:ERROR:%s:%s[%i]:",err_debugme_line++,__FILE__,__FUNCTION__,__LINE__);fprintf(f,format, ## args);fprintf(f,"\n");fclose(f); }}

#else
#define ERR_EDEBUG {}
#define ERR_DDEBUG {}
#define ERR_DEBUG (format, args...) {}

#endif






//DEBUGME_
#endif

