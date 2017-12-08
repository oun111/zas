# ZAS

ZAS is a database client library(currently for MYSQL), with which, you may access a database that are unfamiliar to you with another SQL language that you're fluent in it. 

## Feature
 * do sql language adaptions: oracle -> mysql
 * database access tool
 * sql syntax analyze and checking
 * cross platforms: linux, windows
 * cross languages: c++, java, python

## Install

 * link zas library to your c++ applications
 * load zas library into your java/python applications
 
## Roadmap
 * `src`: source code of zas library
 * `tests`: test cases of zas
 * `win`: project files of zas under windows
 * `wrapper`: wrapper library for java/python
 
## HOWTO

 * in c++, you may access MYSQL with ZAS like this:
 
 '''
 /* initialize connection object with MYSQL database driver */
 zas_connect cnn(tnsFilePath,dal_mysql) ;
 /* initialize stream object with SQL-prepare mode */
 zas_stream stream(cnn,true);
 
 /* initialize the ORACLE-style SQL */
 stream.open(0,"select id,`nvl(name)`,price from test_db.test_tbl "
        "where id>=:f1<unsigned int> and id<:f2<int,in>");
 
 /* insert place holders and execute the SQL */
 streams<<1;
 streams<<5;
 
 /* fetch results */
 while (!streams[strs].eof()) {
  streams>>id ;
  streams>>name ;
  streams>>point ;
  streams>>size ;
  streams.flush();
  printd("%d: name %s, point %f, size %ld\n",
    id, name, point, size);
}
 '''
