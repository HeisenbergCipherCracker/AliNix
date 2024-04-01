#include <gui/fonts/fontparser.hpp>
#include <vfs.hpp>
#include <string.hpp>
#include <log.hpp>


using namespace LIBHeisenKernel;

Font* FontParser::FromFile(char* filename)
{
    if(FileExists(filename) == false)
        return 0;

    uint32_t fileSize = GetFileSize(filename);
    if((int)fileSize == -1)
        return 0;

    uint8_t* filebuffer = new uint8_t[fileSize];
    if(ReadFile(filename, filebuffer) != 0) {
        delete filebuffer;
        return 0;
    }


    // Place CFF header over buffer
    CFFHeader* header = (CFFHeader*)filebuffer;
    if(header->Magic != 0xCFF || header->Version != 1) {
        delete filebuffer;
        return 0;
    }
    
    Print("Parsing font %s, size = %d Points\n", (char*)(filebuffer + header->FontNameOffset), header->FontSize);
    
    Font* result = new Font();
    result->data = filebuffer;
    result->name = (char*)(filebuffer + header->FontNameOffset);
    result->offsetTable = header->CharacterOffsets;
    result->size = header->FontSize;

    return result;
}