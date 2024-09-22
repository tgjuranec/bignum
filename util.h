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
    if((resl - a) != b){
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
    const UINT NIBBLEMASK = std::pow(2,HALFSIZE)-1;  //low nibble AND mask (e.g. '00001111' for UINT)
    UINT al = a & NIBBLEMASK;
    UINT ah = a >> HALFSIZE;
    UINT bl = b & NIBBLEMASK;
    UINT bh = b >> HALFSIZE;
    UINT ll = al * bl;
    UINT lh1 = al * bh;
    UINT lh2 = ah * bl;
    UINT hh = ah * bh;
    resl = ll + (lh1 << HALFSIZE) + (lh2 << HALFSIZE);
    UINT c = ((resl - ll) != ((lh1 << HALFSIZE) + (lh2 << HALFSIZE))) ? 1 : 0;
    resh = hh + (lh1 >> HALFSIZE) + (lh1 >> HALFSIZE) + c;
    return;
}

#endif /* UTIL_H_ */
