/*
 * Class FatLib by Jean-Michel Gallego
 * Copyright (c) 2018 by Jean-Michel Gallego
 *
 * FatLib allow you to easily switch between libraries SdFat, FatFs and SpiFfs
 *   by modifying line '#define FAT_USE ...'
 *   at beginning of file FatLib.h
 *
 *   SdFat is an Arduino library written by William Greiman
 *    that provides read/write access to FAT16/FAT32
 *    file systems on SD/SDHC flash cards.
 *   
 *   FatFs module is a generic FAT file system for
 *    small embedded systems developed by ChaN.
 *
 *   SpiFfs permits to manage a file system using part of the
 *    flash memory of a Esp8266 chip
 *
 * Use class ExtFat instead of classes FatFs , SdFat or FS
 *     class ExtDir instead of classes DirFs , SdFile or Dir (for directories)
 *     class ExtFile instead of classes FileFs , SdFile or File (for files)
 *     object FAT_FS instead of sd , FatFs or SpiFfs
*/

#ifndef FAT_LIB_H
#define FAT_LIB_H

#define FAT_SDFAT  0  // library SdFat
#define FAT_FATFS  1  // library FatFs 
#define FAT_SPIFFS 2  // library SpiFfs for Esp8266

// Uncomment one of the following line to select a library 
//#define FAT_USE  FAT_SDFAT
#define FAT_USE  FAT_FATFS
//#define FAT_USE  FAT_SPIFFS

#if FAT_USE == FAT_SDFAT
  #ifdef ESP8266
    #error sorry, SdFat not yet implemented on Esp8266
  #endif
  #include <ExtSdFat.h>

  #define FAT_FS     sd
  #define FAT_FILE   ExtFile
  #define FAT_DIR    ExtDir

#elif FAT_USE == FAT_FATFS
  #include <FatFs.h>
  #define FAT_FS     FatFs
  #define FAT_FILE   FileFs
  #define FAT_DIR    DirFs
  
  #ifndef FF_MAX_LFN
    #define FF_MAX_LFN _MAX_LFN
  #endif
  #ifndef FA_OPEN_APPEND
    #define FA_OPEN_APPEND FA_OPEN_ALWAYS | FA__WRITTEN
  #endif

  #define O_READ     FA_READ
  #define O_WRITE    FA_WRITE
  #define O_RDWR     FA_READ | FA_WRITE
  #define O_CREAT    FA_CREATE_ALWAYS
  #define O_APPEND   FA_OPEN_APPEND

#elif FAT_USE == FAT_SPIFFS
  #ifndef ESP8266
    #error sorry, this board don't support FAT_SPIFFS library
  #endif
  #include <ExtSpiFfs.h>
  
  #define FAT_FS     SpiFfs
  #define FAT_FILE   ExtFile
  #define FAT_DIR    ExtDir

#else
  #error FAT_USE not defined or with incorrect value
#endif

#endif // FAT_LIB_H

