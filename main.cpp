/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include "bignum.h"




int main(){

    bignum<uint64_t> a("54065061213210840959509450561403514035403514035403540354",10);
    bignum<uint64_t> b("3514035403540354035140354035403540351403540354035403514035403540354",10);
    bignum<uint64_t> c("3514035403594419096353564876363049801964943868070807028070807080708",10);
	a.print();
	b.print();
	bignum<uint64_t> x = a * b;
	
	x.print();
	

    return 0;
}
