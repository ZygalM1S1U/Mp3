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

/// @brief Print the file struct for viewing on the console
void printFileAttributes(void);

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
    memset(fileAttributes.filename, 0u, MAX_FILENAME_SIZE);
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
    for(int i = 0; i < MAX_FILENAME_SIZE; ++i)
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

void printFileAttributes(void)
{
    if(fileOpen)
    {
        // Print the file name
        printf("File Name: ");
        for(int i = 0; i < MAX_FILENAME_SIZE; ++i)
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
