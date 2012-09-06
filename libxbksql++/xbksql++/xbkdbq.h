#include<string>
#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <sys/types.h>
#include <sys/resource.h>


#ifdef WITH_PGSQL
#include "/usr/local/include/libpq-fe.h"
#endif

#ifdef WITH_MYSQL
#include "/usr/local/include/mysql/mysql.h"
#endif


using namespace std;

/*
Die Ableitungen dieser Klasse dürfen ausschließlich NUR
die Umsetzung der virtuellen Basisfunktionen beinhalten, um
ein konsequentes Design zu behalten.
*/



class xbkdbq_base
{
protected:
string db_username,db_password,db_host,db_port,db_dbname;
string dberror_msg;//fuer errorstring() und connect()

public:
int currentline;//die aktuelle Zeile, muss nach query() auf 0 gesetzt werden
int numrows;//Anzahl der Zeilen der Ergebnismenge, muss von query() gesetzt werden
enum connState{cUNKNOWN,cOK,cBAD};//fuer connect()
//TODO: veraltet, muss raus - alles was mit querystate zu tun hat muss weg
enum queryState{qUNKNOWN,qOK,qBAD};//Query ok oder fehlerhaft


//enum fieldTypes(ftUNKNOWN,ftINT4,ftINT8,ftINT11,ftFLOAT,ftCHAR,ftVARCHAR,ftTEXT,ftBINARY);

connState connstate;
queryState querystate;

virtual connState connect() =0;
virtual void disconnect()=0;
virtual bool query(string text)=0;

virtual int num()=0;//gibt Anzahl der Datensaetze zurueck
virtual int numfields()=0;//Anzahl Spalten einer Tabelle 
virtual string fieldname(int field_index)=0;//gibt den Namen der Spalte field zurueck 
virtual int fieldnumber(string field_name)=0;//gibt die Nummer des Feldes zurueck, -1 wenn Feld nicht existiert
virtual string value(string field_name)=0;//gibt den Inhalt des Feldes zurueck
virtual int next()=0;//springt zum naechsten Datensatz, -1 wenn Ende sonst akt. Position zurueckgeben 
virtual bool eof()=0;//gibt true zurueck wenn currentline>=numrows 

//virtual int size(field_name)=0;//Gesamtlaenge des Feldinhaltes
virtual string errorstring()=0;
//Parameter setzen
void username(string username);
void password(string password);
void host(string host);
void port(string port);
void dbname(string db);
};


#ifdef WITH_PGSQL


class xbkdbq_pgsql : public xbkdbq_base
{
private:
PGconn     *conn; PGresult   *res;
public:
 ~xbkdbq_pgsql();
 connState connect();
 bool query(string text);//false=fehler
 int num();
 int numfields();
 string fieldname(int field_index);
 int fieldnumber(string field_name);
 string value(string field_name);
 int next();
 bool eof();
 string errorstring();
 void username(string username);
 void password(string password);
 void host(string host);
 void port(string port);
 void dbname(string db); 
 void disconnect();
} ;

#endif

#ifdef WITH_MYSQL

class xbkdbq_mysql : public xbkdbq_base
{
private:
MYSQL	   *conn; 
MYSQL_RES  *res;
MYSQL_ROW  row;

MYSQL_FIELD *sql_fields;//Namen der Felder, nach query
unsigned int sql_num_fields;//Anzahl der Felder

public:
 ~xbkdbq_mysql();
 connState connect();
 bool query(string text);
 int num();
 int numfields();
 string fieldname(int field_index);
 int fieldnumber(string field_name);
 string value(string field_name);
 int next();
 bool eof();
 string errorstring();
 void username(string username);
 void password(string password);
 void host(string host);
 void port(string port);
 void dbname(string db);
 void disconnect();
} ;
#endif 

