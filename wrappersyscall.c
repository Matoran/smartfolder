#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "wrappersyscall.h"


pid_t forkw() {
    int val;
    if ((val = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    return val;
}
