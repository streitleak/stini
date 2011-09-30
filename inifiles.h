#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_SETTING 1024
#define DEFAULT_SECTION 256

class StIniFile 
{
    private:
        int      TotalSection;
        int      SecStartAndEnd[DEFAULT_SECTION][2];
        int      TotalSecLines;
        char     SectionName[DEFAULT_SECTION][DEFAULT_SETTING];
        char     *SectionLines[DEFAULT_SETTING];
        bool     CheckAllCharIsEnglish( char *CharArray );
        int      GetSectionLocation( char *SectionName );
        char     FileName[255];
        FILE     *fp;
    public:
        StIniFile( char *File );
        ~StIniFile( void );
        int      ReadInteger( char *Seciton, char *Lines, int Default );
        char    *ReadString( char *Section, char *Lines, char *Default );
        bool     WriteInteger( char *Section, char *Lines, int Value );
        bool     WriteString( char *Section, char *Lines, char *Value );
        bool     ReadSection( char *Section, char **Value, int *Length);
        void     Preprocess( char *prestr );
		int      ReadSectionLen( char *Section);
};

