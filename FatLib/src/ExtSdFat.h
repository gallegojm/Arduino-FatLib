/*
 * Classes to wrap SdFat library from William Greiman
 * Copyright (c) 2018 by Jean-Michel Gallego
 *
 * You must install SdFat V1.07 in order to use FatLib
 */
 
#ifndef EXTSDFAT_LIB_H
#define EXTSDFAT_LIB_H

#include <SdFat.h>

#define FF_MAX_LFN 255

class ExtSdFat : public SdFat
{
public:
  int32_t  capacity() { return card()->cardSize() >> 1; }; // capacity in kBytes
  int32_t  free()     { return vol()->freeClusterCount() * vol()->blocksPerCluster() >> 1; };
  bool     isDir( char * path );
  bool     timeStamp( char * path, uint16_t year, uint8_t month, uint8_t day,
                      uint8_t hour, uint8_t minute, uint8_t second );
  bool     getFileModTime( char * path, uint16_t * pdate, uint16_t * ptime );
};

extern ExtSdFat sd;

class ExtDir : public SdFile
{
public:
  bool     openDir( char * dirPath );
  bool     closeDir() { return close(); };
  bool     nextFile();
  bool     isDir()    { return isdir; };
  char *   fileName() { return lfn; };
  uint32_t fileSize() { return filesize; };
  uint16_t fileModDate() { return filelwd; };
  uint16_t fileModTime() { return filelwt; };

private:
  SdFile   curFile;
  char     lfn[ FF_MAX_LFN + 1 ];    // Buffer to store the LFN
  bool     isdir;
  uint32_t filesize;
  uint16_t filelwd;
  uint16_t filelwt;
};

class ExtFile : public SdFile
{
public:
  int      writeString( char * buf ) { return write( buf, strlen( buf )); }; 
  bool     writeChar( char car )     { return write( & car, 1 ); };
  int16_t  readString( char * buf, int len );
  char     readChar()                { return read(); };
  uint16_t readInt();
  uint16_t readHex();
};

// Return true if char c is allowed in a long file name

inline bool legalChar( char c )
{
  if( c == '"' || c == '*' || c == '?' || c == ':' || 
      c == '<' || c == '>' || c == '|' )
    return false;
  return 0x1f < c && c < 0x7f;
}

#endif // EXTSDFAT_LIB_H

