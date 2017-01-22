#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "logger.h"
#include "parser.h"
#include "wrappersyscall.h"
#include "crawler.h"
#include "destroyer.h"
#include "linker.h"

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: smartfolder -d dir_name\n");
        printf("usage: smartfolder dir_name search_path [expression]\n");
        exit(1);
    }
    logger("smartfolder launched\n", LOG, true);
    logger("smartfolder params:", LOG, true);
    for (int i = 1; i < argc; ++i) {
        logger("%s ", LOG, false, argv[i]);
    }
    logger("\n", LOG, false);
    if (strcmp(argv[1], "-d") == 0) {
        logger("smartfolder -d\n", DEBUG, true);
        if (argc != 3) {
            printf("usage: smartfolder -d dir_name\n");
            exit(1);
        }
        logger("kill smartfolder with name: %s\n", DEBUG, true, argv[2]);
        pid_t pid = readPID(argv[2]);
        logger("pid read: %d\n", DEBUG, true, pid);
        //read file
        killw(pid, SIGKILL);
        logger("smartfolder: %s with pid %d killed\n", LOG, true, argv[2], pid);
        char *absolutePath = readPath(argv[2]);
        destroy(absolutePath);
        logger("directory %s removed\n", LOG, true, absolutePath);
    } else {
        if (argc < 3) {
            printf("usage: smartfolder dir_name search_path [expression]\n");
            exit(2);
        }
        logger("smartfolder number of params ok\n", DEBUG, true);
        parser(argc, argv);
        pid_t value = forkw();
        //child
        if (value == 0) {
            logger("child started\n", DEBUG, true);
            while (1) {
                crawler_launcher(argv[2]);
                logger("crawler ok\n", DEBUG, true);
                checkFiles(linker_destination);
                logger("check files ok\n", DEBUG, true);
                sleep(1);
            }
        } else {
            logger("child PID %ld\n", DEBUG, true, (long) value);
            savePID(argv[1], value);
            savePath(argv[1], realpath(argv[1], NULL));
        }
    }
    logger("smartfolder parent ended\n", DEBUG, true);
    return EXIT_SUCCESS;
}
