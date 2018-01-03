
## What is ZAS
 - ZAS（Zhou’s Adaptor of Sql）is being developed based on the `MYSQL Client/Server Protocol`
 - ZAS is a C++ library that used by applications to access MYSQL databases
 - ZAS can translate SQL syntax automatically 


## Features
 - Cross platforms: 32/64 bit processors supports, linux and windows supports
 - Cross languages: you may use ZAS with c++, java, python
 - Provides OTLV4-compatible interfaces
 - Provides automatically SQL syntax translations that is transparent to upper applications
 - Supports most SQL syntaxs in ORACLE/MYSQL 
 - Good performance in SQL syntax analyzing
 - The underlying `MYSQL Client/Server Protocol` is being optimized
 - Conceals complexities of `MYSQL Client/Server Protocol`
 - Good extensibility
 - Being tested over a year and runs steadily


## Structure

 ![Alt text](https://github.com/oun111/images/blob/master/zas_structure.png)

 - Connection String processor: processes the DB connection strings including Oracle TNS and traditional DSN format
 - Protocol Management(the `MYSQLC` libray) provides:
    1. implementations of the `MYSQL Client/Server Protocol`
    2. fully support for the `prepare` and `query` mode
    3. API compatibilities with `libmysqlclient`
 - SQL Syntax Engine: performs SQL syntax analyzing/checking/translations
 - OTLV4 Compatible API: provides a set of c++ classes that compatible with OTLV4 libraries


## Process Flow

 ![Alt text](https://github.com/oun111/images/blob/master/zas_process_flow.png)

 - First of all, the application should call `rlogon() ` to parse connection strings and to do database login 
 - Second, the application should call `open()` to initialize the SQL and translate it
 - Third, the application should input enough place holder values to execute the statement
 - Last, fetching the result
 - Further more, one can `reopen` a new SQL or `execute` the old one again
 
 
## Connection String Handling

 ![Alt text](https://github.com/oun111/images/blob/master/zas_conn_str.png)

 - When login, the connection string is passed to the `connection string processor` which will test  the inputed string
    1. if it’s TNS format, the processor will parse the TNS file for login informations
    2. if it’s DSN format, the processor will parse it directly
    3. otherwise, throws C++ exception 


## the SQL Syntax Engine

 ![Alt text](https://github.com/oun111/images/blob/master/zas_syntax_engine.png)

 - when a SQL is inputed, it would be processed by a hard-coding  parser and be translated to a syntax tree
 - if no errors, the tree will be scaned for syntax errors
 - if everythings’ ok, the tree will be scaned again for `translation points` to being translated to `MYSQL-style` syntax
 - then, all place-holders within the tree will be processed
 - last, the tree will be serialized to string form and passed to output


## The MYSQLC library

 - The library provides the following functionalities: database login, SQL execution, result fetching, protocol compression, connection maintainance
 - The protocol compression invokes the `deflate` algorithm from zlib 
 - The library employs the `ping` protocol to maintain the database connection
 - To execute a SQL statement, the application may use `query mode` with which all place holder values are embeded within the SQL statement string that will be sent to server by a single `com_query` request, here’s the diagram:

 ![Alt text](https://github.com/oun111/images/blob/master/zas_query.png)


 - The application may also execute SQL under `prepare mode`. The SQL execution progress is seperated into 2 requests:
    1. the primordial SQL statement is sent to server by `com_stmt_prepare` to prepare resources and do checkings at server side
    2. the place-holder value list are embeded into the `com_stmt_execute` request to initiate the `SQL execution` progress, and the `binary/string/blob` type place-holders values should be sent by `com_send_long_data` requests especially,  Here’s the diagram:

 ![Alt text](https://github.com/oun111/images/blob/master/zas_prepare.png)


## Compiling and Installing

 - Simply do `make clean install` under source root directory
 - The `libzas.a`, `libzas.so`, and `zas wrapper` library will be generated
 - Link the `.a` or `.so` library to your c++ applications
 - Load the wrapper library into your java/python applications
 
 
## Roadmap
 * `src`: core of zas library
 * `tests`: test cases of zas
 * `win`: vss project files of zas under windows
 * `wrapper`: library classes for java/python that encapsulates APIs onto ZAS 
 
 
## Wrappers
 * both java and python wrappers load a c library named `libcwpr` that encapsulates a set of usage with ZAS APIs. see wrapper/cwpr.c for more details
 * in java wrapper, it calls `libcwpr` methods with JNI interfaces
 * in python wrapper, it dynamically loads the `libcwpr`
 
 
## HOWTO

 ### `In c++, one should access MYSQL with ZAS like this:`
 
```c++
 /* initialize connection object and login to database with MYSQL driver */
 zas_connect cnn(tnsFilePath,dal_mysql) ;
 /* initialize stream object with SQL-prepare mode */
 zas_stream stream(cnn,true);
 
 /* initialize the ORACLE-style SQL */
 stream.open(0,"select id,nvl(name),price from test_db.test_tbl "
        "where id>=:f1<unsigned int> and id<:f2<int,in>");
 
 /* insert place holders and execute the SQL */
 streams<<1;
 streams<<5;
 
 /* fetch results */
 while (!streams[strs].eof()) {
  int id=0;
  char name[256] = "";
  float point = 0.0;
  long size = 0L;
  
  streams>>id ;
  streams>>name ;
  streams>>point ;
  streams>>size ;
  streams.flush();
  printd("%d: name %s, point %f, size %ld\n",
    id, name, point, size);
}
```

### `In java, one should set correct 'CLASSPATH' of ZAS java wrapper class and play like this:`

```java
public class test_cases {

  public static void main(String[] args) throws Exception {

    /* initialize instance of zas.class and do initializations with it */
    zas mz = (zas)Class.forName("zas").newInstance();

    /* login to MYSQL */
    mz.login("localhost",3306,"root","123","");

    /* initialize the ORACLE-style SQL */
    mz.prepare("select id,nvl(name),price,size from test_db.test_tbl where id<:f1<int>");

    /* insert placeholder and execute SQL */
    mz.insertInt(10);

    /* fetch results */
    while (!mz.isEof()) {
      Integer id = 0;
      String name = "" ;
      Double price = 0.0;
      Long size = 0L; 

      id = mz.fetchInt();
      name = mz.fetchStr();
      price = mz.fetchDouble();
      size = mz.fetchLong();
      System.out.println(id + ": name: " + name + ", point: " + 
        price + ", size: " + size + "\n");
    }   

  }
}

```

### `In python, one should load ZAS python wrapper class and play like this:`

```python

# path of ZAS python wrapper class
sys.path.append(zasPythonWrapperPath)

# load the wrapper class
from python.zas import *

def main():

  # do initializations
  mz = zas()

  # login to database
  if mz.login("localhost",3306,"root","123","")!=0 :
    print("login fail\n")
    exit(-1)

  # initialize the ORACLE-style SQL
  if mz.prepare('select nvl(id),name,price,size from test_db.test_tbl where id <:f1<int>')!=0 :
    print("prepare fail\n")
    exit(-1)

  # insert placeholder and execute SQL
  if mz.insert_int(10)!=0 :
    exit(-1)

  # fetch results
  while mz.is_eof()==0 :
    id = mz.fetch_int()
    name = mz.fetch_str()
    price = mz.fetch_double()
    size = mz.fetch_long()
    print("{0}: name: {1}, point: {2}, size: {3}\n".format(id,name,price,size))

```
