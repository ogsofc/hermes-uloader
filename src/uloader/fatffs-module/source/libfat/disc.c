/*
 disc.c
 Interface to the low level disc functions. Used by the higher level
 file system code.
 
 Copyright (c) 2008 Michael "Chishm" Chisholm
	
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "disc.h"

/*
The list of interfaces consists of a series of name/interface pairs.
The interface is returned via a simple function. This allows for
platforms where the interface has to be "assembled" before it can
be used, like DLDI on the NDS. For cases where a simple struct
is available, wrapper functions are used.
The list is terminated by a NULL/NULL entry.
*/


extern DISC_INTERFACE __io_wiisd;
extern DISC_INTERFACE __io_usbstorage;

static const DISC_INTERFACE* get_io_wiisd (void) {
	return &__io_wiisd;
}

static const DISC_INTERFACE* get_io_usbstorage (void) {
	return &__io_usbstorage;
}

const INTERFACE_ID _FAT_disc_interfaces[] = {
	{ "sd",  get_io_wiisd  },
	{ "usb", get_io_usbstorage },
	{NULL, NULL}
};

#if 0
/* ====================== Wii ====================== */
#if   defined (__wii__)
#include <sdcard/wiisd_io.h>
#include <ogc/usbstorage.h>

static const DISC_INTERFACE* get_io_wiisd (void) {
	return &__io_wiisd;
}
static const DISC_INTERFACE* get_io_usbstorage (void) {
	return &__io_usbstorage;
}
const INTERFACE_ID _FAT_disc_interfaces[] = {
	{"sd", get_io_wiisd},
	{"usb", get_io_usbstorage},
	{NULL, NULL}
};	
	
/* ==================== Gamecube ==================== */
#elif defined (__gamecube__)
#include <sdcard/gcsd.h>
#include <sdcard/wiisd_io.h>

static const DISC_INTERFACE* get_io_gcsda (void) {
	return &__io_gcsda;
}
static const DISC_INTERFACE* get_io_gcsdb (void) {
	return &__io_gcsdb;
}

const INTERFACE_ID _FAT_disc_interfaces[] = {
	{"carda", get_io_gcsda},
	{"cardb", get_io_gcsdb},
	{NULL, NULL}
};	

/* ====================== NDS ====================== */
#elif defined (NDS)
#include <nds/arm9/dldi.h>

const INTERFACE_ID _FAT_disc_interfaces[] = {
	{"fat", dldiGetInternal},
	{NULL, NULL}
};	

/* ====================== GBA ====================== */
#elif defined (GBA)
#include <disc.h>

const INTERFACE_ID _FAT_disc_interfaces[] = {
	{"fat", discGetInterface},
	{NULL, NULL}
};	

#endif

#endif

bool _FAT_disc_readSectors (const DISC_INTERFACE* disc, sec_t sector, sec_t numSectors, void* buffer) {
bool ret;
    os_sync_after_write( (void *) buffer, numSectors<<9);
	ret=disc->readSectors (sector, numSectors, buffer);
	os_sync_before_read( (void *) buffer, numSectors<<9);


return ret;
}


/*
Write numSectors sectors to a disc, starting at sector. 
numSectors is between 1 and LIMIT_SECTORS if LIMIT_SECTORS is defined,
else it is at least 1
sector is 0 or greater
buffer is a pointer to the memory to read from
*/
bool _FAT_disc_writeSectors (const DISC_INTERFACE* disc, sec_t sector, sec_t numSectors, const void* buffer) {
	os_sync_after_write( (void *) buffer, numSectors<<9);
	return disc->writeSectors (sector, numSectors, buffer);
}
