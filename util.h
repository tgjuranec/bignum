/*
 * util.h
 *
 *  Created on: Sep 22, 2024
 *      Author: tgjuranec
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>

template <typename UINT>
void util_add(UINT a, UINT b, UINT &resh, UINT &resl){
    resl = a + b;
    const UINT FULLSIZE = sizeof(UINT)*8; //half word size in bits
    const UINT UINTMAX = (((1 << (FULLSIZE-1))-1) | ((1 << (FULLSIZE-1))));
    if((UINTMAX - a) < b){
        resh = 1;
    }
    else{
        resh = 0;
    }
    return;
}



template <typename UINT>
void util_mult(UINT a, UINT b, UINT &resh, UINT &resl){
    const UINT HALFSIZE = sizeof(UINT)*4; //half word size in bits
    constexpr UINT NIBBLEMASK = std::pow(2,HALFSIZE)-1;  //low nibble AND mask (e.g. '00001111' for UINT)
    const UINT UINTMAX = (((1 << (2*HALFSIZE-1))-1) | ((1 << (2*HALFSIZE-1))));
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
