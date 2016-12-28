#include <stdlib.h>
#include <string.h>
#include <errno.h>
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
    log("démarrage");
    //parser(argc, argv);
    //pid_t value = forkw();

    crawler_launcher(argv[2]);
    //child
    //if(value == 0){

    //error
    //}else if(value < 0){
    //    log("error fork");
    //    log(strerror(errno));
    //    return 1;
    //}
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
    return EXIT_SUCCESS;
}