#include <string.h>
#include <ctype.h>
#include "inifiles.h"

/*   string   to   low*/ 
inline char* strlwr( char* str ) 
{ 
      char* orig = str; 
      //   process   the   string 
      for( ; *str != '\0'; str++   ) 
      {
	      *str   =   tolower(*str);
      }
      return orig; 
}

StIniFile::StIniFile( char *File )
 {
      TotalSection  = 0;
      TotalSecLines = 0;

      strcpy( FileName, File );
      FileName[199]='\0';
      fp = NULL;
      for( int i=0;i<DEFAULT_SECTION;i++ )
      {    SecStartAndEnd[i][0] = 0;
           SecStartAndEnd[i][1] = 0;
      }

      for( int i=0;i<DEFAULT_SETTING;i++ )
      {
           SectionLines[i]=NULL;
      }
	  
      if( (fp=fopen( FileName, "r+" )) == NULL )
      {   fclose(fp);
          return;
      }
      else
      {    char tmpStr[DEFAULT_SETTING];
           char tmp[DEFAULT_SETTING];

            while( !feof(fp)  )
            {    
                 try
                 {
                      fgets( tmpStr, 1023, fp );
                      strlwr( tmpStr );
                 }
                 catch(...)
                 {    printf("INI fgets failure.\n\r");
                      fclose(fp);
                      return;
                 }
                 Preprocess( tmpStr );
                 try
                 {
                      if( tmpStr[0] == '[' )
                      {    if( CheckAllCharIsEnglish( tmpStr ) == false )
                           {    continue;
                           }
                           else
                           {    if( strlen(tmpStr) > 0 )
                                {    strncpy( &SectionName[TotalSection][0], &tmpStr[1], strlen(tmpStr)-2 );
                                }
                                else  continue;
                           }
                           if( TotalSection == 0 )
                           {    SecStartAndEnd[TotalSection][0] = 0;
                           }
                           else
                           {    SecStartAndEnd[TotalSection-1][1] = TotalSecLines;
                                SecStartAndEnd[TotalSection][0] = TotalSecLines;
                           }
                           TotalSection++;
                      }
                      else if(( tmpStr[0] == '#' )||( strlen( tmpStr ) == 0 ))
                      {    continue;
                      }
                      else
                      {    strcpy( tmp, tmpStr );
                           SectionLines[TotalSecLines] = new char [DEFAULT_SETTING];
                           strcpy( SectionLines[TotalSecLines], tmp );
                           TotalSecLines++;
                      }
                 }
                 catch(...)
                 {    printf("INI Initiate failure.\n\r");
                      fclose(fp);
                      return;
                 }
            };
            SecStartAndEnd[TotalSection-1][1] = TotalSecLines;
       }
 }
 
StIniFile::~StIniFile( void )
 {   
     for( int i=0;i<TotalSecLines;i++ )
      {   
		  if( SectionLines[i] != NULL )
		  {
			  delete [] SectionLines[i];
			  SectionLines[i] = NULL;
		  }
      }
     fclose( fp );
 }

int StIniFile::ReadInteger( char *Section, char *Lines, int Default )
 {    char tmp[DEFAULT_SETTING];
      char tmp2[255];
      char tmp3[255];
      int  rtn;
      bool found=false;
      char sect[255],line[255];
      strcpy( sect, Section );
      strcpy( line, Lines );
      strlwr( sect );
      strlwr( line );
try
 {
      for( int i=0;i<=TotalSection;i++ )
       {    if( strncmp( SectionName[i], sect, strlen( Section )) == 0 )
             {    for( int j=SecStartAndEnd[i][0];j<SecStartAndEnd[i][1];j++ )
                   {    strcpy( tmp, SectionLines[j] );
                        for( int k=0;k<strlen(tmp);k++ )
                         {    if( tmp[k] == '=' )
                               {    strncpy( tmp2, tmp, k );
                                    if( strncmp( tmp2, line, strlen( Lines )) == 0 )
                                     {   strncpy( tmp3, tmp+k+1, strlen(tmp) - k );
                                         rtn = atoi( tmp3 );
                                         found = true;
                                         break;
                                     }
                                    else break;
                               }
                              else if( tmp[k] == '#' )
                               {    break;
                               }
                          }
                         if( found == true )   break;
                   }
                  if( found == true )   break;
             }
            else    continue;
       }
 }
catch(...)
 {    printf("ReadInteger failure.\n\r");
      return Default;
 }
      if( found == false )       return Default;
      else                       return rtn;
 }
char *StIniFile::ReadString( char *Section, char *Lines, char *Default )
 {    static char tmp3[255];
	  char tmp[DEFAULT_SETTING];
      char tmp2[255];
      
      bool found=false;
      char sect[255],line[255];
      strcpy( sect, Section );
      strcpy( line, Lines );
      strlwr( sect );
      strlwr( line );
try
 {
      for( int i=0;i<=TotalSection;i++ )
       {    if( strncmp( SectionName[i], sect, strlen( Section )) == 0 )
             {    for( int j=SecStartAndEnd[i][0];j<SecStartAndEnd[i][1];j++ )
                   {    strcpy( tmp, SectionLines[j] );
                        for( int k=0;k<strlen(tmp);k++ )
                         {    if( tmp[k] == '=' )
                               {    strncpy( tmp2, tmp, k );
                                    if( strncmp( tmp2, line, strlen( Lines )) == 0 )
                                     {   strncpy( tmp3, tmp+k+1, strlen(tmp) - k );
                                         found = true;
                                         break;
                                     }
                                    else break;
                               }
                              else if( tmp[k] == '#' )
                               {    break;
                               }
                          }
                         if( found == true )   break;
                   }
                  if( found == true )   break;
             }
            else    continue;
       }
 }
catch(...)
 {    printf("ReadString failure.\n\r");
      return Default;
 }
      if( tmp3[strlen(tmp3)-1] == '\n' )    tmp3[strlen(tmp3)-1] = '\0';
      if( found == false )    return Default;
      else                    return tmp3;
 }
bool StIniFile::WriteInteger( char *Section, char *Lines, int Value )
 {    char tmp[DEFAULT_SETTING];
      bool found=false;
      fseek( fp, 0L, 0 );
try
 {
      while( !feof( fp ))
       {    fgets( tmp, 1023, fp );
            if( tmp[0] == '#' )
             {    if( strncmp( tmp, Lines, strlen( Lines )) == 0 )
                   {    fprintf( fp,"%s=%d", Lines, Value );
                        found = true;
                        break;
                   }
             }
            else  continue;
       };
 }
catch(...)
 {    printf("WriteInteger failure.\n\r");
      return false;
 }
      if( found == false )    return false;
      else                    return true;
 }
bool StIniFile::WriteString( char *Section, char *Lines, char *Value )
 {    char tmp[DEFAULT_SETTING];
      bool found=false;
      fseek( fp, 0L, 0 );
try
 {
      while( !feof( fp ))
       {    fgets( tmp, 1023, fp );
            if( tmp[0] == '#' )
             {    if( strncmp( tmp, Lines, strlen( Lines )) == 0 )
                   {    fprintf( fp,"%s=%s", Lines, Value );
                        found = true;
                        break;
                   }
             }
            else  continue;
       };
 }
catch(...)
 {    printf("WriteString failure.\n\r");
      return false;
 }
      if( found == false )   return false;
      else                   return true;
 }
int StIniFile::SectionExists( char *SecName)
{
	return GetSectionLocation( SecName);
}
int StIniFile::GetSectionLocation( char *SecName )
 {    for( int i=0;i<TotalSection;i++ )
       {    if( strncmp( SectionName[i], SecName, strlen( SecName )) == 0 )    return i;
       }
      return 0;
 }

bool StIniFile::CheckAllCharIsEnglish( char *CharArray )
 {    int Length;
      Length = strlen( CharArray );
      int GetNonEngCharCounts = 0;
      for( int i=0;i<Length;i++ )
       {    if((( CharArray[i] >= 0x65 )||( CharArray[i] <= 0x5a ))||(( CharArray[i] >= 0x61 )||( CharArray[i] <= 0x7a )))
             {   continue;
             }
            else GetNonEngCharCounts++;
       }
      if( GetNonEngCharCounts > 0 )     return false;
      else                              return true;
 }

void StIniFile::Preprocess( char *prestr )
 {    int len = strlen(prestr);
      for( int i=0;i<len;i++ )
       {    if(( prestr[i] == '\r' )||( prestr[i] == '\n' ))
             {    prestr[i] = '\0';
                  return;
             }
       }
 }
 
bool StIniFile::ReadSection( char *Section, char **Value, int *Length)
{     bool found=false;
      char sect[255],line[255];
      int ValueCount=0;
      strcpy( sect, Section );
      strlwr( sect );
try
 {
      for( int i=0;i<=TotalSection;i++ )
       {    if( strncmp( SectionName[i], sect, strlen( Section )) == 0 )
             {   *Length = SecStartAndEnd[i][1]-SecStartAndEnd[i][0]-1;
                 for( int j=SecStartAndEnd[i][0];j<SecStartAndEnd[i][1]-1;j++ )
                  {    
                       if( Value != NULL )
                       {
							strcpy( Value[ValueCount], SectionLines[j] );
                       }
                       ValueCount++;
                  }
                 break;
             }
            else    continue;
       }
 }
catch(...)
 {    printf("ReadSection failure.\n\r");
      return false;
 }
      return true;
}

int StIniFile::ReadSectionLen( char *Section) 
{
    bool found=false;
    char sect[255];
    int rtn=0;
    strcpy( sect, Section );
    strlwr( sect ); 
    try
    {
        for( int i=0;i<=TotalSection;i++ ) 
        {
             if( strncmp( SectionName[i], sect, strlen( Section )) == 0 )
             {
                 rtn = SecStartAndEnd[i][1]-SecStartAndEnd[i][0]-1; 
             }
             else continue; 
        }
    }
    catch(...) 
    {
         printf("ReadSection failure.\n\r");
         return rtn;
    }
    return rtn;
}
