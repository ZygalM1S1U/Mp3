#include "../Inc/mp3decode.h"
#include "../Inc/main.h"

/// @brief converts the tag size into an int for a tag
uint32_t tagSizeToIntDecode(uint8_t* bytes);

void findTagHeader(fileAttributes_ *fileAttributes, FILE* mp3FilePtr, long fileIndex)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";

    switch((MP3_TAG_HEADER_INDICIES)fileIndex)
    {
    case TAG_IDENTIFICATION_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 3);
        memcpy(fileAttributes->mp3Attributes_u.currentTagHeader.fileIdentifier, fileInforRetrieve, 3);
        break;
    case TAG_VERSION_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 2);
        memcpy(fileAttributes->mp3Attributes_u.currentTagHeader.version, fileInforRetrieve, 2);
        break;
    case TAG_FLAGS_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 1);
        fileAttributes->mp3Attributes_u.currentTagHeader.flags = fileInforRetrieve[0];
        break;
    case TAG_SIZE_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 4);
        memcpy(fileAttributes->mp3Attributes_u.currentTagHeader.size, fileInforRetrieve, 4);
        fileAttributes->mp3Attributes_u.currentTagHeader.tagSizeUnPacked = tagSizeToIntDecode(fileAttributes->mp3Attributes_u.currentTagHeader.size);
        if(*(uint32_t*)fileAttributes->mp3Attributes_u.currentTagHeader.size > 1)    // Per id3 spec, atleast one byte for tags
            fileAttributes->frameCount.tagFound = true;
        break;
    default:
        // Nothing left to handle in the header...
        break;
    }
}

void frameIDHandler(fileAttributes_ *fileAttributes, FILE* mp3FilePtr, char currentChar)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";

    switch((FRAME_TYPES)currentChar)
    {
    case IPLS_FRAME:
        // Only one frame type with 'I'
        fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameType = IPLS_FRAME;
        memcpy(fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameId, "IPLS", 4);
        printf("IPLS Frame found\n");
        fileAttributes->frameCount.frameFound = true;
        break;
    case URL_FRAME:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 3);
        printf("URL Frame found\n");
        fileAttributes->frameCount.frameFound = true;
        break;
    case TEXT_FRAME:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 3);
        fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameId[0] = TEXT_FRAME;
        memcpy(&fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameId[1], fileInforRetrieve, 3);
        printf("Text Frame found\n");
        fileAttributes->frameCount.frameFound = true;
        break;
    default:
        break;
    }
}

void getFieldInformation(FILE* mp3FilePtr, uint8_t* dataNeeded, uint8_t size)
{
    // Get original file offset
    long fileOriginalSize = ftell(mp3FilePtr);

    // Grab the data you need
    for(int j = 0; j < size; ++j)
    {
        dataNeeded[j] = fgetc(mp3FilePtr);
    }

    // Rewind the file pointer
    if(fseek(mp3FilePtr, (-size), SEEK_CUR) != 0)
    {
        // File rewind failed.
        printf("\n\n\nFile Rewind failed.\n\n\n");
    }
}

void printFrameHandler(void)
{

    /*
    // Print the current frame
    switch(fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameType)
    {
    case :
        break;
    default:
        break;
    }*/
}

/*
 *
typedef enum FRAME_TYPES_T
{
    TEXT_FRAME = 0x54, // T in ASCII
    URL_FRAME  = 0x57, // W in ASCII
    IPLS_FRAME = 0x49, // I in ASCII, involved people list

}FRAME_TYPES;
 * */

void decodeFrameHeader(fileAttributes_ *fileAttributes, FILE* mp3FilePtr, long fileIndex, long frameIndex)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";
    //printf("frameIndex: %lu\n", fileIndex);
    //("Frame Offset: %lu\n", frameIndex);
    switch((MP3_FRAME_HEADER_INDICIES)frameIndex)
    {
    case FRAME_IDENTIFICATION_INDEX:
        // It has already been stored.
        break;
    case FRAME_SIZE_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 4);
        // No more weird size decoding :D
        memcpy(fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameSize, fileInforRetrieve, 4);
        printf("size individual fields: ");
        for(int i = 0; i < 4; ++i)
            printf(" 0x%02X ", fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameSize[i]);
        newLine();
        fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.packedFrameSize = unPackData(fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameSize);
        if(fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.packedFrameSize > 1)    // Per id3 spec, atleast one byte for frames
            fileAttributes->frameCount.validFrame = true;
        break;
    case FRAME_FLAGS_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 2);
        memcpy(fileAttributes->mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags, fileInforRetrieve, 2);
        break;
    default:
        // Nothing left to handle in the header...
        break;
    }
}

uint32_t tagSizeToIntDecode(uint8_t* bytes)
{
    return (((bytes[0] & 0x7F) << 21) | ((bytes[1] & 0x7F) << 14) | ((bytes[2] & 0x7F) << 7) | (bytes[3] & 0x7F));
}
