#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "logger.h"
#include "parser.h"
#include "wrappersyscall.h"
#include "crawler.h"

/**
 *
 * @param directory
 * @param result
 */
/*void search(char *directory, char *result){

    printf("répertoire actuel %s\n", get_current_dir_name());
    chdir(directory);
    printf("répertoire actuel après chdir %s\n", get_current_dir_name());
    int handle = opendir(".");
    //printf("numero handle %d\n", handle);
    struct dirent *actualDir;
    char *source = malloc(2000);
    char *link = malloc(2000);
    while((actualDir = readdir(handle)) != NULL){
        switch(actualDir->d_type){
            case 0:
                //printf("dossier courant et dossier parent: ");
                break;
            case DT_DIR:
                printf("dossier: ");
                printf(" %s\n", actualDir->d_name);
                search(actualDir->d_name, result);
                chdir("..");
                break;
            case DT_REG:
                strcpy(link, result);
                strcpy(source, get_current_dir_name());
                strcat(strcat(source, "/"), actualDir->d_name);
                strcat(link, actualDir->d_name);
                printf("source: %s \n", source);
                printf("dest: %s \n", link);
                printf(" %s\n", actualDir->d_name);
                symlink(source, link);
                //faire test si vrai alors créer lien dans result
                break;
            case DT_LNK:
                printf("lien: ");
                printf(" %s\n", actualDir->d_name);
                break;
            default:
                printf("type inconnu %d\n", actualDir->d_type);
                printf(" %s\n", actualDir->d_name);
        }
        printf("\n");

    }

    closedir(handle);
}*/

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
        logFile("kill smartfolder with name");
        logFile(argv[2]);
        pid_t pid = readPID(argv[2]);
        //read file
        if(kill(pid, SIGKILL) == 0){
            printf("killed\n");
        }else{
            perror("kill");
        }
    }else{
        parser(argc, argv);

        pid_t value = forkw();
        printf("\n%ld %s\n", (long)value, argv[1]);
        //child
        if(value == 0){
            //redirige stdout vers un fichier
            FILE *out;
            out = fopen("out.txt", "a");
            dup2(fileno(out), STDOUT_FILENO);
            debug("child started");
            crawler_launcher(argv[2]);
            while(1){}
            debug("child ended");
            //error
        }else if(value < 0){
            logFile("error fork");
            logFile(strerror(errno));
            return 1;
        }else{
            savePID(argv[1], value);
        }
    }

    /*printf("%d unknow\n", DT_UNKNOWN);
    printf("%d fifo\n", DT_FIFO);
    printf("%d chr\n", DT_CHR);
    printf("%d dir\n", DT_DIR);
    printf("%d blk\n", DT_BLK);
    printf("%d reg\n", DT_REG);
    printf("%d lnk\n", DT_LNK);
    printf("%d sock\n", DT_SOCK);
    crawler_create();


    char *directory = "/home/cyril.iseli/test";
    char *result = "/home/cyril.iseli/result/";
    chdir(directory);
    //symlink("/home/cyril.iseli/bernstein.m", "/home/cyril.iseli/result/bernstein.m");
    search(directory, result);*/
    debug("parent ended");
    return EXIT_SUCCESS;
}