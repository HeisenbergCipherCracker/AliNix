/**
 * @author Ali Mirmohammad
 * @file power.c
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
 *  - Provides utility over the power management of the kernel.
*/
#include <alinix/power.h>
#include <alinix/core/power.h>
#include <alinix/types.h>
#include <alinix/enums.h>
#include <alinix/log.h>
#include <alinix/memory.h>
#include <alinix/types.h>
#include <alinix/apm.h>
#include <alinix/pit.h>
#include <alinix/port.h>

uint32_t* SMI_CMD;
uint8_t ACPI_ENABLE;
uint8_t ACPI_DISABLE;
uint32_t* PM1a_CNT;
uint32_t* PM1b_CNT;
uint16_t SLP_TYPa;
uint16_t SLP_TYPb;
uint16_t SLP_EN;
uint16_t SCI_EN;
uint8_t PM1_CNT_LEN;



unsigned int* acpiCheckRSDPtr(unsigned int* ptr)
{
    char* sig = "RSD PTR ";
    struct RSDPtr* rsdp = (struct RSDPtr*)ptr;
    uint8_t* bptr;
    uint8_t check = 0;
    int i;

    if(memcmp(sig, rsdp, 8) == 0)
    {
        // check checksum rsdpd
        bptr = (uint8_t*)ptr;
        for (i = 0; i < (int)sizeof(struct RSDPtr); i++)
        {
            check += *bptr;
            bptr++;
        }

        // found valid rsdpd   
        if (check == 0) {
            if (rsdp->Revision == 0)
                Log(Info, "ACPI Version 1");
            else
                Log(Info, "ACPI Version 2");
            
            return (unsigned int*)rsdp->RsdtAddress;
        }
    }

    return 0;
}



// finds the acpi header and returns the address of the rsdt
unsigned int* acpiGetRSDPtr(void)
{
    unsigned int* addr;
    unsigned int* rsdp;

    // search below the 1mb mark for RSDP signature
    for(addr = (unsigned int*)0x000E0000; (int)addr < 0x00100000; addr += 0x10/sizeof(addr))
    {
        rsdp = acpiCheckRSDPtr(addr);
        if (rsdp != 0)
            return rsdp;
    }

    // at address 0x40:0x0E is the RM segment of the ebda
    int ebda = *((short*)0x40E);   // get pointer
    ebda = ebda*0x10 & 0x000FFFFF;   // transform segment into linear address

    // search Extended BIOS Data Area for the Root System Description Pointer signature
    for(addr = (unsigned int*)ebda; (int)addr < ebda + 1024; addr += 0x10/sizeof(addr))
    {
        rsdp = acpiCheckRSDPtr(addr);
        if (rsdp != 0)
            return rsdp;
    }

    return 0;
}



// checks for a given header and validates checksum
int acpiCheckHeader(unsigned int* ptr, char* sig)
{
    if(memcmp(ptr, sig, 4) == 0)
    {
        char* checkPtr = (char*)ptr;
        int len = *(ptr + 1);
        char check = 0;
        while (0 < len--)
        {
            check += *checkPtr;
            checkPtr++;
        }
        if (check == 0)
            return 0;
    }
    return -1;
}

int acpiEnable(void)
{
    // check if acpi is enabled
    if ((inportw((unsigned int) PM1a_CNT) & SCI_EN) == 0 )
    {
        // check if acpi can be enabled
        if (SMI_CMD != 0 && ACPI_ENABLE != 0)
        {
            outportb((unsigned int)SMI_CMD, ACPI_ENABLE); // send acpi enable command
            // give 3 seconds time to enable acpi
            int i;
            for (i = 0; i < 300; i++)
            {
                if ((inportw((unsigned int)PM1a_CNT) & SCI_EN) == 1)
                    break;
                
                
            }
            if (PM1b_CNT != 0)
                for (; i < 300; i++ )
                {
                    if ((inportw((unsigned int) PM1b_CNT) & SCI_EN) == 1)
                        break;
                    
                    
                }
            if (i < 300) {
                Log(Info, "Enabled ACPI");
                return 0;
            } else {
                Log(Info, "Couldn't Enable ACPI");
                return -1;
            }
        } else {
            Log(Info, "No known way to Enable ACPI");
            return -1;
        }
    } else {
        Log(Info, "ACPI was already enabled");
        return 0;
    }
}



//
// bytecode of the \_S5 object
// -----------------------------------------
//        | (optional) |    |    |    |   
// NameOP | \          | _  | S  | 5  | _
// 08     | 5A         | 5F | 53 | 35 | 5F
//
// -----------------------------------------------------------------------------------------------------------
//           |           |              | ( SLP_TYPa   ) | ( SLP_TYPb   ) | ( Reserved   ) | (Reserved    )
// PackageOP | PkgLength | NumElements  | byteprefix Num | byteprefix Num | byteprefix Num | byteprefix Num
// 12        | 0A        | 04           | 0A         05  | 0A          05 | 0A         05  | 0A         05
//
//----this-structure-was-also-seen----------------------
// PackageOP | PkgLength | NumElements |
// 12        | 06        | 04          | 00 00 00 00
//
// (Pkglength bit 6-7 encode additional PkgLength bytes [shouldn't be the case here])
//
int initAcpi(void)
{
    unsigned int* ptr = acpiGetRSDPtr();

    // check if address is correct  ( if acpi is available on this pc )
    if (ptr != 0 && acpiCheckHeader(ptr, "RSDT") == 0)
    {
        // the RSDT contains an unknown number of pointers to acpi tables
        int entrys = *(ptr + 1);
        entrys = (entrys-36) / 4;
        ptr += 36/4;   // skip header information

        while (0<entrys--)
        {
            // check if the desired table is reached
            if (acpiCheckHeader((unsigned int*)*ptr, "FACP") == 0)
            {
                entrys = -2;
                struct FACP* facp = (struct FACP*)*ptr;
                
                if (acpiCheckHeader((unsigned int*)facp->DSDT, "DSDT") == 0)
                {
                    // search the \_S5 package in the DSDT
                    char *S5Addr = (char*)facp->DSDT + 36; // skip header
                    int dsdtLength = *(facp->DSDT+1) - 36;
                    while (0 < dsdtLength--)
                    {
                        if (memcmp(S5Addr, "_S5_", 4) == 0)
                            break;
                        S5Addr++;
                    }
                    // check if \_S5 was found
                    if (dsdtLength > 0)
                    {
                        // check for valid AML structure
                        if ((*(S5Addr-1) == 0x08 || (*(S5Addr - 2) == 0x08 && *(S5Addr-1) == '\\') ) && *(S5Addr+4) == 0x12 )
                        {
                            S5Addr += 5;
                            S5Addr += ((*S5Addr &0xC0) >> 6) + 2;   // calculate PkgLength size

                            if (*S5Addr == 0x0A)
                                S5Addr++;   // skip byteprefix
                            SLP_TYPa = *(S5Addr) << 10;
                            S5Addr++;

                            if (*S5Addr == 0x0A)
                                S5Addr++;   // skip byteprefix
                            SLP_TYPb = *(S5Addr) << 10;

                            SMI_CMD = facp->SMI_CMD;

                            ACPI_ENABLE = facp->ACPI_ENABLE;
                            ACPI_DISABLE = facp->ACPI_DISABLE;

                            PM1a_CNT = facp->PM1a_CNT_BLK;
                            PM1b_CNT = facp->PM1b_CNT_BLK;
                            
                            PM1_CNT_LEN = facp->PM1_CNT_LEN;

                            SLP_EN = 1 << 13;
                            SCI_EN = 1;

                            return 0;
                        } else {
                            Log(Info, "\\_S5 parse error");
                        }
                    } else {
                        Log(Info, "\\_S5 not present");
                    }
                } else {
                    Log(Info, "DSDT invalid");
                }
            }
            ptr++;
        }
        Log(Info, "no valid FACP present");
    } else {
        Log(Info, "no ACPI");
    }

    return -1;
}

void Initialize()
{
    //TODO: Find a better way to access physical ACPI memory
    EnablePagefaultAutoFix();
    initAcpi();
    DisablePagefaultAutoFix();
}

void Poweroff()
{
    // if(apm->Enabled) {
    //     Log(Info, "Shutdown via APM");
    //     apm->SetPowerState(APM_ALL_DEVICE, APM_POWER_OFF);
    // }
    
    // Else try ACPI

    // SCI_EN is set to 1 if acpi shutdown is possible
    if (SCI_EN == 0)
    {
        Log(Error, "ACPI poweroff not possible");
        return;
    }

    acpiEnable();

    Log(Info, "Sending Shutdown to ACPI");

    // send the shutdown command
    outportw((unsigned int)PM1a_CNT, SLP_TYPa | SLP_EN );
    if (PM1b_CNT != 0)
        outportw((unsigned int)PM1b_CNT, SLP_TYPb | SLP_EN );

    Log(Error, "ACPI poweroff failed, rebooting instead");
    for(int i = 5; i >= 0; i--)
    {
        Log(Info, "%d", i);
        // pit->Sleep(1000);
    }
    Reboot();
}

void Reboot()
{
    DisableInterrupts();

    /* flush the keyboard controller */
    unsigned temp;
    do
    {
        temp = inportb(0x64);
        if((temp & 0x01) != 0)
        {
            inportb(0x60);
            continue;
        }
    } while((temp & 0x02) != 0);

    /* Reset! */
    outportb(0x64, 0xFE);
}