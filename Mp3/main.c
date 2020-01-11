#include "main.h"

#define DEBUG_ACTIVE                1
#define MAX_FILE_SIZE               25
#define MP3_FILE_ID_FRAME_SIZE      10
#define MP3_MAX_ID_FIELD_SIZE       4


/// @note MP3 Notes
/// @section Header Flags
/*********************************************************
 *
 * a - Unsynchronisation
 *

   Bit 7 in the 'ID3v2 flags' indicates whether or not
   unsynchronisation is applied on all frames (see section 6.1 for
   details); a set bit indicates usage.


 * b - Extended header

 * The second bit (bit 6) indicates whether or not the header is
 * followed by an extended header. The extended header is described in
 * section 3.2. A set bit indicates the presence of an extended
 * header.


 * c - Experimental indicator

 * The third bit (bit 5) is used as an 'experimental indicator'. This
 * flag SHALL always be set when the tag is in an experimental stage.


 * d - Footer present

 * Bit 4 indicates that a footer (section 3.4) is present at the very
 * end of the tag. A set bit indicates the presence of a footer.

************************************************************/

typedef enum MP3_HEADER_INDICIES_T
{
    TAG_IDENTIFICATION_INDEX = 0,
    VERSION_INDEX = 3,
    FLAGS_INDEX = 5,
    SIZE_INDEX = 6
}MP3_HEADER_INDICIES;

typedef enum TAG_FLAGS_T
{
    UNSYNCHRONIZATION_FLAG = 0,
    EXTENDED_HEADER_FLAG = 1,
    EXPERIMENTAL_FLAG = 2
}TAG_FLAGS_BIT_POS;

bool programRunning = true;

bool fileOpen = false;

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

/// @brief This function will retrieve and map the header flags for the file itself
/// @param flag - a uint8_t which is pulled from the mp3 header, to determine more attributes about the file
void HeaderFlagRetrival(uint8_t flag);

/// @brief converts the tag size into an int for a tag
uint32_t tagSizeToIntDecode(uint8_t* bytes);

/// @brief Converts an arbitrary sized ASCII buffer to hex
void convASCIItoHex(uint8_t* ASCIIBuffer, uint8_t* hexBufferPacked, uint64_t size);

/// @brief Creates a new line via printf
void newLine(void);

__attribute__((__packed__)) struct fileAttributes_t
{
    char filename[MAX_FILE_SIZE];
    uint8_t* fileContent; /// @todo See if this is needed
    long fileSize;
    struct
    {
        struct
        {
            struct
            {
                // MP3 ID3v2 Tag ID header                                                Size in bytes (10)
                uint8_t fileIdentifier[3];      // Identifies the file identity of MP3    0 - 2
                uint8_t version[2];             // Identifies the version of MP3 2.x      3 - 5
                uint8_t flags;                  // Identifies attributes of the file      6
                uint8_t size[4];                // Identifies file size 4 * byte content  7 - 10
                uint32_t tagSizeUnPacked;
            }mp3FileTagHeader;
            uint16_t frameCRC;
        }mp3CurrentFrame;

    }mp3Attributes;
};

// Static instance of struct
static struct fileAttributes_t fileAttributes;

int main(void)
{
    // Set initial values of the program
    initializeProgram();

    while(programRunning)
    {
        /// @todo Create a file opening interface for MP3 file opening
        printf("Welcome to the MP3 Parser\n");
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

    // Set the file open flag
    fileOpen = true;

    fseek(mp3FilePtr, 0L, SEEK_END);

    fileAttributes.fileSize = ftell(mp3FilePtr);
#ifdef DEBUG_ACTIVE
    printf("The file Size %ld\n", fileAttributes.fileSize);
#endif
    rewind(mp3FilePtr);

    mp3Parse(mp3FilePtr);
}

void mp3Parse(FILE* mp3FilePtr)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";

    long fileIndex = 0;
    // print the file name
    for(int i = 0; i < MAX_FILE_SIZE; ++i)
        printf("%c", fileAttributes.filename[i]);
    newLine();
    char c = 0;

    while(c != EOF)
    {
        // Header retrival routine
        if(fileIndex < MP3_FILE_ID_FRAME_SIZE)
        {
            switch((MP3_HEADER_INDICIES)fileIndex)
            {
            case TAG_IDENTIFICATION_INDEX:
                getFieldInformation(mp3FilePtr, fileInforRetrieve, 3);
                memcpy(fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.fileIdentifier, fileInforRetrieve, 3);
                break;
            case VERSION_INDEX:
                getFieldInformation(mp3FilePtr, fileInforRetrieve, 2);
                memcpy(fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.version, fileInforRetrieve, 2);
                break;
            case FLAGS_INDEX:
                fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.flags = (uint8_t)c;
                break;
            case SIZE_INDEX:
                getFieldInformation(mp3FilePtr, fileInforRetrieve, 4);
                // 00 00 00 00
                // Mask out the MSB of each byte
                fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.tagSizeUnPacked = tagSizeToIntDecode(fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.size);
                //fileAttributes.mp3Attributes.mp3FileTagHeader.tagSizeUnPacked = (*(uint32_t*)fileAttributes.mp3Attributes.mp3FileTagHeader.size);
                break;
            default:
                // Nothing left to handle in the header...
                break;
            }
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
#ifdef DEBUG_ACTIVE
        printf("printing the chars : %c\n", dataNeeded[j]);
#endif
    }

#ifdef DEBUG_ACTIVE
    for(int i = 0; i < size; ++i)
        printf("printing the Hex : 0x%02X\n", dataNeeded[i]);
#endif

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

        // Print the container version
        printf("Container Version: 2.");
        for(int j = 0; j < 2; ++j)
            printf("%d", (int)fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.version[j]);

        newLine();  // New line

        // Print the current tag size
        printf("Print Tag Size: 0x%02X\n", fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.tagSizeUnPacked);

        // Print tag flags
        uint8_t unsynchronization, experimental, extendedHeader = 0u;
        unsynchronization = fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.flags&1;
        extendedHeader = (fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.flags>>1)&1;
        experimental = (fileAttributes.mp3Attributes.mp3CurrentFrame.mp3FileTagHeader.flags>>2)&1;
        printf("Tag Flags:\nUnsynchronization: 0x%02X\nExtended Header: 0x%02X\nExperimental: 0x%02X\n",
               unsynchronization, extendedHeader, experimental);
    }
}

void HeaderFlagRetrival(uint8_t flag)
{
    // Pull out useful information

}

uint32_t tagSizeToIntDecode(uint8_t* bytes)
{
    // mask out the MSB nibble
    uint32_t val = 0u;
    uint8_t msbNibble = 0u;
    for (int i = 0 ; i < 4 ; i++)
    {
        msbNibble = bytes[i]&0x0F;
        msbNibble ^= 1 << 7;
        msbNibble >>= 1;

    }
    // toggle the MSB
    // Shove into a new int
    uint32_t val = 0;
    for (int i = 0 ; i < 4 ; i++)
    {
        bytes[i] ^= 1 << 7;
    }
    printf("tag value %u", val);
    return (val * 4);
}

void newLine(void)
{
    printf("\n");
}
