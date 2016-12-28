//
// Created by matoran on 12/16/16.
//

#include "wrappersyscall.h"
#include <sys/stat.h>

pid_t forkw(){
    return fork();
}
