# Makefile of Alinix kernelm licensed under GNU affero GPL
##########
# .s files are GAS assembly
# .asm files are nasm assembly
##########
#####################
#xHCI
# nec-usb-xhci
# qemu-xhci
#EHCI
# usb-ehci
# ich9-usb-ehci1
#UHCI
# ich9-usb-uhci1
# ich9-usb-uhci2
# ich9-usb-uhci3
# piix3-usb-uhci
# piix4-usb-uhci
# vt82c686b-usb-uhci
#OHCI
# sysbus-ohci
# pci-ohci
#######################

VERSION = 1
PATCHLEVEL = 4
SUBLEVEL = 4

# Check if the make version is exactly 3.80
ifeq ($(MAKE_VERSION),3.80)
    $(error This makefile requires GNU Make version 3.80. Your Make version is $(MAKE_VERSION))
endif

INCLUDEDIRS := include
QEMUOPTIONS := -boot d -device VGA,edid=on,xres=1024,yres=768 -trace events=../qemuTrace.txt -d cpu_reset #-readconfig qemu-usb-config.cfg -drive if=none,id=stick,file=disk.img -device usb-storage,bus=ehci.0,drive=stick

G++PARAMS := -m32 -g -D Alinix -I $(INCLUDEDIRS) -I arch/mips/include -Wall -fno-omit-frame-pointer -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-exceptions -fno-rtti -fno-leading-underscore -Wno-write-strings -fpermissive -Wno-unknown-pragmas
GCCPARAMS := -m32 -g -D Alinix -I $(INCLUDEDIRS) -I arch/mips/include -Wall -fno-omit-frame-pointer -nostdlib -fno-builtin -Wno-unknown-pragmas
ASPARAMS := --32
LDPARAMS := -m elf_i386

KRNLSRCDIR := kernel
KRNLOBJDIR := kernel/obj
KERNEL_SEC_DIR = security

SECURITY_SRCS := $(wildcard security/*.c)

# Create a list of object files corresponding to the source files
SECURITY_OBJS := $(patsubst $(KRNLSRCDIR)/%.c,$(KRNLOBJDIR)/%.o,$(SECURITY_SRCS))


KRNLFILES := $(shell find $(KRNLSRCDIR) -type f \( -name \*.cpp -o -name \*.s -o -name \*.asm -o -name \*.c \)) #Find all the files that end with .cpp/.s/.asm/.c
KRNLOBJS := $(patsubst $(KRNLSRCDIR)/%,$(KRNLOBJDIR)/%,$(patsubst %.cpp,%.o,$(patsubst %.s,%.o,$(patsubst %.asm,%.o,$(patsubst %.c,%.o,$(KRNLFILES)))))) #Replace the .cpp/.s/.asm/.c extension with .o

####################################
#C++ source files
####################################
$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.cpp
	mkdir -p $(@D)
	@echo "COMPILING $@"
	i686-elf-g++ $(G++PARAMS) -c -o $@ $<

####################################
#C source files
####################################
$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.c
	mkdir -p $(@D)
	@echo "COMPILING $@"
	i686-elf-gcc $(GCCPARAMS) -c -o $@ $<




####################################
#GDB Stub
####################################
$(KRNLOBJDIR)/gdb/i386-stub.o: $(KRNLSRCDIR)/gdb/i386-stub.c
	mkdir -p $(@D)
	@echo "COMPILING $@"
	i686-elf-gcc $(GCCPARAMS) -fleading-underscore -c -o $@ $<

####################################
#GAS assembly files
####################################
$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.s
	mkdir -p $(@D)
	i686-elf-as $(ASPARAMS) -o $@ $<

####################################
#NASM assembly files
####################################
$(KRNLOBJDIR)/%.o: $(KRNLSRCDIR)/%.asm
	mkdir -p $(@D)
	@echo "ASSEMBLING $@"
	nasm -f elf32 -O0 $< -o $@

# $(KRNLOBJDIR)/security/%.o: $(KRNLSRCDIR)/security/%.c
#     mkdir -p $(@D)
#     @echo "COMPILING $@"
#     $(CC) $(GCCPARAMS) -c -o $@ $<

$(SECURITY_OBJS)/%.o: (SECURITY_SRCS)/%.c
	mkdir -p $(@D)
	@echo "COMPILING $@"
	i686-elf-gcc $(GCCPARAMS) -c -o $@ $<



Alinix.bin: kernel/linker.ld $(KRNLOBJS)
	# cd security && $(MAKE) all
	cd drivers && $(MAKE) all 
	i686-elf-ld $(LDPARAMS) -T $< -o $@ $(KRNLOBJS)

install : Alinix.bin


Alinix.iso: Alinix.bin
	nm -a Alinix.bin | sort -d > isofiles/debug.sym || true
	cp -r isofiles/. iso || true
	mkdir iso/boot || true
	mkdir iso/boot/grub || true
	cp Alinix.bin iso/boot/Alinix.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	hdiutil makehybrid -o Alinix.iso iso -iso -joliet
	# rm -rf iso

all : Alinix.iso 

versionInfo:
	@echo "Kernel Version: $(VERSION).$(PATCHLEVEL).$(SUBLEVEL)"

# menuconfig: $(shell chmod +x dialog.sh && ./dialog.sh)


.PHONY: clean

clean:
	rm -rf $(KRNLOBJDIR) Alinix.bin Alinix01.iso