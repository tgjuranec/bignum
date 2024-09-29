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
	for(uint8_t c=0; c < SIZE; c++){
		util_add<uint64_t>(s[SIZE-1-c], a.s[SIZE-1-c], resh, resl);
		s[SIZE-1-c] = resl + carry;
		carry = resh;
	}
}


void bignum::mult(bignum &a){
	uint64_t resh{0}, resl{0};
	uint64_t bnTmp[SIZE*2] = {0};
	for(int8_t i = (SIZE-1); i >= 0; i--){
		for(int8_t j = (SIZE-1); j >= 0; j--){
			util_mult<uint64_t>(s[i], a.s[j], resh, resl);
			uint64_t resh_add;
			int shiftcarry = 0;

			do{
				util_add<uint64_t>(bnTmp[i+j+1-shiftcarry], resl, resh_add, bnTmp[i+j+1-shiftcarry]);
				shiftcarry++;
				resl = resh_add;
			}
			while(resh_add > 0 && (i+j+1-shiftcarry) > 0);
			shiftcarry = 0;
			do{
				util_add<uint64_t>(bnTmp[i+j-shiftcarry], resh, resh_add, bnTmp[i+j-shiftcarry]);
				shiftcarry++;
				resh = resh_add;
			}
			while(resh_add > 0 && (i+j-shiftcarry) > 0);
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
	nMultiplication = 0;
	bignum one{0,0,0,1};
	bignum zero{0,0,0,0};
	if(eq(zero)){
		for(int i = 0; i < SIZE; i++){
			s[i] = one.s[i];
		}
	}
	bignum tmpPower = one;
	bignum tmp = one;
	for (int word = (SIZE-1); word >=0 ; word--){
		for(uint64_t bit = 0; bit < (sizeof(uint64_t)*8); bit++){
			uint64_t isone = (a.s[word] & ((uint64_t)1 << (uint64_t) bit)); 	// those cast must be
																					// otherwise they are converted to 32-bit
																					// e.g. (1<<34) = 0x4
			if(isone){
				// "this" is fixed until the end of the algorythm
				// Temporary value is stored in the tmp variable
				// init tmpPower to base valuein *this
				tmpPower = *this;
				// rise tmpPower to power
				for(uint32_t nmult = 0; nmult < ((SIZE-1-word)*sizeof(uint64_t)*8 + bit); nmult++){
					tmpPower.mult(tmpPower);
					nMultiplication++;
				}
				// store temporary value by multiplying existing and tmpPower result
				tmp.mult(tmpPower);
			}
		}
	}
	for(int word = 0; word< SIZE; word++){
		s[word] = tmp.s[word];
	}
	std::cout << "Function pow(): nMultiplication = " << nMultiplication << "\n";

}

void bignum::powmod(bignum &a){
	bignum one{0,0,0,1};
	bignum zero{0,0,0,0};
	if(eq(zero)){
		for(int i = 0; i < SIZE; i++){
			s[i] = one.s[i];
		}
	}
	if(eq(one)){
		return;
	}
	bignum tmpPower = *this;
	bignum tmp = one;
	for (int word = (SIZE-1); word >=0 ; word--){
		for(uint64_t bit = 0; bit < (sizeof(uint64_t)*8); bit++){
			uint64_t isone = (a.s[word] & ((uint64_t)1 << (uint64_t) bit)); 	// those cast must be
																					// otherwise they are converted to 32-bit
																					// e.g. (1<<34) = 0x4
			// "this" is fixed until the end of the algorythm
			// Temporary value is stored in the tmp variable
			// with every increment (bit shift) multiply tmp
			if(isone){
				// store temporary value by multiplying existing and tmpPower result
				tmp.mult(tmpPower);
			}
			// square current value of tmpPower with every incrementation
			tmpPower.mult(tmpPower);
		}
	}
	for(int word = 0; word< SIZE; word++){
		s[word] = tmp.s[word];
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
