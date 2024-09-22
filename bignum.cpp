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
		util_add<uint64_t>(s[SIZE-1-c], a.s[SIZE-1-c], resh, resl);
		s[SIZE-1-c] = resl + carry;
		carry = resh;
	}
}


void bignum::mult(bignum &a){
	uint64_t resh{0}, resl{0};
	uint64_t bnTmp[SIZE*2] = {0};
	for(uint8_t i = 0; i < SIZE; i++){
		for(uint8_t j = 0; j < SIZE; j++){
			util_mult<uint64_t>(s[i], a.s[j], resh, resl);
			bnTmp[i+j] += resh;
			bnTmp[i+j+1] += resl;
		}
	}
	for(uint8_t i = 0;i < SIZE; i++){
		s[i] = bnTmp[i+SIZE];
	}
}

/*
 * POWER FUNCTION
 * uses characteristic of powers a^(256*255*253) = ((a^256)^255)^253
 */
void bignum::pow(bignum &a){
	bignum tmp{0,0,0,1};
	for (uint8_t word = 0; word < SIZE ; word ++){
		for(uint8_t bit = 0; bit < sizeof(uint64_t); bit++){
			if((a.s[SIZE-1-word] & (1 << bit)) == 1 ){
				for(uint32_t nmult = 0; nmult < (word*sizeof(uint64_t)+bit); nmult++){
					tmp = *this;
					mult(tmp);
				}
			}
		}
	}

}

void bignum::print(){
	for(uint8_t i=0; i < SIZE; i++){
		std::cout << std::hex << std::uppercase << s[i] << " ";
	}
	std::cout << "\n";
}

/*
 * TEST IF NUMBERS ARE EQUAL
 */
bool bignum::eq(bignum &a){
	for(uint8_t i = 0; i < SIZE; i++){
		if(s[i] != a.s[i]){
			return false;
		}
	}
	return true;
}

/*
 * TEST IF IT IS LESS THAN ARGUMENT
 */
bool bignum::lt(bignum &a){
	for(uint8_t i = 0; i < SIZE; i++){
		if(s[i] < a.s[i]){
			return true;
		}
		else if(s[i] > a.s[i]){
			return false;
		}
	}
	return false;
}


bool bignum::gt(bignum &a){
	for(uint8_t i = 0; i < SIZE; i++){
		if(s[i] > a.s[i]){
			return true;
		}
		else if(s[i] < a.s[i]){
			return false;
		}
	}
	return false;
}

void bignum::incr(){
	bignum one{0,0,0,1};
	add(one);
}
