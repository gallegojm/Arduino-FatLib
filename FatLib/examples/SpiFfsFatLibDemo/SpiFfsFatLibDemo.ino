/*
  Demo for class FatLib by Jean-Michel Gallego
    using library SpiFfs on Esp8266
*/

#include <SPI.h>
#include <FatLib.h>

#if FAT_USE != FAT_SPIFFS
  #error    You must set
  #error      #define FAT_USE  FAT_SPIFFS
  #error    to run this demo
#endif

void setup()
{
  int res;

  Serial.begin( 115200 );
  do
    delay( 10 );
  while( Serial.available() && Serial.read() >= 0 );
  Serial.println( "Testing FatLib with library SpiFfs" );
  Serial.println( "Press a key to start" );
  while( Serial.read() < 0 )
    delay( 1 );
  delay( 400 );
  Serial.println();

  uint32_t millisBeginTest = millis();

  // Mount SD card
  Serial.print( "Mount the SD card:  " );
  res = FAT_FS.begin();
  printError( res, "Unable to mount SD card" );
  Serial.println( "ok" );

  // Show capacity and free space of SD card
  Serial.print( "Capacity of card:   " );
  Serial.print( FAT_FS.capacity());
  Serial.println( " MBytes" );
  Serial.print( "Free space on card: " );
  Serial.print( FAT_FS.free());
  Serial.println( " MBytes" );

  // List root directory
  Serial.println();
  Serial.println( "List of directories and files in root:" );
  FAT_DIR dir;
  if( dir.openDir( "/" ))
  {
    while( dir.nextFile())
    {
      Serial.print( dir.fileName());
      Serial.print( "\t" );
      if( dir.isDir() )
        Serial.println( "(Directory)" );
      else
      {
        Serial.print( dir.fileSize());
        Serial.println( " Bytes" );
      }
    }
    dir.closeDir();
  }
  
  // Create a file
  FAT_FILE file;
  char * fileName = "/New Directory/A new file.txt";
  Serial.println();
  Serial.print( "Create file '" );
  Serial.print( fileName );
  Serial.println( "'" );
  res = file.open( fileName, O_RDWR | O_CREAT );
  printError( res, "Error creating file" );
  
  // Writing text to a file and closing it
  Serial.println();
  Serial.println( "Write to file" );
  res = file.writeString( "Test d'ecriture dans un fichier\r\n" );
  if( res >= 0 )
    res = file.writeString( "Testing writing to file\r\n" );
  if( res >= 0 )
    res = file.writeString( "Prueba de escritura en un archivo\r\n" );
  // Write next line using writeBuffer()
  if( res >= 0 )
  {
    char * ps1 = "Test di scrittura su file\r\n";
    uint32_t nwrite = file.write( ps1, strlen( ps1 ));
    res = nwrite == strlen( ps1 );
  }
  // Write last line byte per byte,  using writeChar()
  if( res >= 0 )
  {
    char * ps2 = "Testes de gravacao em um arquivo\r\n";
    uint8_t pc = 0 ;
    while( res > 0 && ps2[ pc ] != 0 )
      res = file.writeChar( ps2[ pc ++ ] );
  }
  printError( ( res >= 0 ), "Error writing to file" );

  // Read content of file
  Serial.println();
  Serial.print( "Content of '" );
  Serial.print( fileName );
  Serial.println( "' is:" );
  char line[ 64 ];
  file.seekSet( 0 ); // set cursor to beginning of file
  while( file.readString( line, sizeof( line )) > 0 )
    Serial.println( line );

  // Close the file
  Serial.println();
  Serial.println( "Close the file" );
  res = file.close();
  printError( res, "Error closing file" );

  // Rename and move the file to root
  char * newName = "/TEST.TXT";
  Serial.println();
  Serial.print( "Rename file '" );
  Serial.print( fileName );
  Serial.print( "' to '" );
  Serial.print( newName );
  Serial.println( "'" );
  if( FAT_FS.exists( newName ))
    FAT_FS.remove( newName );
  res = FAT_FS.rename( fileName, newName );
  printError( res, "Error renaming file" );

  // Show content of directory
  listDir( "/" );

  // Open file for reading
  Serial.println();
  Serial.print( "Open file '" );
  Serial.print( newName );
  Serial.println( "'" );
  res = file.open( newName, O_READ );
  printError( res, "Error opening file" );

  // Read content of file and close it
  Serial.println();
  Serial.print( "Content of '" );
  Serial.print( newName );
  Serial.println( "' is:" );
  // Read first line byte per byte, just to demostrate use of readChar()
  char c;
  do
    Serial.print( c = file.readChar());
  while( c != '\n' && c != 0 );
  // Read next lines with readString() and print length of lines
  int l;
  while( ( l = file.readString( line, sizeof( line ))) > 0 )
  {
    Serial.print( line );
    Serial.print( " (Length is: " );
    Serial.print( l );
    Serial.println( ")" );
  }
  file.close();
    
  // Delete the file
  Serial.println();
  Serial.println( "Delete the file" );
  res = FAT_FS.remove( newName );
  printError( res, "Error deleting file" );

  Serial.println();
  Serial.print( "Test ok in " );
  Serial.print( millis() - millisBeginTest );
  Serial.println( " milliseconds" );
}

void loop()
{
  yield();
}

//    LIST DIRECTORY

void listDir( char * dirname )
{
  FAT_DIR d;
  
  Serial.print( "\nContent of directory '" );
  Serial.print( dirname );
  Serial.println( "' :" );
  if( d.openDir( dirname ))
  {
    uint8_t entrees = 0;
    while( d.nextFile())
    {
      Serial.println( d.fileName());
      entrees ++;
    }
    Serial.print( entrees );
    Serial.print( " files or directories in " );
    Serial.println( dirname );
  }
  d.closeDir();
}

//    PRINT ERROR STRING & STOP EXECUTION

// if ok is false, print the string msg and enter a while loop for ever

void printError( int ok, char * msg )
{
  if( ok )
    return;
  Serial.println( msg );
  while( true )
    yield();
}
