/**
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/
#include <alinix/init.h>
#include <alinix/types.h>
#include <alinix/kernel.h>
#include <alinix/port.h>
#include <alinix/unstar.h>
#include <alinix/memory.h>
#include <alinix/ulib.h>
#include <alinix/heap.h>

int oct2bin(unsigned char *str, int size){
    int n = 0;
    unsigned char *c = str;

    while (size-- > 0){
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}


/**
 * @brief Look up a file in a TAR archive and retrieve its contents.
 * 
 * @param archive The TAR archive stored as a byte array.
 * @param filename The name of the file to search for in the TAR archive.
 * @param out A pointer to a pointer that will store the contents of the found file.
 * @return The size of the file if found, 0 otherwise.
 */
int tar_lookup(unsigned char *archive, char *filename, char **out){
    unsigned char *ptr = archive; // Set pointer to the start of the archive

    while (!memcmp(ptr + 257 ,"ustar",5)) { // Check for valid ustar header
        int fileSize = oct2bin(ptr+0x7c,11); // Get file size from the header
        if (!memcmp(ptr, filename, strlen(filename) + 1)) {
            *out = ptr + 512; // Set the pointer to start of file
            return fileSize;
        }
        ptr += (((fileSize + 511) / 512) + 1) * 512;
    }
    return 0;
}
