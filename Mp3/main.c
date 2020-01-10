#include "main.h"

#define DEBUG_ACTIVE                1
#define MAX_FILE_SIZE               25
#define MP3_FILE_ID_FRAME_SIZE      128
#define MP3_MAX_ID_FIELD_SIZE       30

typedef enum MP3_HEADER_INDICIES_T
{
    TAG_IDENTIFICATION_INDEX = 0,
    TITLE_INDEX = 3,
    ARTIST_INDEX = 33,
    ALBUM_INDEX = 63,
    YEAR_INDEX = 93,
    COMMENT_INDEX = 97,
    GENRE_INDEX = 127
}MP3_HEADER_INDICIES;


bool programRunning = true;

/// @brief A function that inits the program with the intial values
void initializeProgram(void);

/// @brief A function to set up the MP3 file for parsing
void mp3FileOpen(char* fileName);

/// @brief This function will parse a file in memory from the fp
void mp3Parse(FILE* mp3FilePtr);

/// @brief This function will find all fields of information
void getFieldInformation(FILE *mp3FilePtr, uint8_t *dataNeeded, uint8_t size);

__attribute__((__packed__)) struct fileAttributes_t
{
    char filename[MAX_FILE_SIZE];
    uint8_t* fileContent;
    long fileSize;
    struct
    {
        // Tag ID header                    Size in bytes (128)
        uint8_t tagId[3];                   // 3	(0-2)        Tag identification. Must contain 'TAG' if tag exists and is correct.
        uint8_t title[30];                  // B	30	(3-32)	 Title
        uint8_t artist[30];                 // C	30	(33-62)	 Artist
        uint8_t album[30];                  // D	30	(63-92)	 Album
        uint8_t year[4];                    // E	4	(93-96)	 Year
        uint8_t comment[30];                // F	30	(97-126) Comment
        uint8_t genre;                      // G	1	(127)	 Genre
        struct
        {
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

    fseek(mp3FilePtr, 0L, SEEK_END);

    fileAttributes.fileSize = ftell(mp3FilePtr);

    rewind(mp3FilePtr);

    mp3Parse(mp3FilePtr);
}

void mp3Parse(FILE* mp3FilePtr)
{
    uint8_t fileInforRetrieve[MP3_MAX_ID_FIELD_SIZE] = "";

#ifdef DEBUG_ACTIVE
    long fileIndex = 0;
    // print the file name
    for(int i = 0; i < MAX_FILE_SIZE; ++i)
        printf("%c", fileAttributes.filename[i]);
    printf("\n");
    char c = 0;
    fileIndex = ftell(mp3FilePtr);
    printf("Index 1: %ld\n", fileIndex);
    while(c != EOF)
    {
        if(fileIndex < MP3_FILE_ID_FRAME_SIZE)
        {
            switch((MP3_HEADER_INDICIES)fileIndex)
            {
            case TAG_IDENTIFICATION_INDEX:
                getFieldInformation(mp3FilePtr, fileInforRetrieve, sizeof(fileAttributes.mp3Attributes.tagId));
                memcpy(fileAttributes.mp3Attributes.tagId, fileInforRetrieve, sizeof(fileAttributes.mp3Attributes.tagId));
                printf("Tag ID: ");
                for(int i = 0; i < 3; ++i)
                    printf("%c", fileAttributes.mp3Attributes.tagId[i]);
                printf("\n");
                break;
            case TITLE_INDEX:
                break;
            case ARTIST_INDEX:
                break;
            case ALBUM_INDEX:
                break;
            case YEAR_INDEX:
                break;
            case COMMENT_INDEX:
                break;
            case GENRE_INDEX:
                break;
            default:
                break;
            }
        }
        c = fgetc(mp3FilePtr);
        fileIndex = ftell(mp3FilePtr);
    }
    printf("\n");
    printf("Done with file dump\n");
#endif
}

void getFieldInformation(FILE* mp3FilePtr, uint8_t* dataNeeded, uint8_t size)
{
    // Get original file offset
    long fileOriginalSize = ftell(mp3FilePtr);

    // Grab the data you need
    for(int j = 0; j < size; ++j)
    {
        dataNeeded[j] = (uint8_t)fgetc(mp3FilePtr);
    }

    // Rewind the file pointer
    fseek(mp3FilePtr, 0, (int)fileOriginalSize);
}

