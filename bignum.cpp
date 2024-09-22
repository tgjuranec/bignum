/*
 * bignum.cpp
 *
 *  Created on: Sep 22, 2024
 *      Author: tgjuranec
 */

#include "bignum.h"
#include <iostream>

void bignum::add(bignum &a){
	uint64_t resh{0}, resl{0}, carry{0};
	for(uint8_t c; c < SIZE; c++){
		::util_add<uint64_t>(s[SIZE-1-c], a.s[SIZE-1-c], resh, resl);
		s[SIZE-1-c] = resl + carry;
		carry = resh;
	}
}
void bignum::mult(bignum &a){
	uint64_t resh{0}, resl{0}, carry{0};
	for(uint8_t c; c < SIZE; c++){
		::util_mult<uint64_t>(s[SIZE-1-c], a.s[SIZE-1-c], resh, resl);
		s[SIZE-1-c] = resl + carry;
		carry = resh;
	}
}

void bignum::print(){
	for(uint8_t i=0; i < SIZE; i++){
		std::cout << std::hex << std::uppercase << s[i] << " ";
	}
	std::cout << "\n";
}
