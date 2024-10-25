/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include "bignum.h"




int main(){
	uint64_t c1[] = {0,0,10,10};
	uint64_t c2[] = {0,0,20,20};
	uint64_t c3[] = {0,0,2,2};
	bignum<uint64_t> a(c1,sizeof(c1)/sizeof(uint64_t));
	bignum<uint64_t> b(c2,sizeof(c2)/sizeof(uint64_t));
    bignum<uint64_t> c(c3,sizeof(c3)/sizeof(uint64_t));

	bignum<uint64_t> x = b/c;
	bignum<uint64_t> y = a*b;
	bignum<uint64_t> l = b%c;
	x.print();
	y.print();
	l.print();
   

    return 0;
}
