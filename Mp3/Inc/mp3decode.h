#ifndef _MP3DECODE_H__
#define _MP3DECODE_H__

#include "main.h"
#include "mp3standard.h"

/// @brief Finds a tag header
void findTagHeader(FILE* mp3FilePtr, long fileIndex);

/// @brief Parses the text frame header and fills in the appropriate information
void frameIDHandler(FILE* mp3FilePtr, char currentChar);

/// @brief Print the current frame
void printFrameHandler(void);

/// @brief Decode the frame header
void decodeFrameHeader(FILE* mp3FilePtr, long fileIndex, long frameIndex);

/// @brief This function will find all fields of information
void getFieldInformation(FILE *mp3FilePtr, uint8_t *dataNeeded, uint8_t size);

#endif // _MP3DECODE_H__
