#ifndef _MP3_STANDARD_H__
#define _MP3_STANDARD_H__


#define MAX_FILENAME_SIZE           25
#define MP3_FILE_ID_FRAME_SIZE      10
#define MP3_MAX_ID_FIELD_SIZE       4
#define MP3_MAX_FRAME_TYPE_SIZE     3
#define MP3_TAG_HEADER_OFFSET       0x0A
#define MP3_FRAME_HEADER_OFFSET


/// @todo figure out if this is needed.
typedef enum GENRES_T
{
    BLUES = 0

}GENRES;

/// @note MP3 Notes
/// @section Tag Header Flags
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

typedef enum MP3_TAG_HEADER_INDICIES_T
{
    TAG_IDENTIFICATION_INDEX = 0,
    TAG_VERSION_INDEX = 3,
    TAG_FLAGS_INDEX = 5,
    TAG_SIZE_INDEX = 6
}MP3_TAG_HEADER_INDICIES;

typedef enum MP3_FRAME_HEADER_INDICIES_T
{
    //FRAME_IDENTIFICATION_INDEX = 0,
    FRAME_SIZE_INDEX = 0,
    FRAME_FLAGS_INDEX = 4
}MP3_FRAME_HEADER_INDICIES;

typedef enum TAG_FLAGS_T
{
    UNSYNCHRONIZATION_FLAG = 0,
    EXTENDED_HEADER_FLAG = 1,
    EXPERIMENTAL_FLAG = 2
}TAG_FLAGS_BIT_POS;

typedef enum FRAME_FLAGS_T
{
    FRAME_TAG_ALTER_PRESERVATION_FLAG = 0,
    FRAME_FILE_ALTER_PRESERVATION_FLAG = 1,
    FRAME_READ_ONLY_FLAG = 2,
    FRAME_COMPRESSION_FLAG = 8,
    FRAME_ENCRYPTION_FLAG = 9,
    FRAME_GROUPING_INDENTITY_FLAG = 10
}FRAME_FLAGS_BIT_POS;


// This is the standard for ip3v2.3
/*
 *
 * 4.20    AENC    [[#sec4.20|Audio encryption]]
4.15    APIC    [#sec4.15 Attached picture]
4.11    COMM    [#sec4.11 Comments]
4.25    COMR    [#sec4.25 Commercial frame]
4.26    ENCR    [#sec4.26 Encryption method registration]
4.13    EQUA    [#sec4.13 Equalization]
4.6     ETCO    [#sec4.6 Event timing codes]
4.16    GEOB    [#sec4.16 General encapsulated object]
4.27    GRID    [#sec4.27 Group identification registration]
4.4     IPLS    [#sec4.4 Involved people list]
4.21    LINK    [#sec4.21 Linked information]
4.5     MCDI    [#sec4.5 Music CD identifier]
4.7     MLLT    [#sec4.7 MPEG location lookup table]
4.24    OWNE    [#sec4.24 Ownership frame]
4.28    PRIV    [#sec4.28 Private frame]
4.17    PCNT    [#sec4.17 Play counter]
4.18    POPM    [#sec4.18 Popularimeter]
4.22    POSS    [#sec4.22 Position synchronisation frame]
4.19    RBUF    [#sec4.19 Recommended buffer size]
4.12    RVAD    [#sec4.12 Relative volume adjustment]
4.14    RVRB    [#sec4.14 Reverb]
4.10    SYLT    [#sec4.10 Synchronized lyric/text]
4.8     SYTC    [#sec4.8 Synchronized tempo codes]
4.2.1   TALB    [#TALB Album/Movie/Show title]
4.2.1   TBPM    [#TBPM BPM (beats per minute)]
4.2.1   TCOM    [#TCOM Composer]
4.2.1   TCON    [#TCON Content type]
4.2.1   TCOP    [#TCOP Copyright message]
4.2.1   TDAT    [#TDAT Date]
4.2.1   TDLY    [#TDLY Playlist delay]
4.2.1   TENC    [#TENC Encoded by]
4.2.1   TEXT    [#TEXT Lyricist/Text writer]
4.2.1   TFLT    [#TFLT File type]
4.2.1   TIME    [#TIME Time]
4.2.1   TIT1    [#TIT1 Content group description]
4.2.1   TIT2    [#TIT2 Title/songname/content description]
4.2.1   TIT3    [#TIT3 Subtitle/Description refinement]
4.2.1   TKEY    [#TKEY Initial key]
4.2.1   TLAN    [#TLAN Language(s)]
4.2.1   TLEN    [#TLEN Length]
4.2.1   TMED    [#TMED Media type]
4.2.1   TOAL    [#TOAL Original album/movie/show title]
4.2.1   TOFN    [#TOFN Original filename]
4.2.1   TOLY    [#TOLY Original lyricist(s)/text writer(s)]
4.2.1   TOPE    [#TOPE Original artist(s)/performer(s)]
4.2.1   TORY    [#TORY Original release year]
4.2.1   TOWN    [#TOWN File owner/licensee]
4.2.1   TPE1    [#TPE1 Lead performer(s)/Soloist(s)]
4.2.1   TPE2    [#TPE2 Band/orchestra/accompaniment]
4.2.1   TPE3    [#TPE3 Conductor/performer refinement]
4.2.1   TPE4    [#TPE4 Interpreted, remixed, or otherwise modified by]
4.2.1   TPOS    [#TPOS Part of a set]
4.2.1   TPUB    [#TPUB Publisher]
4.2.1   TRCK    [#TRCK Track number/Position in set]
4.2.1   TRDA    [#TRDA Recording dates]
4.2.1   TRSN    [#TRSN Internet radio station name]
4.2.1   TRSO    [#TRSO Internet radio station owner]
4.2.1   TSIZ    [#TSIZ Size]
4.2.1   TSRC    [#TSRC ISRC (international standard recording code)]
4.2.1   TSSE    [#TSEE Software/Hardware and settings used for encoding]
4.2.1   TYER    [#TYER Year]
4.2.2   TXXX    [#TXXX User defined text information frame]
4.1     UFID    [#sec4.1 Unique file identifier]
4.23    USER    [#sec4.23 Terms of use]
4.9     USLT    [#sec4.9 Unsychronized lyric/text transcription]
4.3.1   WCOM    [#WCOM Commercial information]
4.3.1   WCOP    [#WCOP Copyright/Legal information]
4.3.1   WOAF    [#WOAF Official audio file webpage]
4.3.1   WOAR    [#WOAR Official artist/performer webpage]
4.3.1   WOAS    [#WOAS Official audio source webpage]
4.3.1   WORS    [#WORS Official internet radio station homepage]
4.3.1   WPAY    [#WPAY Payment]
4.3.1   WPUB    [#WPUB Publishers official webpage]
4.3.2   WXXX    [#WXXX User defined URL link frame]
 */

typedef enum FRAME_TYPES_T
{
    TEXT_FRAME = 0x54, // T in ASCII
    URL_FRAME  = 0x57, // W in ASCII
    IPLS_FRAME = 0x49, // I in ASCII, involved people list

}FRAME_TYPES;


typedef struct __attribute__((__packed__)) mp3FileTagHeader_t
{
    // MP3 ID3v2 Tag ID header                                                Size in bytes (10)
    uint8_t fileIdentifier[3];      // Identifies the file identity of MP3    0 - 2
    uint8_t version[2];             // Identifies the version of MP3 2.x      3 - 5
    uint8_t flags;                  // Identifies attributes of the file      6
    uint8_t size[4];                // Identifies file size 4 * byte content  7 - 10
    uint32_t tagSizeUnPacked;
}mp3FileTagHeader;

typedef struct __attribute__((__packed__)) mp3CurrentFrameHeader_t
{
    /*  Flags
     *  Bit 1  : Tag alter preservation  -> Tells what to do with this frame if it is unknown and the tag is altered in any way.
     *                                      0    Frame should be preserved.
     *                                      1    Frame should be discarded.
     *
     *  Bit 2  : File alter Preservation -> Tells what to do if the frame unknown and the file, excluding the tag, is altered.
     *                                      This does not apply when the audio is completely replaced with other audio data.
     *                                      0    Frame should be preserved.
     *                                      1    Frame should be discarded.
     *
     *  Bit 3  : Read Only ->               Tells the software that the contents of this frame is intended to be read only,
     *                                      should be flipped if the section is altered
     *                                      0 Frame is R/W
     *                                      1 Frame is Read-only
     *
     *  Bit 9  : Compression ->             This flag indicates whether or not the frame is compressed.
     *                                      0    Frame is not compressed.
     *                                      1    Frame is compressed using [#ZLIB zlib] with 4 bytes for 'decompressed size' appended to the frame header.
     *
     *  Bit 10 : Encryption  ->             This flag indicates wether or not the frame is enrypted. If set one byte indicating with which method it was
     *                                      encrypted will be appended to the frame header.
     *                                      0   Frame is not encrypted.
     *                                      1    Frame is encrypted.
     *
     *  Bit 11 : Grouping Identity ->       This flag indicates whether or not this frame belongs in a group with other frames.
     *                                      If set a group identifier byte is added to the frame header. Every frame with the same group identifier belongs
     *                                      to the same group.
     *                                      0    Frame does not contain group information
     *                                      1    Frame contains group information
     */
    uint8_t frameId[4];     // Frame-ID    -> 4 bytes
    uint8_t frameSize[4];   // Frame Size  -> 4 bytes
    uint8_t frameFlags[2];  // Frame flags -> 2 bytes
    FRAME_TYPES frameType;  // Frame type for grouping
}mp3CurrentFrameHeader;

typedef struct __attribute__((__packed__)) mp3CurrentFrame_t
{
    mp3CurrentFrameHeader currentFrameHeader;
    uint16_t frameCRC;
}mp3CurrentFrame;

typedef struct __attribute__((__packed__)) mp3FileInformation
{
    char artistInfo[30];
    char songInfo[30];
    char fileGenre[30];
    uint64_t trackLength;



}mp3FileInfo;


typedef struct __attribute__((__packed__)) mp3Attributes_t
{
    mp3FileTagHeader currentTagHeader;
    mp3CurrentFrame currentFrame;
    mp3FileInfo trackFileInfo;

}mp3Attributes;



#endif // _MP3_STANDARD_H__
