# ZAS

ZAS is a database client library(currently for MYSQL), with which, you may access a database that are unfamiliar to you with another SQL language that you're fluent in it. 

## Feature
 * do sql language adaptions, we support oracle to mysql currently
 * database access tool
 * sql syntax analyze and checking
 * cross platforms usages: linux, windows
 * you may use ZAS with c++, java, python

## Install

 * link zas library to your c++ applications
 * load zas library into your java/python applications
 
## Roadmap
 * `src`: source code of zas library
 * `tests`: test cases of zas
 * `win`: project files of zas under windows
 * `wrapper`: wrapper library for java/python
 
## HOWTO

 ### in c++, you may access MYSQL with ZAS like this:
 
```
 /* initialize connection object and login to database with MYSQL driver */
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
```

### in java, one should load the ZAS java library and play like this:

```
public class test_cases {

  public static void main(String[] args) throws Exception {

    /* load zas wrapper class and do initializations with it */
    zas mz = (zas)Class.forName("zas").newInstance();

    /* login to MYSQL */
    mz.login("localhost",3306,"root","123","");

    /* initialize the SQL */
    mz.prepare("select id,nvl(name),price,size from test_db.test_tbl where id<:f1<int>");

    while (true) {

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
}

```

### in python, one should load the ZAS python library and play like this:

```
# load zas wrapper class
sys.path.append("/mnt/sda5/zyw/work/zas/wrapper")

from python.zas import *

def main():

  # do initializations
  mz = zas()

  # login to database
  if mz.login("localhost",3306,"root","123","")!=0 :
    print("login fail\n")
    exit(-1)

  # initialize the SQL
  if mz.prepare('select *from test_db.test_tbl where id <:f1<int>')!=0 :
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
