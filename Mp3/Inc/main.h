#ifndef _MAIN_H__
#define _MAIN_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "mp3decode.h"

__attribute__((__packed__)) struct fileAttributes_t
{
    char filename[MAX_FILENAME_SIZE];
    uint8_t* fileContent; /// @todo See if this is needed
    long fileSize;
    mp3Attributes mp3Attributes_u;
};

/// @brief This function will print the current time
/// @note  This should only be run at start-up
static void printTime(void);

// Static functions
static void printTime(void)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Print local date and time
    printf("\nLocal Date & Time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900,
           tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/// @brief Creates a new line via printf
static void newLine(void)
{
    printf("\n");
}

#endif  // _MAIN_H__
