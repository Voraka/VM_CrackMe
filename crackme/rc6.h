#ifndef _RC6_H_
#define _RC6_H
#include <windows.h>
namespace RC6
{
	constexpr UINT w = 32;	/* word size in bits */
	constexpr UINT r = 20;	/* based on security estimates */
							//#define P32 0xB7E15163	/* Magic constants for key setup */
							//#define Q32 0x9E3779B9
	constexpr UINT P32 = 0xC85E63DF;
	constexpr UINT Q32 = 0xAF6B57C3;

	/* derived constants */
	constexpr int bytes = w / 8;
	//#define bytes   (w / 8)				/* bytes per word */
#define c_       ((b + bytes - 1) / bytes)	/* key in words, rounded up */
	constexpr UINT R24 = 2 * r + 4;
	//#define R24     (2 * r + 4)
	constexpr UINT lgw = 5;                       	/* log2(w) -- wussed out */

													/* Rotations */
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

	extern __forceinline void rc6_key_setup(unsigned char *K, int b);
	extern __forceinline void rc6_block_encrypt(unsigned int *pt, unsigned int *ct);
	extern __forceinline void rc6_block_decrypt(unsigned int *ct, unsigned int *pt);
}
#endif