/**
 *	bobbycar - Modded Bobby Car toy for toddlers
 *	Copyright (C) 2020-2020 Johannes Bauer
 *
 *	This file is part of bobbycar.
 *
 *	bobbycar is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	bobbycar is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with bobbycar; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void *_sbrk(intptr_t increment);
void _exit(int status);
ssize_t _write_r(struct _reent *reent, int fd, const void *data, size_t length);
int _close_r(struct _reent *reent, int fd);
int _fstat_r(struct _reent *reent, int fd, struct stat *stat);
int _isatty_r(struct _reent *reent, int fd);
off_t _lseek_r(struct _reent *reent, int fd, off_t offset, int whence);
ssize_t _read_r(struct _reent *reent, int fd, const void *data, size_t length);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
