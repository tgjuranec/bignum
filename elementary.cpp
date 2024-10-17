/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include "bignum.h"




int main(){
	uint64_t c1[] = {0,0,0,0xFFFFFFFFFFFFFFFF};
	uint64_t c2[] = {0,0,0,1};
	bignum<uint64_t> a(c1,4);
	bignum<uint64_t> acarry(c1,4);
    bignum<uint64_t> b(c2,4);

    a.add(b);
    a.print();

    acarry.add(b);
    acarry.print();





    return 0;
}
