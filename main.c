/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Entry point of the program
 * @version 0.1
 * @date December 2016 and January 2017
 * @file main.c
 *
 * Basic tests of the args and call parser, crawler and the link checker
 */
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
 * entry point of the program
 * @param argc number of arguments
 * @param argv array of string
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: smartfolder -d dir_name\n");
        printf("usage: smartfolder dir_name search_path [expression]\n");
        exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
        }
        logger("kill smartfolder with name: %s\n", DEBUG, true, argv[2]);
        pid_t pid = readPID(argv[2], true);
        if (pid == -1) {
            exit(EXIT_FAILURE);
        }
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
        if (readPID(argv[1], false) != -1) {
            printf("smartfolder with this name already exist\n");
            exit(EXIT_FAILURE);
        }
        logger("smartfolder number of params ok\n", DEBUG, true);
        parser(argc, argv);
        pid_t value = forkw();
        //child
        if (value == 0) {
            logger("child started\n", DEBUG, true);
            char *absolutePath = realpath(argv[2], NULL);
            while (1) {
                crawler_launcher(absolutePath);
                checkFiles(linker_destination);
                logger("\n", DEBUG, false);
                sleep(5);
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
