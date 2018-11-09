/*
 * Classes to wrap FS library
 * Copyright (c) 2018 by Jean-Michel Gallego
 */

 #ifdef ESP8266

#include <ExtSpiFfs.h>

SpiFfsClass SpiFfs;

bool ExtFile::open( char * fileName, uint8_t mode )
{
  char * pmode;
  
  if( mode == O_READ )
    pmode = "r";
  else if( mode == O_RDWR | O_CREAT )
    pmode = "w+";
  else
    pmode = "";
  f = SPIFFS.open( fileName, pmode );
  return f;
}

int16_t ExtFile::readString( char * buf, int lbuf )
{
  if( buf == NULL )
    return -1;
  int16_t nr = 0;
  int16_t c;
  do
  {
    c = f.read();
    if( c == -1 || c == '\n' || c == '\r' )
      break;
    buf[ nr ] = c;
  }
  while( ++ nr < lbuf - 1 );
  buf[ nr ] = '\0';
  while( c != -1 )
  {
    c = f.peek();
    if( c != '\n' && c != '\r' )
      break;
    c = f.read();
  }
  return nr;
};

#endif // ESP8266
