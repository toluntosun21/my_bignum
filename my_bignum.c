#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_bignum.h"

//#define DEBUG

#ifdef DEBUG
#define PRINT_ARR(label, arr, len)\
	printf("\n%s\n", label);\
	for(int t = 0 ; t < (len); t++)\
		printf("0x%X ", arr[t]);\
	printf("\n\n")

#define PRINT_ARR_INT(label, arr, len)\
	printf("\n%s\n", label);\
	for(int t = 0 ; t < (len); t++)\
		printf("%u ", arr[t]);\
	printf("\n\n")
#else
#define PRINT_ARR(label, arr, len) {}
#define PRINT_ARR_INT(label, arr, len) {}
#endif

#ifdef COUNT_CORE_MULT
unsigned int g_coreMultNum = 0;
#endif

#define WORD_TO_BYTE(num) ( (num) << 2 )

#define POWER_OF_TWO(num) ( 1 << (num) )

int schoolBookMultUnequal(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precisionA, unsigned int p_precisionB)
{
	int i,j;
	uint64_t l_carry = 0, l_tempcarry = 0;

	uint64_t l_temp, l_tempH, l_tempL;
	unsigned int l_pre_res = p_precisionB + p_precisionA;


	for(i = 0; i < l_pre_res; i++)
	{
		p_res[i] = 0;
	}

	for(i = 0; i < p_precisionA; i++)
	{
		l_carry = 0;
		for(j = 0; j < p_precisionB; j++)
		{
			l_temp = ((uint64_t)p_a[i]) * ((uint64_t)p_b[j]);
#ifdef COUNT_CORE_MULT
			g_coreMultNum++;
#endif
			l_tempL = l_temp & 0xFFFFFFFF;
			l_tempH = l_temp >> 32;

			l_tempcarry = ((l_tempL + p_res[i + j] + l_carry) >> 32 ) + l_tempH;
			p_res[i + j] += l_tempL + l_carry;

			l_carry = l_tempcarry;
		}
		p_res[i + p_precisionB] = l_carry;
	}

	return 0;
}



int schoolBookMult(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	int i,j;
	uint64_t l_carry = 0, l_tempcarry = 0;

	uint64_t l_temp, l_tempH, l_tempL;


	for(i = 0; i < (p_precision << 1); i++)
	{
		p_res[i] = 0;
	}

	for(i = 0; i < p_precision; i++)
	{
		l_carry = 0;
		for(j = 0; j < p_precision; j++)
		{
			l_temp = ((uint64_t)p_a[i]) * ((uint64_t)p_b[j]);
#ifdef COUNT_CORE_MULT
			g_coreMultNum++;
#endif
			l_tempL = l_temp & 0xFFFFFFFF;
			l_tempH = l_temp >> 32;

			l_tempcarry = ((l_tempL + p_res[i + j] + l_carry) >> 32 ) + l_tempH;
			p_res[i + j] += l_tempL + l_carry;

			l_carry = l_tempcarry;
		}
		p_res[i + p_precision] = l_carry;
	}

	return 0;
}

/*
 * http://bioinfo.ict.ac.cn/~dbu/AlgorithmCourses/Lectures/Lec5-Fast-Division-Hasselstrom2003.pdf
 * algo 3.1
 * precision of p_quotient = 2
 */
int schoolBookDivSubRoutine(uint32_t *p_quotient, uint32_t *p_remainder, uint32_t *p_a, uint32_t *p_b, unsigned int p_pre_a, unsigned int p_pre_b)
{
	uint64_t l_q;
	uint32_t *l_T, *l_T_dummy;
	uint32_t *l_Bbeta;
	unsigned int l_pre_t, l_pre_a, l_pre_b;
	int i, l_ret = 0;
	volatile uint32_t l_dummy;
	const uint32_t l_1 = 1;

	l_pre_b = p_pre_b;
	while(p_b[l_pre_b - 1] == 0 && l_pre_b >= 1)l_pre_b--;

	l_pre_a = p_pre_a;
	while(p_a[l_pre_a - 1] == 0 && l_pre_a >= 1)l_pre_a--;

	// input check
	// p_pre_b = n
	if (l_pre_b == 0)
	{
		return - 1;//div by zero
	}
	//TODO: add check for a

	if (l_pre_a > l_pre_b + 1)
	{
		return -1;
	}
	else if ((p_b[l_pre_b - 1] & 0x80000000) == 0)
	{
		return -1;
	}

	// .1

	if (l_pre_a == (l_pre_b + 1))
	{
		l_Bbeta = malloc(WORD_TO_BYTE(l_pre_b + 1));//mult of beta and B, indeed a shift
		memcpy(l_Bbeta+1, p_b, WORD_TO_BYTE(l_pre_b));
		l_Bbeta[0] = 0;
		if (compare(p_a, l_Bbeta, l_pre_a/*l_pre_b + 1*/) >= 0)//if the MSB bit of A is 1
		{
			uint32_t *l_A;
			uint32_t l_beta[2] = {0,1};
			l_A = malloc(WORD_TO_BYTE(l_pre_a));
			sub_unequal(l_A, p_a, l_Bbeta, l_pre_a, l_pre_b + 1);

			l_ret = schoolBookDivSubRoutine(p_quotient, p_remainder, l_A, p_b, l_pre_a, l_pre_b);
			if (l_ret == 0)
			{
				l_ret = add_inplace(p_quotient, l_beta, 2);
			}
			free(l_A);
			free(l_Bbeta);
			return l_ret;
		}
		else
		{
			free(l_Bbeta);
		}

	}
	// .2, .3
	l_pre_t = l_pre_b + 1;
	// .3
	uint64_t l_temp = ((((uint64_t) p_a[l_pre_a - 1]) << 32) + p_a[l_pre_a - 2]);
	l_q = l_temp / p_b[l_pre_b - 1];

	if (l_q > 0xFFFFFFFF)
	{
		l_q = 0xFFFFFFFF;
	}
	else
	{
		l_q = l_q & 0xFFFFFFFF;;
	}

	// 4.
	l_T = malloc(WORD_TO_BYTE(l_pre_t));
	l_T_dummy = malloc(WORD_TO_BYTE(l_pre_t));
	l_ret = schoolBookMultUnequal(l_T, (uint32_t*) &l_q, p_b, 1, l_pre_b);

	if (l_ret != 0)
	{
		goto schoolBookDivHelper_end;
	}

	// 5. 6.
	for (i = 0; i < 2; i++)
	{
		if (compare(p_a, l_T, l_pre_a) == (-1))
		{
			l_q--;
			sub_inplaceUnequal(l_T, p_b, l_pre_t, l_pre_b);
		}
		else
		{
			l_dummy = l_q - 1;
			sub_unequal(l_T_dummy, l_T, p_b, l_pre_t, l_pre_b);
		}
	}

	// 7.
	p_quotient[0] = l_q & 0xFFFFFFFF;
	p_quotient[1] = l_q >> 32;

	sub_unequal(p_remainder, p_a, l_T, l_pre_a, l_pre_t);

schoolBookDivHelper_end:

	free(l_T_dummy);
	free(l_T);
	free(p_a);

	return l_ret;
}


// p_quotient, p_remainder precision returned is pre_a
// algo 3.2
int schoolBookDiv(uint32_t *p_quotient, uint32_t *p_remainder, uint32_t *p_a, uint32_t *p_b, unsigned int p_pre_a, unsigned int p_pre_b)
{
	int l_ret;
	unsigned int l_pre_t, l_pre_a, l_pre_b;
	uint64_t l_temp, l_tempH, l_tempL;
	uint32_t *l_Ap, *l_s;
	uint32_t *l_q, *l_q2, *l_r;
	unsigned int l_pre_ap, l_pre_s;

	l_pre_b = p_pre_b;
	while(p_b[l_pre_b - 1] == 0 && l_pre_b >= 1)l_pre_b--;

	if ((p_b[l_pre_b - 1] & 0x80000000) == 0)
	{
		return -1;
	}

	l_pre_a = p_pre_a;
	while(p_a[l_pre_a - 1] == 0 && l_pre_a >= 1)l_pre_a--;

	memset(p_quotient, 0x00, WORD_TO_BYTE(p_pre_a));
	memset(p_remainder, 0x00, WORD_TO_BYTE(p_pre_a));
	// .1
	if (l_pre_a < l_pre_b){
		memcpy(p_remainder, p_a, WORD_TO_BYTE(p_pre_a));
		return 0;
	}
	// .2
	else if (l_pre_a == l_pre_b)
	{

		if (compare(p_a, p_b, p_pre_a) == (-1))
		{
			memcpy(p_remainder, p_a, WORD_TO_BYTE(p_pre_a));
			return 0;
		}
		else
		{
			p_quotient[0] = 1;
			return sub(p_remainder, p_a, p_b, p_pre_a);
		}
	}
	// .3
	else if (l_pre_a == (l_pre_b + 1))
	{
		return schoolBookDivSubRoutine(p_quotient, p_remainder, p_a, p_b, p_pre_a, p_pre_b);
	}

	// .4
	l_pre_ap = l_pre_b + 1;
	l_pre_s = l_pre_a - l_pre_b - 1;

	l_Ap = p_a + l_pre_s;
	l_s = p_a;

	l_q = malloc(WORD_TO_BYTE(l_pre_a));
	l_q2 = malloc(WORD_TO_BYTE(l_pre_a));
	l_r = malloc(WORD_TO_BYTE(l_pre_a + (l_pre_a - l_pre_b - 1)));
	memset(l_r, 0x00, l_pre_a + (l_pre_a - l_pre_b - 1));

	l_ret = schoolBookDivSubRoutine(l_q, l_r + (l_pre_a - l_pre_b - 1) , l_Ap, p_b, l_pre_ap, p_pre_b);
	if(l_ret == (-1))
	{
		goto schoolBookDiv_end;
	}
	l_ret = schoolBookDiv(l_q2, p_remainder, l_Ap, p_b, l_pre_ap, p_pre_b);
	if(l_ret == (-1))
	{
		goto schoolBookDiv_end;
	}

schoolBookDiv_end:
	free(l_r);
	free(l_q2);
	free(l_q);

	return l_ret;
}

int add_unequal(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_pre_a, unsigned int p_pre_b)
{
	uint64_t l_temp;
	unsigned int l_pre_res = p_pre_a;
	if (p_pre_b > p_pre_a)
		l_pre_res = p_pre_b;

	for(int i = 0; i < l_pre_res + 1; i++)
		p_res[i] = 0;

	for(int i = 0; i < l_pre_res; i++)
	{
		l_temp = ( (i >= p_pre_a)? 0 : ((uint64_t)p_a[i]) ) +
				( (i >= p_pre_b)? 0 : ((uint64_t)p_b[i]) ) +
				((uint64_t)p_res[i]);

		p_res[i+1] = l_temp >> 32;

		p_res[i] = (l_temp & 0xFFFFFFFF);
	}

	return 0;
}

int add(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	return add_unequal(p_res, p_a, p_b, p_precision, p_precision);
}


int add_old(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	uint64_t l_temp;

	for(int i = 0; i < p_precision + 1; i++)
		p_res[i] = 0;

	for(int i = 0; i < p_precision; i++)
	{
		l_temp = ((uint64_t)p_a[i]) + ((uint64_t)p_b[i]);

		p_res[i+1] = l_temp >> 32;

		p_res[i] += (l_temp & 0xFFFFFFFF);
	}

	return 0;
}

int add_inplace(uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	uint32_t *l_res = malloc(WORD_TO_BYTE(p_precision + 1));
	memset(l_res, 0x00, WORD_TO_BYTE(p_precision + 1));
	add(l_res, p_a, p_b, p_precision);
	memcpy(p_a, l_res, WORD_TO_BYTE(p_precision));
	if(l_res[p_precision])
	{
		printf("unexptected length in add_inplace\n");
	}

	free(l_res);

	return 0;
}

/*
 * assume p_a is greater than p_b
 */
int sub(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	uint64_t l_temp;
	int i;
	int l_borrow = 0;

	memset(p_res, 0x00, WORD_TO_BYTE(p_precision));

	for(i = 0; i < p_precision; i++)
	{
		if (l_borrow && p_a[i] == 0)
		{
			p_res[i] = 0 - p_b[i];
		}
		else
		{
			l_temp = p_a[i];
			p_res[i] = p_a[i] - l_borrow - p_b[i];
			if (l_temp - l_borrow >= p_b[i])
			{
				l_borrow = 0;
			}
			else
			{
				l_borrow = 1;
			}
		}
	}

	if(l_borrow)
		return -1;
	else return 0;
}

int sub_inplace(uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	int ret = 0;
	uint32_t *l_res = malloc(WORD_TO_BYTE(p_precision + 1));//TODO: reduce the precision
	memset(l_res, 0x00, WORD_TO_BYTE(p_precision + 1));
	ret = sub(l_res, p_a, p_b, p_precision);
	memcpy(p_a, l_res, WORD_TO_BYTE(p_precision));

	free(l_res);

	return ret;
}

/* precision of res is p_precision_a */
int sub_unequal(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision_a, unsigned int p_precision_b)
{
	int ret = 0;
	uint32_t *l_b_;

	if (p_precision_a < p_precision_b)
	{
		return -1;
	}

	l_b_ = malloc(WORD_TO_BYTE(p_precision_a));

	memcpy(l_b_, p_b, WORD_TO_BYTE(p_precision_b));
	memset(l_b_ + p_precision_b, 0x00, WORD_TO_BYTE(p_precision_a - p_precision_b));

	memset(p_res, 0x00, WORD_TO_BYTE(p_precision_a));
	ret = sub(p_res, p_a, l_b_, p_precision_a);

	free(l_b_);

	return ret;
}


int sub_inplaceUnequal(uint32_t *p_a, uint32_t *p_b, unsigned int p_precision_a, unsigned int p_precision_b)
{
	int ret = 0;
	uint32_t *l_res, *l_b_;

	if (p_precision_a < p_precision_b)
	{
		return -1;
	}

	l_res = malloc(WORD_TO_BYTE(p_precision_a));
	l_b_ = malloc(WORD_TO_BYTE(p_precision_a));

	memcpy(l_b_, p_b, WORD_TO_BYTE(p_precision_b));
	memset(l_b_ + p_precision_b, 0x00, WORD_TO_BYTE(p_precision_a - p_precision_b));

	memset(l_res, 0x00, WORD_TO_BYTE(p_precision_a));
	ret = sub(l_res, p_a, l_b_, p_precision_a);
	memcpy(p_a, l_res, WORD_TO_BYTE(p_precision_a));

	free(l_b_);
	free(l_res);

	return ret;
}

int shiftDigitsLeft(uint32_t *p_num, unsigned int p_amount, unsigned int p_precision)
{
	int i;
	for(i = p_precision - 1; i >= 0; i--)
	{
		if (i >= p_amount)
			p_num[i] = p_num[i - p_amount];
		else
			p_num[i] = 0;
	}
	return 0;
}

int compare(uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	int i;
	for(i = p_precision - 1; i >= 0; i--)
	{
		if (p_a[i] > p_b[i])
			return 1;
		else if (p_a[i] < p_b[i])
			return -1;
	}
	return 0;
}

/*
 * works for odd p_precision
 */
int oneLevelKaratsubaMult(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	uint32_t *l_aL, *l_aH, *l_bL, *l_bH;
	uint32_t *l_r0, *l_r1, *l_r2, *l_aLH, *l_bLH;
	int i;
	const unsigned int l_leftPre = p_precision >> 1;
	const unsigned int l_rightPre = p_precision - l_leftPre;


	l_r0 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_r1 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_r2 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_aLH = malloc(WORD_TO_BYTE(p_precision + 1));
	l_bLH = malloc(WORD_TO_BYTE(p_precision + 1));

	memset(l_r0, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_r1, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_r2, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_aLH, 0x00, WORD_TO_BYTE(p_precision + 1));
	memset(l_bLH, 0x00, WORD_TO_BYTE(p_precision + 1));



	for(i = 0; i < p_precision << 1; i++)
		p_res[i] = 0;


	l_aL = p_a;
	l_aH = p_a + l_leftPre;

	l_bL = p_b;
	l_bH = p_b + l_leftPre;

	schoolBookMult(l_r0, l_aL, l_bL, l_leftPre);// r0 is l_leftPre*2
	schoolBookMult(l_r2, l_aH, l_bH, l_rightPre);// r2 is l_rightPre*2

	add_unequal(l_aLH, l_aL, l_aH, l_leftPre, l_rightPre);//alH is l_rightPre + 1
	add_unequal(l_bLH, l_bL, l_bH, l_leftPre, l_rightPre);//blH is l_rightPre + 1
	schoolBookMult(l_r1, l_aLH, l_bLH, l_rightPre + 1);//(l_rightPre + 1)*2, indeed 2*l_rightPre + 1 with better analysis
	sub_inplace(l_r1, l_r0, (l_rightPre << 1) + 1);
	sub_inplace(l_r1, l_r2, (l_rightPre << 1) + 1);//g_r1 is of p_precision at this point

	shiftDigitsLeft(l_r1, l_leftPre, p_precision << 1);
	shiftDigitsLeft(l_r2, l_leftPre << 1, p_precision << 1);

	add(p_res, l_r1, l_r2, p_precision << 1);
	add_inplace(p_res, l_r0, p_precision << 1);


	free(l_bLH);
	free(l_aLH);
	free(l_r2);
	free(l_r1);
	free(l_r0);

	return 0;
}


/*
 * works only if p_precision is divisible by 2
 */
int oneLevelKaratsubaMultwithSub(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	uint32_t *l_aL, *l_aH, *l_bL, *l_bH;
	uint32_t *l_r0, *l_r1, *l_r2, *l_aLH, *l_bLH, *l_inter_buf, *l_resExt;

	const unsigned int l_prehalf = p_precision >> 1;
	int l_minus_flag = 0;


	l_r0 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_r1 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_r2 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_aLH = malloc(WORD_TO_BYTE(p_precision + 1));
	l_bLH = malloc(WORD_TO_BYTE(p_precision + 1));
	l_inter_buf = malloc(WORD_TO_BYTE(p_precision << 1));
	l_resExt = malloc(WORD_TO_BYTE((p_precision << 1) + 1));

	memset(l_r0, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_r1, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_r2, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_aLH, 0x00, WORD_TO_BYTE(p_precision + 1));
	memset(l_bLH, 0x00, WORD_TO_BYTE(p_precision + 1));
	memset(l_inter_buf, 0x00, WORD_TO_BYTE(p_precision << 1));


	memset(p_res, 0x00, WORD_TO_BYTE(p_precision << 1));


	l_aL = p_a;
	l_aH = p_a + l_prehalf;

	l_bL = p_b;
	l_bH = p_b + l_prehalf;

	schoolBookMult(l_r0, l_aL, l_bL, l_prehalf);// r0 is p_precision
	schoolBookMult(l_r2, l_aH, l_bH, l_prehalf);// r2 is p_precision

	if (compare(l_aH, l_aL, l_prehalf) >=0)
	{
		sub(l_aLH, l_aH, l_aL, l_prehalf);//alH is l_prehalf
	}
	else
	{
		sub(l_aLH, l_aL, l_aH, l_prehalf);//alH is l_prehalf
		l_minus_flag = 1;
	}
	if (compare(l_bH, l_bL, l_prehalf) >=0)
	{
		sub(l_bLH, l_bH, l_bL, l_prehalf);//blH is l_prehalf
		l_minus_flag ^= 1;
	}
	else
	{
		sub(l_bLH, l_bL, l_bH, l_prehalf);//blH is l_prehalf
	}

	schoolBookMult(l_r1, l_aLH, l_bLH, l_prehalf);//(l_prehalf*2)

	add(l_inter_buf, l_r0, l_r2, p_precision);
	if (l_minus_flag)
	{
		if(sub_inplace(l_inter_buf, l_r1, p_precision) == -1)
		{
			printf("error in subtraction, karatsuba with sub\n");
			return -1;
		}
		memcpy(l_r1, l_inter_buf, p_precision << 2);
	}
	else
	{
		add_inplace(l_r1, l_inter_buf, p_precision);
	}

	shiftDigitsLeft(l_r1, l_prehalf, p_precision << 1);
	shiftDigitsLeft(l_r2, p_precision, p_precision << 1);

	add(l_resExt, l_r1, l_r2, p_precision << 1);
	add_inplace(l_resExt, l_r0, p_precision << 1);
	memcpy(p_res, l_resExt, WORD_TO_BYTE((p_precision << 1) + 1));

	free(l_resExt);
	free(l_inter_buf);
	free(l_bLH);
	free(l_aLH);
	free(l_r2);
	free(l_r1);
	free(l_r0);

	return 0;
}


/*
 * works for odd p_precision
 */
int twoLevelKaratsubaMult(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision)
{
	uint32_t *l_aL, *l_aH, *l_bL, *l_bH;
	uint32_t *l_r0, *l_r1, *l_r2, *l_aLH, *l_bLH;
	uint32_t *l_resExt;

	const unsigned int l_leftPre = p_precision >> 1;
	const unsigned int l_rightPre = p_precision - l_leftPre;

	l_r0 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_r1 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_r2 = malloc(WORD_TO_BYTE(p_precision << 1));
	l_aLH = malloc(WORD_TO_BYTE(p_precision << 2));
	l_bLH = malloc(WORD_TO_BYTE(p_precision << 2));
	l_resExt = malloc(WORD_TO_BYTE((p_precision << 1) + 1));

	memset(l_r0, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_r1, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_r2, 0x00, WORD_TO_BYTE(p_precision << 1));
	memset(l_aLH, 0x00, WORD_TO_BYTE(p_precision << 2));
	memset(l_bLH, 0x00, WORD_TO_BYTE(p_precision << 2));
	memset(l_resExt, 0x00, WORD_TO_BYTE((p_precision << 1) + 1));


	l_aL = p_a;
	l_aH = p_a + l_leftPre;

	l_bL = p_b;
	l_bH = p_b + l_leftPre;

	oneLevelKaratsubaMult(l_r0, l_aL, l_bL, l_leftPre);// r0 is l_leftPre*2
	oneLevelKaratsubaMult(l_r2, l_aH, l_bH, l_rightPre);// r2 is l_rightPre*2


	add_unequal(l_aLH, l_aL, l_aH, l_leftPre, l_rightPre);//alH is l_rightPre + 1
	add_unequal(l_bLH, l_bL, l_bH, l_leftPre, l_rightPre);//blH is l_rightPre + 1

	oneLevelKaratsubaMult(l_r1, l_aLH, l_bLH, l_rightPre + 1);//(l_rightPre + 1)*2, indeed 2*l_rightPre + 1 with better analysis
	sub_inplace(l_r1, l_r0, (l_rightPre << 1) + 1);
	sub_inplace(l_r1, l_r2, (l_rightPre << 1) + 1);//g_r1 is of p_precision at this point

	shiftDigitsLeft(l_r1, l_leftPre, p_precision << 1);
	shiftDigitsLeft(l_r2, l_leftPre << 1, p_precision << 1);

	add(l_resExt, l_r1, l_r2, p_precision << 1);
	add_inplace(l_resExt, l_r0, p_precision << 1);
	memcpy(p_res, l_resExt, WORD_TO_BYTE((p_precision << 1)));

	free(l_resExt);
	free(l_bLH);
	free(l_aLH);
	free(l_r2);
	free(l_r1);
	free(l_r0);
	return 0;
}

static int modmult_internal(uint32_t *p_res, uint32_t *p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision, int (*multiplier)(uint32_t*, uint32_t*, uint32_t*, unsigned int))
{
	uint32_t *l_TH, *l_T1H;
	uint32_t *l_T, *l_T1, *l_T2;

	l_T = malloc(WORD_TO_BYTE(p_precision << 3));
	l_T1 = malloc(WORD_TO_BYTE(p_precision << 3));
	l_T2 = malloc(WORD_TO_BYTE(p_precision << 3));

	memset(l_T, 0x00, WORD_TO_BYTE(p_precision << 3));
	memset(l_T1, 0x00, WORD_TO_BYTE(p_precision << 3));
	memset(l_T2, 0x00, WORD_TO_BYTE(p_precision << 3));

	multiplier(l_T, p_a, p_b, p_precision);


	l_TH = l_T + p_precision;
	multiplier(l_T1, l_TH, p_mu, p_precision + 2);//we have to put plus 2 since karatsuba with sub does not implemented with unequal halves (odd precision case)

	l_T1H = l_T1 + p_precision;
	multiplier(l_T2, l_T1H, p_p, p_precision);
	if (sub_inplace(l_T, l_T2, p_precision << 1) == -1)
	{
		printf("T can't be less than T2, returning error\n");
		return -1;
	}

	sub(l_T1, l_T, p_p, p_precision);

	if (l_T[p_precision] == 0 && compare(l_T, p_p, p_precision) < 0)
	{
		memcpy(p_res, l_T, p_precision << 2);
	}
	else // c' - p case
	{
		memcpy(p_res, l_T1, p_precision << 2);
	}

	free(l_T2);
	free(l_T1);
	free(l_T);

	return 0;
}


int modmult_karatsubasub(uint32_t *p_res, uint32_t *p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision)
{
	return modmult_internal(p_res, p_mu, p_a, p_b, p_p, p_precision, oneLevelKaratsubaMultwithSub);
}

int modmult_schoolbook(uint32_t *p_res, uint32_t *p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision)
{
	return modmult_internal(p_res, p_mu, p_a, p_b, p_p, p_precision, schoolBookMult);
}



int mongtgomery_reduction(uint32_t *p_res, uint32_t *p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision)
{
	uint32_t *l_T, *l_T1, *l_T2, *l_C, *l_C1, *l_pExt, *l_Ccpy;


	l_T = malloc(p_precision << 3);
	l_T1 = malloc(p_precision << 3);
	l_T2 = malloc(p_precision << 3);
	l_C = malloc(((p_precision << 1) + 1) << 2);
	l_C1 = malloc(((p_precision << 1) + 1) << 2);
	l_pExt = malloc(((p_precision << 1) + 1) << 2);


	twoLevelKaratsubaMult(l_T, p_a, p_b, p_precision);

	twoLevelKaratsubaMult(l_T1, l_T, p_mu, p_precision);

	twoLevelKaratsubaMult(l_T2, l_T1, p_p, p_precision);
	add(l_C, l_T2, l_T, p_precision<<1);
	l_Ccpy = l_C + p_precision;



	memset(l_pExt, 0x00, ((p_precision) + 1) << 2);
	memcpy(l_pExt, p_p, (p_precision) << 2);

	sub(l_C1, l_Ccpy, l_pExt, p_precision + 1);

	if (compare(l_Ccpy, l_pExt, p_precision + 1) < 0)
	{
		memcpy(p_res, l_Ccpy, p_precision << 2);
	}
	else // c' - p case
	{
		memcpy(p_res, l_C1, p_precision << 2);
	}

	free(l_pExt);
	free(l_C1);
	free(l_C);

	free(l_T2);

	free(l_T1);

	free(l_T);

	return 0;
}

int mongtgomery_reduction_wordlevel(uint32_t *p_res, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision)
{
	int i;
	uint32_t *l_T, *l_T2Lptr, *l_T3, *l_TOrg, *l_Tp, *l_pExt;
	uint32_t l_T2L;

	l_TOrg = malloc(p_precision << 4);
	l_T = l_TOrg;
	memset(l_T, 0x00, p_precision << 4);
	twoLevelKaratsubaMult(l_T, p_a, p_b, p_precision);

	l_T2Lptr = malloc(p_precision << 2);
	memset(l_T2Lptr, 0x00, p_precision << 2);

	l_T3 = malloc(p_precision << 3);

	for(i = 0; i < p_precision; i++)
	{
		memset(l_T3, 0x00, p_precision << 3);
#ifdef COUNT_CORE_MULT
		g_coreMultNum++;
#endif
		l_T2L = (((uint64_t)l_T[0]) * ((uint64_t)p_mu)) & 0xFFFFFFFF;
		l_T2Lptr[0] = l_T2L;
		schoolBookMultUnequal(l_T3, l_T2Lptr, p_p, 1, p_precision);//precision becomes (p_precision+1)
		add_inplace(l_T, l_T3, p_precision << 1);
		l_T ++;
	}

	l_pExt = malloc(((p_precision << 1) + 1) << 2);
	memset(l_pExt, 0x00, ((p_precision) + 1) << 2);
	memcpy(l_pExt, p_p, (p_precision) << 2);

	l_Tp = malloc(((p_precision) + 1) << 2);
	sub(l_Tp, l_T, l_pExt, p_precision + 1);

	if (compare(l_T, l_pExt, p_precision + 1) < 0)
	{
		memcpy(p_res, l_T, p_precision << 2);
	}
	else // c' - p case
	{
		memcpy(p_res, l_Tp, p_precision << 2);
	}

	/* below code is needed for one case, idk why???*/
	if (compare(p_res, p_p, p_precision) >= 0)
	{
		sub_inplace(p_res, p_p, p_precision);
	}
	else
	{
		sub_inplace(l_Tp, p_p, p_precision);//dummy
		(void)l_Tp;
	}

	free(l_Tp);
	free(l_pExt);
	free(l_T3);
	free(l_T2Lptr);
	free(l_TOrg);

	return 0;
}



static int shift_onedigitright_inplace(uint32_t *p_a, unsigned int p_precision)
{

	for(int i = 0; i < p_precision - 1; i++)
	{
		p_a[i] = p_a[i+1];
	}
	p_a[p_precision - 1] = 0;
	return 0;

}



int mongtgomery_reduction_mult_wordlevel(uint32_t *p_res, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision)
{
	int i;
	uint32_t *l_T, *l_TOrg, *l_ABI, *l_T1, *l_T3, *l_Tp, *l_pExt;
	uint64_t l_T2;

	l_TOrg = malloc((p_precision + 3) << 2);
	l_T = l_TOrg;

	l_ABI = malloc((p_precision + 1) << 2);

	l_T3 = malloc((p_precision + 2) << 2);



	memset(l_T, 0x00, (p_precision + 3) << 2);
	memset(l_T3, 0x00, (p_precision + 2) << 2);
	memset(l_ABI, 0x00, (p_precision + 1) << 2);

	for(i = 0; i < p_precision; i++)
	{
		//printf("iter %d\n", i);

		schoolBookMultUnequal(l_ABI, p_a + i, p_b, 1, p_precision);
		add_inplace(l_T, l_ABI, p_precision + 1);
		l_T1 = l_T;
#ifdef COUNT_CORE_MULT
		g_coreMultNum++;
#endif
		l_T2 = ((uint64_t)*l_T1) * ((uint64_t)p_mu);
		schoolBookMultUnequal(l_T3, (uint32_t*)&l_T2, p_p, 1, p_precision);
		add_inplace(l_T, l_T3, p_precision + 2);
		shift_onedigitright_inplace(l_T, p_precision + 2);
		//PRINT_ARR("T", l_T, p_precision + 3);
	}

	if(l_T[p_precision + 2])
	{
		printf("unexpected!!!\n");
	}

	l_pExt = malloc(((p_precision << 1) + 1) << 2);
	memset(l_pExt, 0x00, ((p_precision) + 1) << 2);
	memcpy(l_pExt, p_p, (p_precision) << 2);

	l_Tp = malloc(((p_precision) + 1) << 2);
	sub(l_Tp, l_T, l_pExt, p_precision + 1);

	if (compare(l_T, p_p, p_precision) < 0 && !l_T[p_precision])
	{
		memcpy(p_res, l_T, p_precision << 2);
	}
	else // c' - p case
	{
		memcpy(p_res, l_Tp, p_precision << 2);
	}


	free(l_Tp);
	free(l_pExt);
	free(l_T3);
	free(l_ABI);
	free(l_TOrg);

	return 0;
}

int mongtgomery_modmult_wordlevel_reduce(uint32_t *p_res, uint32_t *p_rsq, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision)
{
	int i;
	uint32_t *l_aMonty, *l_bMonty, *l_MontyRes, *l_One;

	l_One = malloc(p_precision << 2);
	for(i = 1; i < p_precision; i++)
	{
		l_One[i] = 0;
	}
	l_One[0] = 1;

	l_aMonty = malloc(p_precision << 2);
	l_bMonty = malloc(p_precision << 2);
	mongtgomery_reduction_wordlevel(l_aMonty, p_mu, p_a, p_rsq, p_p, p_precision);
	mongtgomery_reduction_wordlevel(l_bMonty, p_mu, p_b, p_rsq, p_p, p_precision);

	l_MontyRes = malloc(p_precision << 2);
	mongtgomery_reduction_wordlevel(l_MontyRes, p_mu, l_aMonty, l_bMonty, p_p, p_precision);
	mongtgomery_reduction_wordlevel(p_res, p_mu, l_MontyRes, l_One, p_p, p_precision);

	free(l_MontyRes);
	free(l_bMonty);
	free(l_aMonty);
	free(l_One);

	return 0;
}


int mongtgomery_modmult_wordlevel_multreduce(uint32_t *p_res, uint32_t *p_rsq, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision)
{
	int i;
	uint32_t *l_aMonty, *l_bMonty, *l_MontyRes, *l_One;

	l_One = malloc(p_precision << 2);
	for(i = 1; i < p_precision; i++)
	{
		l_One[i] = 0;
	}
	l_One[0] = 1;

	l_aMonty = malloc(p_precision << 2);
	l_bMonty = malloc(p_precision << 2);
	mongtgomery_reduction_mult_wordlevel(l_aMonty, p_mu, p_a, p_rsq, p_p, p_precision);
	mongtgomery_reduction_mult_wordlevel(l_bMonty, p_mu, p_b, p_rsq, p_p, p_precision);

	l_MontyRes = malloc(p_precision << 2);
	mongtgomery_reduction_mult_wordlevel(l_MontyRes, p_mu, l_aMonty, l_bMonty, p_p, p_precision);
	mongtgomery_reduction_mult_wordlevel(p_res, p_mu, l_MontyRes, l_One, p_p, p_precision);

	free(l_MontyRes);
	free(l_bMonty);
	free(l_aMonty);
	free(l_One);

	return 0;
}


int sliding_window_exponentiation(uint32_t *p_res, uint32_t *p_rsq, unsigned int p_k, uint32_t p_mu, uint32_t *p_g, uint32_t *p_e, uint32_t *p_p, unsigned int p_precision)
{
	uint32_t **l_precomputationTable;
	uint32_t *l_One, *l_w;
	unsigned int i, l_rowNum;
	int l_iBit, l_iWord;
	unsigned int l_window;
	unsigned int l_shiftAmount;

	l_One = malloc(WORD_TO_BYTE(p_precision));
	l_w = malloc(WORD_TO_BYTE(p_precision));

	for(i = 1; i < p_precision; i++)
	{
		l_One[i] = 0;
	}
	l_One[0] = 1;

	l_rowNum = POWER_OF_TWO(p_k - 1) + 1;
	l_precomputationTable = (uint32_t **)malloc(l_rowNum * sizeof(uint32_t*));

	for(i = 0; i < l_rowNum; i++)
	{
		l_precomputationTable[i] = (uint32_t *)malloc(p_precision * sizeof(uint32_t));
	}

	// l_precomputationTable[0] is for g * R
	mongtgomery_reduction_wordlevel(l_precomputationTable[0], p_mu, p_g, p_rsq, p_p, p_precision);
	PRINT_ARR_INT("g_monty", l_precomputationTable[0], p_precision);

	// l_precomputationTable[1] is for g^2 * R
	mongtgomery_reduction_wordlevel(l_precomputationTable[1], p_mu, l_precomputationTable[0], l_precomputationTable[0], p_p, p_precision);
	PRINT_ARR_INT("gsq_monty", l_precomputationTable[1], p_precision);

#define S_gMonty l_precomputationTable[0]
#define S_gSqMonty l_precomputationTable[1]



	for(i = 2; i < l_rowNum; i++)
	{
		if (i == 2)
			mongtgomery_reduction_wordlevel(l_precomputationTable[i], p_mu, S_gSqMonty, S_gMonty, p_p, p_precision);
		else
			mongtgomery_reduction_wordlevel(l_precomputationTable[i], p_mu, l_precomputationTable[i-1], S_gSqMonty, p_p, p_precision);

		PRINT_ARR_INT("g_next", l_precomputationTable[i], p_precision);

	}

	l_iBit = 31;
	l_iWord = p_precision - 1;

	while(!(p_e[l_iWord] >> l_iBit))l_iBit--;

	mongtgomery_reduction_wordlevel(l_w, p_mu, l_One, p_rsq, p_p, p_precision);

	// defines the mapping between the window and l_precomputationTable
	inline uint32_t * window2table(unsigned int p_window)
	{
		if (p_window >= POWER_OF_TWO(p_k))
		{
			printf("Error (invalid length) in mapping of window2table()");
			return NULL;
		}

		if (p_window == 1) return S_gMonty;
		else if (p_window == 2) return S_gSqMonty;
		else if (p_window % 2 == 1) return l_precomputationTable[1 + (p_window>>1)];
		else
		{
			printf("Error (even window) in mapping of window2table()");
			return NULL;
		}
	}

	inline void decrement_i()
	{
		if (l_iBit)l_iBit--;
		else
		{
			l_iBit = 31;
			l_iWord--;
		}
	}


	inline void increment_i()
	{
		if (l_iBit < 31)l_iBit++;
		else
		{
			l_iBit = 0;
			l_iWord++;
		}
	}


	while(l_iWord >= 0)
	{
		l_shiftAmount = 1;
		// fetch next
		l_window = (p_e[l_iWord] >> l_iBit) & 1;
		decrement_i();


		if (l_window)
		{
			// trying to find the window
			for (i = 1; (i < p_k) && (l_iWord >= 0); i++)
			{
				// fetch next
				unsigned int l_next = (p_e[l_iWord] >> l_iBit) & 1;
				decrement_i();
				l_window = (l_window << 1 ) | l_next;
				l_shiftAmount++;
			}

			while (!(l_window % 2))
			{
				l_window >>= 1;
				increment_i();
				l_shiftAmount--;
			}

		}

		for (i = 0; i < l_shiftAmount; i++)
		{
			mongtgomery_reduction_wordlevel(l_w, p_mu, l_w, l_w, p_p, p_precision);//take the square of l_w, l_shiftAmount times
		}

		if (l_window)
		{
			mongtgomery_reduction_wordlevel(l_w, p_mu, l_w, window2table(l_window), p_p, p_precision);
		}



	}

	PRINT_ARR_INT("res_monty", l_w, p_precision);


	mongtgomery_reduction_wordlevel(p_res, p_mu, l_w, l_One, p_p, p_precision);

	PRINT_ARR_INT("res", p_res, p_precision);

	for(i = 0; i < l_rowNum; i++)
	{
		free(l_precomputationTable[i]);
	}
	free(l_precomputationTable);
	free(l_w);
	free(l_One);

	return 0;
}



