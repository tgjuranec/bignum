/*
 * bignum.h
 *
 *  Created on: Sep 22, 2024
 *      Author: tgjuranec
 */

#ifndef BIGNUM_H_
#define BIGNUM_H_
#include <cstdint>
#include "util.h"

constexpr uint8_t SIZE{4};

class bignum{
public:
	uint64_t s[SIZE];
	uint64_t nMultiplication{0};
	bignum():s{0,0,0,0}{	}
	bignum(uint64_t a, uint64_t b, uint64_t c, uint64_t d):s{a,b,c,d}{}
	void print();
	void add(bignum &a);
	void mult(bignum &a);
	void pow(bignum &a);
	void powmod(bignum &a);
	bool lt(bignum &a);
	bool eq(bignum &a);
	bool gt(bignum &a);
	void incr();
};





#endif /* BIGNUM_H_ */
