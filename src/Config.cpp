#include "Config.h"

bool ConfigHolder::parseConfig(const char *file) {
    if(!file) {
        printf("    #No filename provided!\n");
        return false;
    }

    FILE *arq = fopen(file, "r");
    if(!arq) {
        printf("    #Can't open file %s!\n", file);
        return false;
    }

    while( !feof(arq) ) {
        char buffer[128];
        char *r = fgets(buffer, sizeof(buffer), arq);

        if( r == NULL ) {
            break;
        }

        int equal=-1, end=1;
	for(int a=0; buffer[a] != '\0' && a<sizeof(buffer); a++) {
            if(buffer[a] == '=') {
                if(equal > 0) {
                    printf("    #Two '=' on line near %s\n", buffer);
                    return false;
                }
                equal = a;
            }
            if(buffer[a] == '\n') {
                end = a;
                break;
            }
        }

        if( equal < 1 ) {
            printf("    #Empty variable on line near %s\n", buffer);
            return false;
        }
        if( end < equal ) {
            printf("    #No '\\n' on end of line near %s (or line exeeds 127b\n", buffer);
            return false;
        } 

        if( 
           equal > (sizeof(ConfigList::Config::name)-1) 
           || (end - equal - 1) > (sizeof(ConfigList::Config::value)-1)
        ) {
            printf("    #No enought space to fit variables on line %s\n", buffer);
        }

        ConfigList::Config cfg;

        memcpy(cfg.name, buffer, equal);
        cfg.name[equal] = '\0';
        memcpy(cfg.value, buffer+equal+1, (end-equal)-1);
        cfg.value[ (end-equal)-1 ] = '\0';        
     
	if(!myList) {
            myList = new ConfigList(cfg, NULL); 
	} else {
            myList->add(cfg);
        }
 
        printf("    |%s=%s\n", cfg.name, cfg.value);
    }
    
    return true;
}


