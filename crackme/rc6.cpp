#include "stdafx.h"
#include "rc6.h"

namespace RC6
{
	unsigned int S[R24 - 1];		/* Key schedule */
	__forceinline void rc6_key_setup(unsigned char *K, int b)
	{
		int i, j, s, v;
		constexpr UINT uLen = (32 + bytes - 1) / bytes;
		unsigned int L[uLen]; /* Big enough for max b */
		unsigned int A, B;

		L[c_ - 1] = 0;
		for (i = b - 1; i >= 0; i--)
			L[i / bytes] = (L[i / bytes] << 8) + K[i];

		S[0] = P32;
		for (i = 1; i <= 2 * r + 3; i++)
			S[i] = S[i - 1] + Q32;

		A = B = i = j = 0;
		v = R24;
		if (c_ > v) v = c_;
		v *= 3;

		for (s = 1; s <= v; s++)
		{
			A = S[i] = ROTL(S[i] + A + B, 3);
			B = L[j] = ROTL(L[j] + A + B, A + B);
			i = (i + 1) % R24;
			j = (j + 1) % c_;
		}
	}

	__forceinline void rc6_block_encrypt(unsigned int *pt, unsigned int *ct)
	{
		unsigned int A, B, C, D, t, u, x;
		int i;

		A = pt[0];
		B = pt[1];
		C = pt[2];
		D = pt[3];
		B += S[0];
		D += S[1];
		for (i = 2; i <= 2 * r; i += 2)
		{
			t = ROTL(B * (2 * B + 1), lgw);
			u = ROTL(D * (2 * D + 1), lgw);
			A = ROTL(A ^ t, u) + S[i];
			C = ROTL(C ^ u, t) + S[i + 1];
			x = A;
			A = B;
			B = C;
			C = D;
			D = x;
		}
		A += S[2 * r + 2];
		C += S[2 * r + 3];
		ct[0] = A;
		ct[1] = B;
		ct[2] = C;
		ct[3] = D;
	}

	__forceinline void rc6_block_decrypt(unsigned int *ct, unsigned int *pt)
	{
		unsigned int A, B, C, D, t, u, x;
		int i;

		A = ct[0];
		B = ct[1];
		C = ct[2];
		D = ct[3];
		C -= S[2 * r + 3];
		A -= S[2 * r + 2];
		for (i = 2 * r; i >= 2; i -= 2)
		{
			x = D;
			D = C;
			C = B;
			B = A;
			A = x;
			u = ROTL(D * (2 * D + 1), lgw);
			t = ROTL(B * (2 * B + 1), lgw);
			C = ROTR(C - S[i + 1], t) ^ u;
			A = ROTR(A - S[i], u) ^ t;
		}
		D -= S[1];
		B -= S[0];
		pt[0] = A;
		pt[1] = B;
		pt[2] = C;
		pt[3] = D;
	}
}



