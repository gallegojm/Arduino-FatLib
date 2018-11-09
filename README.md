# Arduino-FatLib
## FatLib allow you to easily switch between libraries SdFat, FatFs and SPIFFS

Select the library modifying line '#define FAT_USE ...' at beginning of file FatLib.h

You need to install library SdFat (v 1.07) written by William Greiman
(https://github.com/greiman/SdFat)

FatFs is a generic filesystem module written by ChaN.
(http://elm-chan.org/fsw/ff/00index_e.html)
FatFs.h and FatFs.cpp wrap this module in order to use it with the Arduino IDE
It need more memory (flash and ram) than SdFat but allow non Ascii characters in files name.

SPIFFS is used to manage a file system stored on the flash memory of Esp8266 chip
(http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html)

## How to use FatLib

### Use object FAT_FS instead of sd (SdFat), FatFs (FatFs) or SpiFfs (SPIFFS)

 bool     FAT_FS.begin( uint8_t csPin, SPISettings spiSettings )
 int32_t  FAT_FS.capacity()
 int32_t  FAT_FS.free()
 bool     FAT_FS.mkdir( char * path )
 bool     FAT_FS.remove( char * path )
 bool     FAT_FS.rename( char * oldName, char * newName )
 bool     FAT_FS.exists( char * path )
 bool     FAT_FS.isDir( char * path )
 bool     FAT_FS.timeStamp( char * path, uint16_t year, uint8_t month, uint8_t day,
                            uint8_t hour, uint8_t minute, uint8_t second )
 bool     FAT_FS.getFileModTime( char * path, uint16_t * pdate, uint16_t * ptime )

### For directories, use class FAT_DIR instead of classes SdFile (SdFat), DirFs (FatFs) or Dir (SPIFFS)
 
 FAT_DIR  dir // declare directory object
 bool     dir.openDir( const char * dirPath )
 bool     dir.closeDir()
 bool     dir.nextFile()
 char *   dir.fileName()
 uint32_t dir.fileSize()
 bool     dir.isDir()
 uint16_t dir.fileModDate()
 uint16_t dir.fileModTime()
 
### To manage files, use class FAT_FILE instead of classes SdFile (SdFat), FileFs (FatFs), or File (SPIFFS)

 FAT_FILE file // declare file object 
 bool     file.open( char * fileName, uint8_t mode = O_READ )
 bool     file.close()
 uint32_t file.write( void * buf, uint32_t lbuf )
 int      file.writeString( char * buf )
 bool     file.writeChar( char car )
 uint32_t file.read( uint8_t * buf, uint32_t lbuf )
 int16_t  file.readString( char * buf, int lbuf )
 char     file.readChar()
 bool     file.seekSet( uint32_t cur )
 uint32_t file.fileSize()
 
Values for mode parameter are:
 O_READ , O_WRITE , O_RDWR , O_CREAT , O_APPEND
 
### See example FatLibDemo
### Arduino-FtpServer use FatLib
