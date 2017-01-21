#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "logger.h"
#include "parser.h"
#include "wrappersyscall.h"
#include "crawler.h"
#include "destroyer.h"

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]){
    if(strcmp(argv[1], "-d") == 0){
        if(argc != 3){
            printf("usage: ./smartfolder -d dir_name\n");
            exit(2);
        }
        logger("kill smartfolder with name: %s", DEBUG, argv[2]);
        pid_t pid = readPID(argv[2]);
        //read file
        if(kill(pid, SIGKILL) == 0){
            printf("killed\n");
            destroy(readPath(argv[2]));
        }else{
            perror("kill");
        }
    }else{
        if(argc < 3){
            printf("usage: ./smartfolder dir_name search_path [expression]\n");
            exit(2);
        }
        parser(argc, argv);
        printf("realpath of %s : %s", argv[1],  realpath(argv[1], NULL));

        pid_t value = forkw();
        printf("\n%ld %s\n", (long)value, argv[1]);
        //child
        if(value == 0){
            debug("child started");
            while(1){
                crawler_launcher(argv[2]);
                //checkFiles();

                sleep(10);
            }
            debug("child ended");
            //error
        }else{
            savePID(argv[1], value);
            savePath(argv[1], realpath(argv[1], NULL));
        }
    }
    debug("parent ended");
    return EXIT_SUCCESS;
}
