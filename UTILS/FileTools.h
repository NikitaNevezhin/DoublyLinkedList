#ifndef FileTools_H

#define FileTools_H

#include "PrintColor.h"
// #include  "STACK/stack.h"

struct FileInfo
{
    int lines;
    const char* filename;
    int size;
    char* buffer;
    char** indexes;
};

int     StrCount                (char* string, char symb);
    
int     GetFileSize             (const char* filename);
    
int     ReadFromFile            (const char* filename, char destination[], int size);
    
void    WriteDelimeter          (const char* filename);
    
void    CreateFileInfo          (FileInfo *file_info, const char* filename);
    
void    PrintFileInfo           (FileInfo *file_info);
    
char*   ChangeFileExtension     (const char* filename, const char* new_extension);

#endif