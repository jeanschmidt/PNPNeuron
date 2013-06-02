#ifndef __CONDIF_H__
#define __CONDIF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ConfigList {
    struct Config {
        char name[50];
        char value[50];

        Config(void) {
            name[0] = '\0';
            value[0] = '\0';
        }
    };

    Config conf;
    ConfigList *next;
 
    ConfigList(void): 
       next(NULL) 
    { }
    ConfigList(const Config &c, ConfigList *cl): 
       conf(c), next(cl) 
    { }
    ~ConfigList() { 
        if(next) 
            delete next; 
    }

    void add(const Config &c) {
        next = new ConfigList(c, next);
    }

    const Config* get(const char* name) {
        if( strcmp(conf.name, name) == 0)
            return &conf;
        if( next )
            return next->get(name);
        return NULL;
    }
};

class ConfigHolder {
    public:
       static ConfigHolder* get(void) {
           static ConfigHolder* mySelf = NULL;
           if(!mySelf)
               mySelf = new ConfigHolder();
           return mySelf;
       }
       const ConfigList::Config* get(const char* name) {
           if( !name )  return NULL;
           if( myList ) return myList->get(name);
           return NULL;
       }

       bool parseConfig(const char *file);

    protected:
       ConfigList *myList;

       ConfigHolder(void):
          myList(NULL)
       { }
       ~ConfigHolder() { }
};

#endif

