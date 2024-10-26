/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include "bignum.h"




int main(){

	std::string num1{"2854561232151812515135155415151995011016620123102"};
	std::string num2{"2811515450095096987545654121215818151554054652"};
	bignum<uint64_t> a(num1,10);
	bignum<uint64_t> b(num2,10);
	a.print();
	b.print();
	bignum<uint64_t> x = a + b;
	x.print();

    return 0;
}
