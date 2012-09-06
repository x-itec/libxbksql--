
#ifndef xbkqs
#define xbkqs

#include<string>
#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <sys/types.h>
#include <sys/resource.h>

/*
QS: Zeitmessungs-Klasse
wird eingebunden, wenn man #define QS benutzt

*/

#ifndef QS
#define qs_start() {}
#define qs_stop(x) {}
#define qs_stop()  {}
#else
#define qs_start() zeit.start();
#define qs_stop(x) zeit.stop(x,__FILE__,__FUNCTION__,__LINE__); 
#define qs_stop() zeit.stop("",__FILE__,__FUNCTION__,__LINE__);

struct qstime_block
{
string current_file,current_function;//GCC Makrofelder
string myvalue;//benutzerdefinierter Inhalt
int current_line;

//Sekunden waeren clock_user/CLK_TCK nicht vergessen
long clock_user;
long clock_system;

//CPU Auslastung
double cpuload[3];//1,5,15 Minuten

rusage ru_start,ru_stop;//nochmal alle Werte von rusage


};

class qstime
{
public:
int m;//Anzahl lokaler Messwerte fuer diese Funktion
qstime();
~qstime();
qstime(string fi,string function,int line);

void start();//Zeitmessung START

void stop();//Zeitmessung STOP und Result auf Vektor
void stop(string x,char* fi,char * fu,int li);
void print();//listnet Ergebnisse auf

private:
rusage ru,t_start,t_stop;

vector <qstime_block> zeitblock;
qstime_block qstime_tovector;

};
qstime::~qstime()
{
print();
puts("DESTRUKTOR!!!");
}
qstime::qstime()
{
m=0;//Messwerte auf 0 setzen
}
void qstime::start()
{
getrusage(0,&ru);
t_start=ru;
qstime_tovector.ru_start=ru;
}

void qstime::stop(string x,char * fi,char * fu,int li)
{
qstime_tovector.current_file=fi;
qstime_tovector.current_function=fu;
qstime_tovector.current_line=li;
stop();
}

void qstime::stop() 
{
getrusage(0,&ru);
t_stop=ru;
qstime_tovector.clock_user=t_stop.ru_utime.tv_usec-t_start.ru_utime.tv_usec;
qstime_tovector.clock_system=t_stop.ru_stime.tv_usec-t_start.ru_stime.tv_usec;
qstime_tovector.ru_stop=ru;

getloadavg(qstime_tovector.cpuload,3);

zeitblock.push_back(qstime_tovector);
m++;
}

void qstime::print()
{
int x;
for(x=0;x<zeitblock.size();x++)
 {
 cout << zeitblock[x].current_file << ";";
 cout << zeitblock[x].current_function << ";";
 cout << zeitblock[x].current_line << ";";
 cout << zeitblock[x].clock_user << ";";
 cout << zeitblock[x].clock_system << ";";
 cout << zeitblock[x].cpuload[0] << ";";
 cout << zeitblock[x].cpuload[1] << ";";
 cout << zeitblock[x].cpuload[2] << ";\n";
 }
}

qstime zeit;
#endif

#endif
