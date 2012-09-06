#include<string>
#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <sys/types.h>
#include <sys/resource.h>

#include <iostream>
#include <iomanip>

#define WITH_PGSQL

#include "xbkdbq.h"

//Standard-Logging
#define DEBUGME
//error.log logging bei schweren Problemen:
#define ERR_DEBUGME
#include "qa/xbkdebug.c"


/*

Folgende PostgreSQL Funktionen muessen geprueft werden,
inwieweit sie mit MySQL konform sind damit man 
entscheiden kann ob man das in die Basisklasse packen kann:

---------------------------------------------
PQbinaryTuples Returns 1 if the PGresult contains binary tuple data, 0 if it contains ASCII data. Currently, binary tuple data can only be returned by a query that extracts data from a BINARY cursor.

int PQbinaryTuples(const PGresult *res);
---------------------------------------------


*/

#ifdef WITH_MYSQL

xbkdbq_mysql::~xbkdbq_mysql()
{
disconnect();
}
#endif


#ifdef WITH_PGSQL

xbkdbq_pgsql::~xbkdbq_pgsql()
{
disconnect();
}
#endif

#ifdef WITH_MYSQL
void xbkdbq_mysql::disconnect()
{
//keine Implementation gefunden.  
}
#endif


#ifdef WITH_PGSQL
void xbkdbq_pgsql::disconnect()
{
  if(res){PQclear(res);}
  if(conn){PQfinish(conn);}
}
#endif

void xbkdbq_base::username(string username)
{
db_username=username;
}
void xbkdbq_base::password(string password)
{
db_password=password;
}
void xbkdbq_base::host(string host)
{
db_host=host;
}
void xbkdbq_base::port(string port)
{
db_port=port;
}
void xbkdbq_base::dbname(string db)
{
db_dbname=db;
}

#ifdef WITH_MYSQL
string xbkdbq_mysql::errorstring()
{
return dberror_msg;//wird durch eine Exception beschrieben 
}
#endif


#ifdef WITH_PGSQL
string xbkdbq_pgsql::errorstring()
{
return dberror_msg;
}
#endif


#ifdef WITH_MYSQL
inline int xbkdbq_mysql::num()
{
//longlong ?
if(res){ return (numrows=mysql_num_rows(res));}
else{return -1;}

}
#endif

#ifdef WITH_PGSQL

inline int xbkdbq_pgsql::num()
{
if(res){ return (numrows=PQntuples(res));}
else{return -1;}

/* Unter PostgreSQL werden die Anzahl der Datensaetze auch Tupel genannt */
}
#endif


#ifdef WITH_MYSQL
inline bool xbkdbq_mysql::eof()
{
if(!row){DEBUG("ROW ist leer");return true;}else{return false;};

}
#endif

#ifdef WITH_PGSQL

inline bool xbkdbq_pgsql::eof()
{
if(res){ return (currentline<numrows) ? false:true;}
else{return true;}
}
#endif

#ifdef WITH_MYSQL
inline int xbkdbq_mysql::numfields()
{
return mysql_num_fields(res);
}
#endif


#ifdef WITH_PGSQL
inline int xbkdbq_pgsql::numfields()
{
return PQnfields(res);
}
#endif

#ifdef WITH_MYSQL
inline string xbkdbq_mysql::fieldname(int field_index)
{
MYSQL_FIELD *field;
string r;

int x=0;
while( (field = mysql_fetch_field(res)) && x<=field_index  )
{
//    printf("field name %s\n", field->name);
}
if(field->name && x==field_index){return field->name;}else{return "";}
}
#endif


#ifdef WITH_PGSQL
inline string xbkdbq_pgsql::fieldname(int field_index)
{
char *name;
name=PQfname(res,field_index);
if(name){return name;}else{return "";}
}
#endif

#ifdef WITH_MYSQL
inline int xbkdbq_mysql::fieldnumber(string field_index)
{
//noch keine Definition gefunden
}

#endif

#ifdef WITH_PGSQL
inline int xbkdbq_pgsql::fieldnumber(string field_name)
{
return PQfnumber(res, field_name.c_str());
}
#endif

#ifdef WITH_MYSQL
inline string xbkdbq_mysql::value(string field_name)
{
unsigned long *lengths;
unsigned int feldcounter=0;

//hier stehen die Längen aller Feldinhalte drin
lengths = mysql_fetch_lengths(res);

if(!lengths){return "";}

//Feldnamen ermitteln, bzw. den Index
for(feldcounter=0;feldcounter<=sql_num_fields;feldcounter++)
	{
	DEBUG("Feld %s/%s/%i",sql_fields[feldcounter].name,field_name.c_str(),strcmp(sql_fields[feldcounter].name,field_name.c_str()));
	if (strcmp(sql_fields[feldcounter].name,field_name.c_str())==0 )
		{
		DEBUG("Scheint das richtige Feld zu sein.");
		DEBUG("Die Ergebnismenge ist lang: %i bytes",lengths[feldcounter]);
		
		string rueck(lengths[feldcounter],0);
		char *contentzeiger=row[feldcounter];
		for(long c=0;c<=lengths[feldcounter];c++)
			{
			rueck[c]=*contentzeiger++;
			}
		//wir springen einfach raus
		return rueck;
		
		}
	}

//wenn wir nichts gefunden haben, werden wir mal nichts zurückgeben
//oder sagen wir mal "leer" um unnötige Exceptions zu vermeiden

return "";
}
#endif


#ifdef WITH_PGSQL
inline string xbkdbq_pgsql::value(string field_name)
{
return PQgetvalue(res, currentline,fieldnumber(field_name));
}
#endif

#ifdef WITH_MYSQL
inline int xbkdbq_mysql::next()
{

/*
Return Values

A MYSQL_ROW structure for the next row.  NULL if there are no more rows to retrieve or if an error occurred.
Errors
CR_SERVER_LOST
The connection to the server was lost during the query.
CR_UNKNOWN_ERROR
An unknown error occurred.

PS: Für den Server_Lost ist MySQL ja bekannt
*/  

if(res)
{
row = mysql_fetch_row(res);if(row){++currentline;}else{currentline=-1;}
}
else{currentline=-1;}
return currentline;
}
#endif

#ifdef WITH_PGSQL
inline int xbkdbq_pgsql::next()
{
if(res){

if(++currentline>numrows){currentline=-1;}
}
else{currentline=-1;}

return currentline;
}
#endif


#ifdef WITH_MYSQL

bool xbkdbq_mysql::query(string text)
{
currentline=0;
res=NULL;
if(connstate!=cOK){connect();}
if(conn)
	{
		//0=ok, sonst fehler
		//wenn ! = NULL ist dann ist !my.. = ok
		if(!mysql_real_query(conn, text.c_str(), text.length()))
		{
			res = mysql_store_result(conn);
			//Felddefinitionen festhalten
			sql_num_fields = mysql_num_fields(res);
			sql_fields = mysql_fetch_fields(res);
			//die erste Row müssen wir schon einlesen
			next();
	
		}
		else
		{
		//Query Fehler
		dberror_msg=mysql_error(conn);		
		}
	}
if(res){return true;}else{return false;}
}
#endif


#ifdef WITH_PGSQL

bool xbkdbq_pgsql::query(string text)
{
currentline=0;
if(connstate!=cOK){connect();}
if(res){PQclear(res);}

ExecStatusType res_status;

 res        = PQexec(conn, text.c_str()); 
 res_status = PQresultStatus(res);
 dberror_msg= PQresultErrorMessage(res);

/*
 numrows ?
 das muss bei PGSQL so sein sonst wissen wir nicht wann die Query zu Ende ist?
 ich denke das muss nochmal geprüft werden. Das dumme bei PGSQL ist ja, man
 greift auf eine Datensatznummer zu. Man muss natürlich dann wissen, ob man
 am Ende ist. Andererseits braucht man ja das Resultset nur auf NULL zu testen.
 jedenfalls muss geprüft werden, ob das notwendig ist. wenn nein, müssen
 ein paar pgsql routinen hier daraufhin angepasst werden
*/
 numrows    = num();
 //DEBUG("Query: %s",text.c_str());
 //DEBUG("Status: %s",dberror_msg.c_str());

/*
PGRES_EMPTY_QUERY -- The string sent to the backend was empty.
PGRES_COMMAND_OK -- Successful completion of a command returning no data
PGRES_TUPLES_OK -- The query successfully executed
PGRES_COPY_OUT -- Copy Out (from server) data transfer started
PGRES_COPY_IN -- Copy In (to server) data transfer started
PGRES_BAD_RESPONSE -- The server's response was not understood
PGRES_NONFATAL_ERROR
PGRES_FATAL_ERROR
*/

if( !res || (res_status != PGRES_TUPLES_OK && res_status !=PGRES_COMMAND_OK) ) 
 { return false;} else return true; 
}

#endif

#ifdef WITH_MYSQL

xbkdbq_base::connState xbkdbq_mysql::connect()
{
/*
MYSQL *mysql_real_connect(MYSQL *mysql, const char *host,                           const char *user, const char *passwd, const char *db,                           unsigned int port, const char *unix_socket,                           unsigned int client_flag)

Flag name	Flag description
CLIENT_COMPRESS	Use compression protocol.
CLIENT_FOUND_ROWS	Return the number of found (matched) rows, not the number of affected rows.
CLIENT_IGNORE_SPACE	Allow spaces after function names. Makes all functions names reserved words.
CLIENT_INTERACTIVE	Allow interactive_timeout seconds (instead of wait_timeout seconds) of inactivity before closing the connection.
CLIENT_NO_SCHEMA	Don't allow the db_name.tbl_name.col_name syntax.  This is for ODBC. It causes the parser to generate an error if you use that syntax, which is useful for trapping bugs in some ODBC programs.
CLIENT_ODBC	The client is an ODBC client. This changes mysqld to be more ODBC-friendly.
CLIENT_SSL	

Return Values

A MYSQL* connection handle if the connection was successful, NULL if the connection was unsuccessful.  For a successful connection, the return value is the same as the value of the first parameter.

*/
EDEBUG
ERR_EDEBUG

DEBUG("mysql_init()");
conn=mysql_init(conn);

MYSQL *conn_dummy=conn;


if(!conn){DEBUG("mysql_init mem-alloc error!=!");};
DEBUG(".");
DEBUG("mysql_options()");
mysql_options(conn,MYSQL_READ_DEFAULT_GROUP,"your_prog_name");
DEBUG(".");

if(db_host==""){db_host="localhost";}
if(db_port==""){db_port="3306";}
DEBUG(".");
DEBUG("db_host: %s",db_host.c_str());
DEBUG("db_user: %s",db_username.c_str());
DEBUG("db_dbname: %s",db_dbname.c_str());
DEBUG("db_port: %i",atoi(db_port.c_str()));

DEBUG("mysql_real_connect(...)");
conn = mysql_real_connect(conn, db_host.c_str(), db_username.c_str(),db_password.c_str(),db_dbname.c_str(), atoi(db_port.c_str()), NULL,0);
DEBUG("mysql_real_connect() done.");

if(conn){connstate=cOK;}
if(!conn)
  {
   DEBUG("conn failure");
	connstate=cBAD;dberror_msg=mysql_error(conn_dummy);
   ERR_DEBUG("mysql: %s",dberror_msg.c_str());
}
return connstate; 
}
#endif


#ifdef WITH_PGSQL
xbkdbq_base::connState xbkdbq_pgsql::connect()
{
if(db_port==""){db_port="5432";}
//                                               options,pgtty
conn = PQsetdbLogin(db_host.c_str(), db_port.c_str(), NULL, NULL, db_dbname.c_str(),db_username.c_str(),db_password.c_str());

/*
PGconn *PQsetdbLogin(const char *pghost,
                     const char *pgport,
                     const char *pgoptions,
                     const char *pgtty,
                     const char *dbName,
                     const char *login,
                     const char *pwd)
*/


 if (PQstatus(conn) == CONNECTION_BAD)
 {
 connstate=cBAD;
 dberror_msg=PQerrorMessage(conn);
 //DEBUG("Connection to database '%s' failed.\n", db_dbname.c_str());
 DEBUG(PQerrorMessage(conn));
 }
 else
 {
 connstate=cOK;
 }
return connstate;
}
#endif

