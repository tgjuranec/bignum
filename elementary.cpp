/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include "bignum.h"




int main(){
	uint64_t c1[] = {0,0,0x2500,2586565};
	uint64_t c2[] = {0,0,0xffffffabcffff,0xfffffffffff};
	uint64_t c3[] = {0,0,0x25854654151 ,0XFFFFFFFFFFFFFFFF};
	bignum<uint64_t> a(c1,sizeof(c1)/sizeof(uint64_t));
	bignum<uint64_t> b(c2,sizeof(c2)/sizeof(uint64_t));
    bignum<uint64_t> c(c3,sizeof(c3)/sizeof(uint64_t));


    bignum<uint64_t> d = a;
    bignum<uint64_t> e = b;
    a.mult(b);
    a.print();
    a.div(b);
    a.print();
    d.print();

    b.mult(c);
    b.print();
    b.div(c);
    b.print();
    e.print();


    std::cout << a.eq(d) << "\n" << b.eq(e);





    return 0;
}
