/**
 * tool.h
 * 
 * copyright (c) 2018
 *  Wei-Hsiang Tseng
 *  all right reserved
 * 
 */

#include "tool.h"
void rmFileAtrr (char file_name[80]) {
    char *pch = NULL;
    if(NULL != (pch = strrchr(file_name,'.'))) {
        *pch = '\0';
    }
}