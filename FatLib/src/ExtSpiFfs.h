/*
 * Classes to wrap FS library
 * Copyright (c) 2018 by Jean-Michel Gallego
 */

 #ifndef EXTSPIFFS_LIB_H
#define EXTSPIFFS_LIB_H

#ifdef ESP8266

#include <FS.h>

#define FF_MAX_LFN 32

#define O_READ  1
#define O_WRITE 2
#define O_RDWR  3
#define O_CREAT 0x40
#define O_APPEND 0x04

class SpiFfsClass
{
public:
  bool    begin()     { return SPIFFS.begin(); };
  Dir     openDir( const char* path )
                      { return SPIFFS.openDir( path ); };
  bool    exists( const char* path )
                      { return SPIFFS.exists( path ); };
  bool    remove( const char* path )
                      { return SPIFFS.remove( path ); };
  bool    rename( const char * oldName, const char * newName )
                      { return SPIFFS.rename( oldName, newName ); };
  int32_t capacity()  { SPIFFS.info( fs_info ); // capacity in kBytes
                        return fs_info.totalBytes >> 10; };
  int32_t  free()     { SPIFFS.info( fs_info ); // free space in kBytes
                        return ( fs_info.totalBytes - fs_info.usedBytes ) >> 10; };
  bool     timeStamp( char * path, uint16_t year, uint8_t month, uint8_t day,
                      uint8_t hour, uint8_t minute, uint8_t second )
                      { return false; };
  bool     getFileModTime( char * path, uint16_t * pdate, uint16_t * ptime )
                      { * pdate = 0x2821; * ptime = 0x2000; // 01/01/2000 00:00:00
                       return true; };
  bool     isDir( char * path )
                      { return strcmp( path, "/" ) == 0; };
  bool     mkdir( char * path )
                      { return false; };
  bool     rmdir( char * path )
                      { return false; };

private:
  FSInfo fs_info;
};

extern SpiFfsClass SpiFfs;

class ExtDir
{
public:
  bool openDir( char * dirPath )
                         { d = SPIFFS.openDir( dirPath );
                           return true; };
  bool closeDir()        { return true; };
  bool nextFile()        { return d.next(); };
  char * fileName()      { strncpy( fname, d.fileName().c_str() + 1, FF_MAX_LFN );
                           return fname; };
  uint32_t fileSize()    { return (uint32_t) d.fileSize(); };
  bool     isDir()       { return false; };
  uint16_t fileModDate() { return 0x2821; }; // 01/01/2000 00:00:00
  uint16_t fileModTime() { return 0x2000; };
    
private:
  Dir d;
  char fname[ FF_MAX_LFN ];
};

class ExtFile
{
public:
//  ~ExtFsFile() { delete f; }
  bool open( char * fileName, uint8_t mode = O_READ );
  bool close()
           { f.close(); return true; };
  uint32_t write( void * buf, uint32_t lbuf )
           { return f.write((uint8_t *) buf, lbuf ); }; 
  int writeString( char * buf )
           { return (int) f.write((uint8_t *) buf, strlen( buf )); }; 
  bool writeChar( char car )
           { return f.write((uint8_t) car ) == 1; };
  uint32_t read( uint8_t * buf, uint32_t lbuf )
           { return f.read( buf, lbuf ); };
  int16_t readString( char * buf, int lbuf );
  char readChar()
           { return f.read(); };
  bool seekSet( uint32_t cur )
           { return f.seek( cur ); };
  uint32_t fileSize()
           { return f.size(); };
  
private:
  File f;
};

// Return true if char c is allowed in a long file name

inline bool legalChar( char c )
{
  if( c == '"' || c == '*' || c == '?' || c == ':' || 
      c == '<' || c == '>' || c == '|' )
    return false;
  return true;
}

#endif // ESP8266
#endif // EXTSPIFFS_LIB_H
