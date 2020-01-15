#ifndef _MAIN_H__
#define _MAIN_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "mp3decode.h"
#include "mp3standard.h"

/// @brief This function will print the current time
/// @note  This should only be run at start-up
static void printTime(void);

/// @brief Creates a new line via printf
static void newLine(void);

/// @note This should be endianness safe...
static int32_t unPackData(uint8_t* packedBuffer);

/// @brief This prints the current tag information.
/// @note This should be used for debugging only
void printCurrentTag(void);

/// @brief This prints the current frame information
/// @note This should be used for debugging only
void printCurrentFrame(void);

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

/// @note This should be endianness safe...
static int32_t unPackData(uint8_t* packedBuffer)
{
    return packedBuffer[0]<<24 | packedBuffer[1]<<16 | packedBuffer[2]<<8 | packedBuffer[3];
}
#endif  // _MAIN_H__
