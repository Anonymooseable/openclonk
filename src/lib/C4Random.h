/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000  Matthes Bender
 * Copyright (c) 2001-2002  Sven Eberhardt
 * Copyright (c) 2005  Günther Brammer
 * Copyright (c) 2007  Peter Wortmann
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */

/* Network-safe random number generator */

#ifndef INC_C4Random
#define INC_C4Random

#include <ctime>
#include <C4Network2.h>

extern int RandomCount;
extern unsigned int RandomHold;

inline void FixedRandom(DWORD dwSeed)
{
	// for SafeRandom
	srand((unsigned)time(NULL));
	RandomHold=dwSeed;
	RandomCount=0;
}

#ifdef DEBUGREC
int Random(int iRange);
#else

inline void RandomHook(const char* file, const char* function, int line) {
	LogF("%s:%d: %s - Random no %d", file, line, function, RandomCount+1);
}

#define Random(iRange) ([](int range, const char* function){RandomHook(__FILE__, function, __LINE__); return __Random(range);}(iRange, __FUNCTION__))
inline int __Random(int iRange)
{
	RandomCount++;
	if (iRange==0) return 0;
	RandomHold = ((uint64_t)RandomHold * 16807) % 2147483647;
	return RandomHold % iRange;
}
#endif

inline unsigned int SeededRandom(unsigned int iSeed, unsigned int iRange)
{
	if (!iRange) return 0;
	iSeed = iSeed * 214013L + 2531011L;
	return (iSeed >> 16) % iRange;
}


inline int SafeRandom(int range)
{
	if (!range) return 0;
	return rand()%range;
}

#endif // INC_C4Random
