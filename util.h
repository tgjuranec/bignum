/*
 * util.h
 *
 *  Created on: Sep 22, 2024
 *      Author: tgjuranec
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include <utility>
#include <tuple>

template <typename UINT>
void util_add(UINT a, UINT b, UINT &resh, UINT &resl){

    const UINT FULLSIZE = sizeof(UINT)*8; //half word size in bits
    const UINT UINTMAX = (UINT) (0UL - (UINT)1);
    resl = a + b;
    if((UINTMAX - a) < b){
        resh = 1;
    }
    else{
        resh = 0;
    }

    return;
}

template <typename UINT>
void util_add_carry(UINT a, UINT b, UINT carry, UINT &resh, UINT &resl){

    const UINT FULLSIZE = sizeof(UINT)*8; //full word size in bits
    const UINT UINTMAX = (UINT) (0UL - (UINT)1);

    resl = a + b;
    if((UINTMAX - a) < b){
        resh = 1;
    }
    else{
        resh = 0;
    }
    if((UINTMAX - carry) < resl){
    	resh++;
    }

    resl += carry;

}


template <typename UINT>
void util_mult(UINT a, UINT b, UINT &resh, UINT &resl){
    const UINT HALFSIZE = sizeof(UINT)*4; //half word size in bits
    constexpr UINT NIBBLEMASK = std::pow(2,HALFSIZE)-1;  //low nibble AND mask (e.g. '00001111' for UINT)
    const UINT UINTMAX = (UINT) (0UL - (UINT)1);
    UINT al = a & NIBBLEMASK;
    UINT ah = a >> HALFSIZE;
    UINT bl = b & NIBBLEMASK;
    UINT bh = b >> HALFSIZE;
    UINT ll = al * bl;
    UINT lh1 = al * bh;
    UINT lh2 = ah * bl;
    UINT hh = ah * bh;

    UINT lh1_shift = (lh1 << HALFSIZE);
    UINT lh2_shift = (lh2 << HALFSIZE);
    // THERE ARE 3 SUMMANDS
    // first we sum two and then check carry
    // after that we add 3rd summand
    resl = ll + lh1_shift;
    UINT c{0};
    if((UINTMAX - ll) < lh1_shift){
    	c++;
    }
    UINT tmpresl = resl;
    resl += lh2_shift;

    if((UINTMAX - tmpresl) < lh2_shift){
    	c++;
    }

    resh = hh + (lh1 >> HALFSIZE) + (lh2 >> HALFSIZE) + c;
    return;
}

template <typename UINT>
void util_shiftleft(UINT a, UINT nShifts, UINT &resh, UINT &resl){
    const UINT FULLSIZE = sizeof(UINT)*8; //full word size in bits
    // e.g. we need 111110000000000 to select 5 leftmost bits
    // nShifts = 5
    // 1 << (nShifts) = 100000
    // RIGHTMASK = 100000-1 = 11111 -> we need shift 'ones' to left by (FULLSIZE - nShifts )
    // LEFTMASK = ONES << (FULLSIZE - nShifts)    1111100000...
    UINT RIGHTMASK =  ((UINT)1 << nShifts) - (UINT)1;
    // UINT LEFTMASK = RIGHTMASK << (FULLSIZE - nShifts);
    resh = (a >> (FULLSIZE - nShifts)) & RIGHTMASK;
    resl = a<<nShifts;
}


template <typename UINT>
void util_shiftright(UINT a, UINT nShifts, UINT &resh, UINT &resl){
    const UINT FULLSIZE = sizeof(UINT)*8; //full word size in bits
        // e.g. we need 000011111 to select 5 rightmost bits
    // nShifts = 5
    // 1 << (nShifts) = 100000
    // RIGHTMASK = 100000-1 = 11111 
    UINT RIGHTMASK =  (1 << nShifts) - 1;
    resh = (a & RIGHTMASK) << (FULLSIZE - nShifts);
    resl = a>>nShifts;
}


template <typename UINT>
void util_subtract(UINT a, UINT b, UINT &resh, UINT &resl){
	if(a >= b){
		resl = a - b;
		resh = 0;
	}
	else{
		resl = a-b;
		resh = 1;
	}
}

template <typename UINT>
UINT util_pow_mod(UINT a, UINT b, UINT m)
{
	UINT i;
	UINT result = 1;
	UINT x = a % m;

	for (i=1; i<=b; i<<=1)
		{
		  x %= m;

			if ((b&i) != 0)
			  {
					result *= x;
					result %= m;
			  }

		   x *= x;
		}

  return result;
}

#endif /* UTIL_H_ */
