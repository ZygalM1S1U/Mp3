#include "../Inc/main.h"

#define DEBUG_ACTIVE                1

bool programRunning = true;

bool fileOpen = false;

bool tagFound = false;

bool frameFound = false;

bool validFrame = false;

/// @brief A function that inits the program with the intial values
void initializeProgram(void);

/// @brief A function to set up the MP3 file for parsing
void mp3FileOpen(char* fileName);

/// @brief This function will parse a file in memory from the fp
void mp3Parse(FILE* mp3FilePtr);

/// @brief This function will find all fields of information
void getFieldInformation(FILE *mp3FilePtr, uint8_t *dataNeeded, uint8_t size);

/// @brief Print the file struct for viewing on the console
void printFileAttributes(void);

/// @brief converts the tag size into an int for a tag
uint32_t tagSizeToIntDecode(uint8_t* bytes);

/// @brief Creates a new line via printf
void newLine(void);

/// @brief Finds a tag header
void findTagHeader(FILE* mp3FilePtr, long fileIndex);

/// @brief Parses the text frame header and fills in the appropriate information
void frameIDHandler(FILE* mp3FilePtr, char currentChar);

/// @brief Print the current frame
void printFrameHandler(void);

/// @brief Decode the frame header
void decodeFrameHeader(FILE* mp3FilePtr, long fileIndex, long frameIndex);

__attribute__((__packed__)) struct fileAttributes_t
{
    char filename[MAX_FILE_SIZE];
    uint8_t* fileContent; /// @todo See if this is needed
    long fileSize;
    mp3Attributes mp3Attributes_u;
};

// Static instance of struct
static struct fileAttributes_t fileAttributes;

int main(void)
{
    // Set initial values of the program
    initializeProgram();
    newLine();
    printf("Welcome to the MP3 Parser\n");
    printTime();
    newLine();
    while(programRunning)
    {
        /// @todo Create a file opening interface for MP3 file opening
        printf("Please type in the MP3 file you'd like to decode, make sure your directory is correct.\n");
        scanf("%s", &fileAttributes.filename);
        mp3FileOpen(fileAttributes.filename);
    }
    return 0;
}

void initializeProgram(void)
{
    memset(fileAttributes.filename, 0u, MAX_FILE_SIZE);
    fileAttributes.fileSize = 0u;
}

void mp3FileOpen(char* fileName)
{
    FILE* mp3FilePtr;

    strcat(fileName, ".mp3");

    mp3FilePtr = fopen(fileName, "r");
    if(!mp3FilePtr) perror(fileName), exit(1);
    else
    {
        // Set the file open flag
        fileOpen = true;
    }

    fseek(mp3FilePtr, 0L, SEEK_END);

    fileAttributes.fileSize = ftell(mp3FilePtr);
#ifdef DEBUG_ACTIVE
    printf("The file Size %ld\n", fileAttributes.fileSize);
#endif
    rewind(mp3FilePtr);

    if(fileOpen)
    {
        mp3Parse(mp3FilePtr);
    }
}

void mp3Parse(FILE* mp3FilePtr)
{
    long fileIndex, newFrameIndex, frameOffset = 0;
    // print the file name
    for(int i = 0; i < MAX_FILE_SIZE; ++i)
        printf("%c", fileAttributes.filename[i]);
    newLine();
    char c = 0;

    while(c != EOF)
    {
        // Header retrival routine
        if(fileIndex < MP3_FILE_ID_FRAME_SIZE && !tagFound)
        {
            findTagHeader(mp3FilePtr, fileIndex);
        }
        else if(!frameFound)
        {
            printf("Searching for frame at file index: %lu with char %c\n", fileIndex, c);
            frameIDHandler(mp3FilePtr, c);

            if(frameFound)
            {
                printf("File Index when setting a frame: %lu\n", fileIndex);
                frameOffset = fileIndex-4;
                newFrameIndex = fileIndex - frameOffset - 4;
                printf("New Frame Index: %lu\n", newFrameIndex);
            }
        }
        else
        {
            // Frame has been found, pull the frame header
            decodeFrameHeader(mp3FilePtr, fileIndex, newFrameIndex);
             ++newFrameIndex;
        }


        c = fgetc(mp3FilePtr);
        fileIndex = ftell(mp3FilePtr);

    }
    newLine();
    printf("Done with file dump\n");
    printFileAttributes();
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
    fseek(mp3FilePtr, 0, (int)fileOriginalSize);
}

void printFileAttributes(void)
{
    if(fileOpen)
    {
        // Print the file name
        printf("File Name: ");
        for(int i = 0; i < MAX_FILE_SIZE; ++i)
            printf("%c", fileAttributes.filename[i]);
        newLine();  // New line

        // Print file size
        printf("File Size Reported from Ftell: %lu\n", fileAttributes.fileSize);
        newLine();
        if(tagFound)
        {
            printf("Current Tag Information");
            newLine();
            /// @section Current Tag Information
            // Print the container version
            printf("Container Version: 2.");
            for(int j = 0; j < 2; ++j)
                printf("%d", (int)fileAttributes.mp3Attributes_u.currentTagHeader.version[j]);
            newLine();  // New line

            // Print the current tag size
            printf("Tag Size: 0x%08X\n", fileAttributes.mp3Attributes_u.currentTagHeader.tagSizeUnPacked);

            // Print tag flags
            uint8_t unsynchronization, experimental, extendedHeader = 0u;
            unsynchronization = fileAttributes.mp3Attributes_u.currentTagHeader.flags&1;
            extendedHeader = (fileAttributes.mp3Attributes_u.currentTagHeader.flags>>1)&1;
            experimental = (fileAttributes.mp3Attributes_u.currentTagHeader.flags>>2)&1;
            printf("Tag Flags:\nUnsynchronization: 0x%02X\nExtended Header: 0x%02X\nExperimental: 0x%02X\n",
                   unsynchronization, extendedHeader, experimental);
        }
        newLine();

        if(validFrame)
        {
            printf("Current Frame Information: ");
            newLine();
            /// @section Current Frame Information
            // Print the frame ID
            printf("Frame ID: ");
            for(int k = 0; k < 4; ++k)
                printf("%c", fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameId[k]);
            newLine();

            // Print the frame size
            printf("Frame Size: 0x%08X\n", *(uint32_t*)fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameSize);

            // Print frame flags
            uint8_t tagAlterPres, fileAlterPres, RO, Compression, Encryption, GI = 0u;
            tagAlterPres = fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags[0]&1;
            fileAlterPres = (fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags[0]>>1)&1;
            RO = (fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags[0]>>2)&1;
            Compression = fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags[1]&1;
            Encryption =  (fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags[1]>>1)&1;
            GI = (fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags[1]>>2)&1;
            printf("Frame Flags:\nTag Alter Preservation: 0x%02X\nFile Alter Preservation: 0x%02X\nRead-Only: 0x%02X\nCompression: 0x%02X\nEncryption: 0x%02X\nGrouping Identity: 0x%02X\n",
                   tagAlterPres, fileAlterPres, RO, Compression, Encryption, GI);
        }

    }
}

uint32_t tagSizeToIntDecode(uint8_t* bytes)
{
    return (((bytes[0] & 0x7F) << 21) | ((bytes[1] & 0x7F) << 14) | ((bytes[2] & 0x7F) << 7) | (bytes[3] & 0x7F));
}

void newLine(void)
{
    printf("\n");
}

void findTagHeader(FILE* mp3FilePtr, long fileIndex)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";

    switch((MP3_TAG_HEADER_INDICIES)fileIndex)
    {
    case TAG_IDENTIFICATION_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 3);
        memcpy(fileAttributes.mp3Attributes_u.currentTagHeader.fileIdentifier, fileInforRetrieve, 3);
        break;
    case TAG_VERSION_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 2);
        memcpy(fileAttributes.mp3Attributes_u.currentTagHeader.version, fileInforRetrieve, 2);
        break;
    case TAG_FLAGS_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 1);
        fileAttributes.mp3Attributes_u.currentTagHeader.flags = fileInforRetrieve[0];
        break;
    case TAG_SIZE_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 4);
        memcpy(fileAttributes.mp3Attributes_u.currentTagHeader.size, fileInforRetrieve, 4);
        fileAttributes.mp3Attributes_u.currentTagHeader.tagSizeUnPacked = tagSizeToIntDecode(fileAttributes.mp3Attributes_u.currentTagHeader.size);
        if(*(uint32_t*)fileAttributes.mp3Attributes_u.currentTagHeader.size > 1)    // Per id3 spec, atleast one byte for tags
            tagFound = true;
        break;
    default:
        // Nothing left to handle in the header...
        break;
    }
}

void frameIDHandler(FILE* mp3FilePtr, char currentChar)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";

    switch((FRAME_TYPES)currentChar)
    {
    case IPLS_FRAME:
        // Only one frame type with 'I'
        fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameType = IPLS_FRAME;
        memcpy(fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameId, "IPLS", 4);
        printf("IPLS Frame found\n");
        frameFound = true;
        break;
    case URL_FRAME:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 3);
        printf("URL Frame found\n");
        frameFound = true;
        break;
    case TEXT_FRAME:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 3);
        fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameId[0] = TEXT_FRAME;
        memcpy(&fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameId[1], fileInforRetrieve, 3);
        printf("Text Frame found\n");
        frameFound = true;
        break;
    default:
        break;
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

void decodeFrameHeader(FILE* mp3FilePtr, long fileIndex, long frameIndex)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";
    printf("frameIndex: %lu\n", fileIndex);
    printf("File Index - Frame Offset: %lu\n", frameIndex);
    switch((MP3_FRAME_HEADER_INDICIES)frameIndex)
    {
    case FRAME_IDENTIFICATION_INDEX:
        // Already handled :)
        break;
    case FRAME_SIZE_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 4);
        // No more weird size decoding :D
        memcpy(fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameSize, fileInforRetrieve, 4);
        printf("size individual fields: ");
        for(int i = 0; i < 4; ++i)
            printf(" 0x%02X ", fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameSize[i]);
        newLine();
        if(*(uint32_t*)fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameSize > 1)    // Per id3 spec, atleast one byte for frames
            validFrame = true;
        break;
    case FRAME_FLAGS_INDEX:
        getFieldInformation(mp3FilePtr, fileInforRetrieve, 2);
        memcpy(fileAttributes.mp3Attributes_u.currentFrame.currentFrameHeader.frameFlags, fileInforRetrieve, 2);
        break;
    default:
        // Nothing left to handle in the header...
        break;
    }
}
