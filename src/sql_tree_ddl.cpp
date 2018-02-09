

#include "connstr.h"
#include "sql_tree.h"
#include "dbug.h"
#include <assert.h>

using namespace STREE_TYPES;
using namespace LEX_GLOBAL;


/* parse keyword in 'show xxx' list */
stxNode* sql_tree::parse_show_keys(
  char *tkn, int &p)
{
  int st = s_none; 

  if (!strcasecmp(tkn,"engine")) {
    st = s_engine ;
  } else if (!strcasecmp(tkn,"engines")) {
    st = s_engines ;
  } else if (!strcasecmp(tkn,"status")) {
    st = s_status ;
  } else if (!strcasecmp(tkn,"mutex")) {
    st = s_mutex ;
  } else if (!strcasecmp(tkn,"global")) {
    st = s_global ;
  } else if (!strcasecmp(tkn,"variables")) {
    st = s_vars ;
  } else if (!strcasecmp(tkn,"errors")) {
    st = s_err ;
  } else if (!strcasecmp(tkn,"warnings")) {
    st = s_warn ;
  } else if (!strcasecmp(tkn,"processlist")) {
    st = s_proclst ;
  } else {
    //printd("unknown keyword '%s' in 'order by' stmt\n", tkn);
    return NULL ;
  }
  mov(p,tkn);

  return create_node(NULL,m_keyw,st);
}

/* parse keyword in 'drop table' list */
stxNode* sql_tree::parse_dtbl_keys(
  char *tkn, int &p)
{
  int st = s_none; 

  if (!strcasecmp(tkn,"cascade")) {
    st = s_cascade ;
    mov(p,tkn);
  } else if (!strcasecmp(tkn,"restrict")) {
    st = s_restrict ;
    mov(p,tkn);
  } else {
    //printd("unknown keyword '%s' in 'order by' stmt\n", tkn);
    return NULL ;
  }
  return create_node(NULL,m_keyw,st);
}

int sql_tree::parse_create_tbl_additions(stxNode *parent, int &pos)
{
  std::string &s = sql_stmt ;

  while (pos<(int)s.size()) {

    stxNode *p = parse_list_item(pos);

    attach(parent,p);
  }
  return 0;
}

int sql_tree::parse_create_stmt(stxNode *parent, int &p)
{
  char buf[TKNLEN] = "";
  std::string &s = sql_stmt;
  
  p = next_token(s,p,buf);
  mov(p,buf);
  /* 'create table' command  */
  if (!strcasecmp(buf,"table")) {
    /* set parent node type */
    parent->type = mktype(m_stmt,s_cTbl);

    /* it's 'if not exists' */
    p = next_token(s,p,buf);
    if (!strcasecmp(buf,"if")) {
      /* skip 'if' */
      mov(p,buf);
      /* skip 'not' */
      p = next_token(s,p,buf);
      assert(!strcasecmp(buf,"not"));
      mov(p,buf);
      /* skip 'exists' */
      p = next_token(s,p,buf);
      assert(!strcasecmp(buf,"exists"));
      mov(p,buf);

      /* change parent's type: 
       *  'create table' -> 'create table if not exists' 
       */
      sset(parent->type,s_cTbl_cond);
    }

    /* table name */
    p = next_token(s,p,parent->name);
    mov(p,parent->name);

    /* skip '(' */
    p = next_token(s,p,buf);
    if (*buf=='(') {
      mov(p,buf);
      /* force to deal with the 'create' list */
      fset(of_ci,ci_ct);
      parse_list(parent,s_cd_lst,p);
      fset(of_ci,ci_all);

      /* skip ')' */
      p = next_token(s,p,buf);
      mov(p,buf);

      /* parse addition items after create list, 
       *  like 'engine', character, etc */
      parse_create_tbl_additions(parent,p);

    } 
    /* create table with 'select' */
    else if (!strcasecmp(buf,"select")) {
      stxNode *nd = parse_stmt(p);

      attach(parent,nd);
    }

  }
  return 1;
}

int sql_tree::parse_drop_stmt(stxNode *parent, int &p)
{
  char buf[TKNLEN] = "";
  std::string &s = sql_stmt;
  
  p = next_token(s,p,buf);
  mov(p,buf);
  /* 'drop table' command  */
  if (!strcasecmp(buf,"table")) {
    parent->type = mktype(m_stmt,s_dTbl);

    /* it's 'if exists' */
    p = next_token(s,p,buf);
    if (!strcasecmp(buf,"if")) {
      /* skip 'if' */
      mov(p,buf);
      /* skip 'exists' */
      p = next_token(s,p,buf);
      assert(!strcasecmp(buf,"exists"));
      mov(p,buf);

      /* change parent's type: 
       *  'drop table' -> 'drop table if xists' 
       */
      sset(parent->type,s_dTbl_cond);
    }

    /* parse drop list */
    parse_list(parent,s_dTbl_lst,p);
  }
  return 0;
}
