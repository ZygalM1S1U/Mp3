#ifndef _MP3DECODE_H__
#define _MP3DECODE_H__

#include "main.h"
#include "mp3standard.h"

typedef struct __attribute__((__packed__)) frameCount_t
{
    bool validFrame;
    bool frameFound;
    bool tagFound;
}frameCount_;

typedef struct __attribute__((__packed__)) fileAttributes_t
{
    char filename[MAX_FILENAME_SIZE];
    uint8_t* fileContent; /// @todo See if this is needed
    long fileSize;
    mp3Attributes mp3Attributes_u;
    frameCount_ frameCount;
}fileAttributes_;

/// @brief Finds a tag header
void findTagHeader(fileAttributes_* fileAttributes, FILE* mp3FilePtr, long fileIndex);

/// @brief Parses the text frame header and fills in the appropriate information
void frameIDHandler(fileAttributes_* fileAttributes, FILE* mp3FilePtr, char currentChar);

/// @brief Print the current frame
void printFrameHandler(void);

/// @brief Decode the frame header
void decodeFrameHeader(fileAttributes_* fileAttributes, FILE* mp3FilePtr, long fileIndex, long frameIndex);

/// @brief This function will find all fields of information
void getFieldInformation(FILE *mp3FilePtr, uint8_t *dataNeeded, uint8_t size);

#endif // _MP3DECODE_H__
