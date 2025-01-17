#ifndef lint
static char Rcs_Id[] __attribute__ ((unused)) =
    "$Id: hash.c,v 1.25 2005/04/14 14:38:23 geoff Exp $";
#endif

/*
 * hash.c - a simple hash function for ispell
 *
 * Pace Willisson, 1983
 *
 * Copyright 1992, 1993, 1999, 2001, Geoff Kuenning, Claremont, CA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All modifications to the source code must be clearly marked as
 *    such.  Binary redistributions based on modified source code
 *    must be clearly marked as modified versions in the documentation
 *    and/or other materials provided with the distribution.
 * 4. The code that causes the 'ispell -v' command to display a prominent
 *    link to the official ispell Web site may not be removed.
 * 5. The name of Geoff Kuenning may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GEOFF KUENNING AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL GEOFF KUENNING OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * $Log: hash.c,v $
 * Revision 1.25  2005/04/14 14:38:23  geoff
 * Update license.
 *
 * Revision 1.24  2001/07/25 21:51:45  geoff
 * Minor license update.
 *
 * Revision 1.23  2001/07/23 20:24:03  geoff
 * Update the copyright and the license.
 *
 * Revision 1.22  1999/01/07 01:22:38  geoff
 * Update the copyright.
 *
 * Revision 1.21  1997/12/02  06:24:44  geoff
 * Get rid of some compile options that really shouldn't be optional.
 *
 * Revision 1.20  1994/01/25  07:11:34  geoff
 * Get rid of all old RCS log lines in preparation for the 3.1 release.
 *
 */

#include "config.h"
#include "ispell.h"
#include "proto.h"

int		hash P ((ichar_t * word, int hashtblsize));

/*
 * The following hash algorithm is due to Ian Dall, with slight modifications
 * by Geoff Kuenning to reflect the results of testing with the English
 * dictionaries actually distributed with ispell.
 */
#define HASHSHIFT   5

#define HASHUPPER(c)	mytoupper(c)

int hash (s, hashtblsize)
    register ichar_t *	s;
    register int	hashtblsize;
    {
    register long	h = 0;
    register int	i;

#ifdef ICHAR_IS_CHAR
    for (i = 4;  i--  &&  *s != 0;  )
	h = (h << 8) | HASHUPPER (*s++);
#else /* ICHAR_IS_CHAR */
    for (i = 2;  i--  &&  *s != 0;  )
	h = (h << 16) | HASHUPPER (*s++);
#endif /* ICHAR_IS_CHAR */
    while (*s != 0)
	{
	/*
	 * We have to do circular shifts the hard way, since C doesn't
	 * have them even though the hardware probably does.  Oh, well.
	 */
	h = (h << HASHSHIFT)
	  | ((h >> (32 - HASHSHIFT)) & ((1 << HASHSHIFT) - 1));
	h ^= HASHUPPER (*s++);
	}
    return (unsigned long) h % hashtblsize;
    }
