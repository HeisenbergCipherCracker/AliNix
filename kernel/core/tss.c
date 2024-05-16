/**
 * @author Ali Mirmohammad
 * @file tss.c
 * *************************************IMPORTANT ALINIX LICENSE TERM********************************************
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

/**
 * @abstraction:
 *  - Provide util over TSS.
*/

#include <alinix/core/tss.h>
#include <alinix/tss.h>
#include <alinix/enums.h>
#include <alinix/memory.h>
#include <alinix/gdt.h>

static struct TSSEntry tss;

void TSS_Install(uint32_t idx, uint32_t kernelSS, uint32_t kernelESP){
    memset(&tss,0,sizeof(struct TSSEntry));

    uint32_t base = (uint32_t)&tss;

    GGetDescriptor(idx, base, base + sizeof (struct TSSEntry), 0xE9, 0);

    /////////////
    // Init TSS
    memset ((void*) &tss, 0, sizeof (struct TSSEntry));

    tss.ss0 = kernelSS;
	tss.esp0 = kernelESP;
	tss.iomap = sizeof(struct TSSEntry);

    flush_tss();


}

void TSS_SetStack(uint32_t kernelSS, uint32_t kernelESP){
    tss.ss0 = kernelSS;
    tss.esp0 = kernelESP;
}

struct TSSEntry* TSS_GetCurrent(){
    return &tss;
}