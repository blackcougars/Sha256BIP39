#include "sha256.h"
#include <iostream>
#include <bitset>


#define RIGTHROTATE32(a,b) (a >> b) | (a << (32 - b)) 


using namespace std;


// Константы sha256

#define h0 0x6a09e667
#define h1 0xbb67ae85
#define h2 0x3c6ef372
#define h3 0xa54ff53a
#define h4 0x510e527f
#define h5 0x9b05688c
#define h6 0x1f83d9ab
#define h7 0x5be0cd19


const uint32_t k [64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


uint32_t sha256BIP39(const uint32_t* first_part, const uint32_t* second_part , const uint32_t* three_part, const uint32_t* four_part)
{
	/*
	 * Функция sha256 урезанная для BIP39!
	 * Не подходит чтобы рассчитать sha256!
	 * Возращает первые 4 бита результата sha256 (checksum)
	*/
	uint32_t w[64];
	w[0] = *first_part;
	w[1] = *second_part;
	w[2] = *three_part;
    w[3] = *four_part;
	w[4] = 0b10000000000000000000000000000000;  // "Single 1"
	w[15] = 0b00000000000000000000000010000000; // Длина входных данных
	
	// Заполнение нулями (компилятор не сделает)
	for (int i = 5; i < 15; i++)
		w[i] = 0b00000000000000000000000000000000;
	// Изменение нулевых индексов 
	for (int i = 16; i < 64; i++)
	{
	//	w[i] = 0b00000000000000000000000000000000;
		uint32_t s0 = ( RIGTHROTATE32(w[i-15], 7) ) ^ ( RIGTHROTATE32(w[i-15], 18) ) ^ ( w[i-15] >> 3 );
		uint32_t s1 = ( RIGTHROTATE32(w[i-2], 17) ) ^ ( RIGTHROTATE32(w[i-2], 19)  ) ^ (w[i-2] >> 10); 	
		w[i] = w[i-16] + s0 + w[i-7] + s1;
	}
	// Подготовка значений
	uint32_t a = h0;
	uint32_t b = h1;
	uint32_t c = h2;
	uint32_t d = h3;
	uint32_t e = h4;
	uint32_t f = h5;
	uint32_t g = h6;
	uint32_t h = h7;
	
	// Цикл сжатия
	for (int i = 0; i < 64; i++)
	{
		uint32_t S1 = ( RIGTHROTATE32(e, 6) ) ^ ( RIGTHROTATE32(e, 11) ) ^ ( RIGTHROTATE32(e, 25) );
		uint32_t ch = (e & f) ^ ((~e) & g);
		uint32_t tmp1 = h + S1 + ch + k[i] + w[i];
		uint32_t S0 = ( RIGTHROTATE32(a, 2) ) ^ ( RIGTHROTATE32(a, 13) ) ^ ( RIGTHROTATE32(a, 22) );
		uint32_t maj = ( a & b ) ^ (a & c) ^ (b & c);
		uint32_t tmp2 = S0 + maj;
		
		h = g;
		g = f;
		f = e;
		e = d + tmp1;
		d = c;
		c = b;
		b = a;
		a = tmp1 + tmp2;
	}

	uint32_t r0 = h0 + a;
	//uint32_t r1 = h1 + b;
	//uint32_t r2 = h2 + c;
	//uint32_t r3 = h3 + d;
	//uint32_t r4 = h4 + e;
	//uint32_t r5 = h5 + f;
	//uint32_t r6 = h6 + g;
	//uint32_t r7 = h7 + h;
	return r0;
}

int main ()
{
	// test
	uint32_t a = 0b11100101010011111101110011110100;
	uint32_t b = 0b00101101010001001101110001100001;
	uint32_t c = 0b01100000100000001101101011100110;
	uint32_t d = 0b10100111101111000110100111100001;
	
	if (std::bitset<32> ("10001110110011101111010010010000") == ( sha256BIP39(&a, &b, &c, &d) ) )
		cout << "OK" << endl;
	else
		cout << std::bitset<32>( sha256BIP39(&a, &b, &c, &d) ) << endl;
	return 0;
}













