/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include "bignum.h"




int main(){

	bignum<uint64_t> a("2854561232151812515135155415151995010505105050201016620123102",10);
	bignum<uint64_t> b("2811515450095096987545654121215818151554054652",10);
	a.print();
	b.print();
	bignum<uint64_t> x = a + b;
	
	x.print();
	

    return 0;
}
