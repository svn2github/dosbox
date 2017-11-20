/*
 *  Copyright (C) mathfigure <mathfigure@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "redpill.h"
#ifdef WIN32
#include <windows.h>
#else
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define YELLOW "\x1b[1;33m"
#define GREEN "\x1b[1;32m"
#define RED "\x1b[1;31m"
#define NOCOLOR "\x1b[0m"
#endif

void quit(const char *msg)
{
#ifdef WIN32
	MessageBox(NULL, msg, "REDPILL FAIL", MB_OK);
#else
	printf(RED "FAIL.\n" NOCOLOR);
	perror(msg);
	exit(EXIT_FAILURE);
#endif
}

void* new_shm(int size, const char *sid)
{
#ifdef WIN32
	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, sid);
	if (hFileMap == NULL)
		quit("Shared memory failed!");
	return MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0,0,0);
#else
	printf(">Allocating shared memory: " YELLOW "%s" NOCOLOR " = %d bytes, ",sid,size);
	int fd = shm_open(sid, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1)
		quit("shm_open() failed!");
	ftruncate(fd, size);
	void *ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(ptr == MAP_FAILED) {
		close(fd);
		quit("mmap() failed!");
	}
	close(fd);
	printf(GREEN "OK\n" NOCOLOR);
	return ptr;
#endif
}

void delete_shm(const char *sid)
{
#ifdef WIN32

//TODO

#else

	printf(">Releasing shared memory: %s, ",sid);
	if(shm_unlink(sid) == -1)
		printf(RED "FAIL\n" NOCOLOR);
	else
		printf(GREEN "OK\n" NOCOLOR);
#endif
}

void *operator new[](std::size_t size, const char* sid) throw (std::bad_alloc)
{
	return new_shm(size, sid);
}

void operator delete[](void *p, const char* sid)
{
	delete_shm(sid);
}
