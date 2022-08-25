#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tests.h"
#include "my_bignum.h"
#include "test_vectors.h"


extern unsigned int g_coreMultNum;


static uint32_t g_buffer[MAX_PRECISION << 1];//result buffer

int basic_tests(){
	schoolBookMult(g_buffer, g_testvec1a, g_testvec1b, 2);
	if (memcmp(g_buffer, g_testmultres1, 4 << 2))
	{
		printf("Error in test 1 schoolbook\n");
	}
	oneLevelKaratsubaMult(g_buffer, g_testvec1a, g_testvec1b, 2);
	if (memcmp(g_buffer, g_testmultres1, 4 << 2))
	{
		printf("Error in test 1 karatsuba L1\n");
	}

	// TEST2
	schoolBookMult(g_buffer, g_testvec2a, g_testvec2b, 2);
	if (memcmp(g_buffer, g_testmultres2, 4 << 2))
	{
		printf("Error in test 2 mult\n");
	}
	oneLevelKaratsubaMult(g_buffer, g_testvec2a, g_testvec2b, 2);
	if (memcmp(g_buffer, g_testmultres2, 4 << 2))
	{
		printf("Error in test 2 karatsuba L1\n");
	}

	add(g_buffer, g_testvec2a, g_testvec2b, 2);
	if (memcmp(g_buffer, g_testaddres2, 3 << 2))
	{
		printf("Error in test 2 add\n");
	}

	sub(g_buffer, g_testvec2a, g_testvec2b, 2);
	if (memcmp(g_buffer, g_testsubres2, 2 << 2))
	{
		printf("Error in test 2 sub\n");
	}

	// TEST3
	schoolBookMult(g_buffer, g_testvec3a, g_testvec3b, 16);//precision becomes 32
	if (memcmp(g_buffer, g_testmultres3, 32 << 2))
	{
		printf("Error in test 3 mult\n");
	}
	oneLevelKaratsubaMult(g_buffer, g_testvec3a, g_testvec3b, 16);
	if (memcmp(g_buffer, g_testmultres3, 32 << 2))
	{
		printf("Error in test 3 karatsuba L1\n");
	}
	twoLevelKaratsubaMult(g_buffer, g_testvec3a, g_testvec3b, 16);
	if (memcmp(g_buffer, g_testmultres3, 32 << 2))
	{
		printf("Error in test 3 karatsuba L2\n");
	}
	oneLevelKaratsubaMultwithSub(g_buffer, g_testvec3a, g_testvec3b, 16);
	if (memcmp(g_buffer, g_testmultres3, 32 << 2))
	{
		printf("Error in test 3 karatsuba with Sub\n");
	}

	add(g_buffer, g_testvec3a, g_testvec3b, 16);//precision becomes 17
	if (memcmp(g_buffer, g_testaddres3, 17 << 2))
	{
		printf("Error in test 3 add\n");
	}

	sub(g_buffer, g_testvec3a, g_testvec3b, 16);
	if (memcmp(g_buffer, g_testsubres3, 16 << 2))
	{
		printf("Error in test 3 sub\n");
	}

	// TEST4
	schoolBookMult(g_buffer, g_testvec4a, g_testvec4b, 32);//precision becomes 64
	if (memcmp(g_buffer, g_testmultres4, 64 << 2))
	{
		printf("Error in test 4 mult\n");
	}
	oneLevelKaratsubaMult(g_buffer, g_testvec4a, g_testvec4b, 32);
	if (memcmp(g_buffer, g_testmultres4, 64 << 2))
	{
		printf("Error in test 4 karatsuba L1\n");
	}
	twoLevelKaratsubaMult(g_buffer, g_testvec4a, g_testvec4b, 32);
	if (memcmp(g_buffer, g_testmultres4, 64 << 2))
	{
		printf("Error in test 4 karatsuba L2\n");
	}
	oneLevelKaratsubaMultwithSub(g_buffer, g_testvec4a, g_testvec4b, 32);
	if (memcmp(g_buffer, g_testmultres4, 64 << 2))
	{
		printf("Error in test 4 karatsuba with Sub\n");
	}

	add(g_buffer, g_testvec4a, g_testvec4b, 32);//precision becomes 33
	if (memcmp(g_buffer, g_testaddres4, 33 << 2))
	{
		printf("Error in test 4 add\n");
	}

	sub(g_buffer, g_testvec4a, g_testvec4b, 32);
	if (memcmp(g_buffer, g_testsubres4, 32 << 2))
	{
		printf("Error in test 4 sub\n");
	}

	// TEST5
#ifdef COUNT_CORE_MULT
	g_coreMultNum = 0;
#endif
	schoolBookMult(g_buffer, g_testvec5a, g_testvec5b, 32);//precision becomes 64
	if (memcmp(g_buffer, g_testmultres5, 64 << 2))
	{
		printf("Error in test 4 mult\n");
	}
#ifdef COUNT_CORE_MULT
	printf("Core mult num in 1024-bit schoolBookMult : %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
	oneLevelKaratsubaMult(g_buffer, g_testvec5a, g_testvec5b, 32);
	if (memcmp(g_buffer, g_testmultres5, 64 << 2))
	{
		printf("Error in test 5 karatsuba L1\n");
	}
#ifdef COUNT_CORE_MULT
	printf("Core mult num in 1024-bit oneLevelKaratsubaMult : %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
	twoLevelKaratsubaMult(g_buffer, g_testvec5a, g_testvec5b, 32);
	if (memcmp(g_buffer, g_testmultres5, 64 << 2))
	{
		printf("Error in test 5 karatsuba L2\n");
	}
#ifdef COUNT_CORE_MULT
	printf("Core mult num in 1024-bit twoLevelKaratsubaMult : %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
	oneLevelKaratsubaMultwithSub(g_buffer, g_testvec5a, g_testvec5b, 32);
	if (memcmp(g_buffer, g_testmultres5, 64 << 2))
	{
		printf("Error in test 5 karatsuba with Sub\n");
	}
#ifdef COUNT_CORE_MULT
	printf("Core mult num in 1024-bit oneLevelKaratsubaMultwithSub : %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif

	add(g_buffer, g_testvec5a, g_testvec5b, 32);//precision becomes 33
	if (memcmp(g_buffer, g_testaddres5, 33 << 2))
	{
		printf("Error in test 5 add\n");
	}

	sub(g_buffer, g_testvec5a, g_testvec5b, 32);
	if (memcmp(g_buffer, g_testsubres5, 32 << 2))
	{
		printf("Error in test 5 sub\n");
	}

	// TEST6

	modmult_schoolbook(g_buffer, g_testvec6mu, g_testvec6a, g_testvec6b, g_testvec6p, 4);
	if (memcmp(g_buffer, g_testmodmultres6, 4 << 2))
	{
		printf("Error in test 6 mod-mult (schoolbook)\n");
	}
	modmult_karatsubasub(g_buffer, g_testvec6mu, g_testvec6a, g_testvec6b, g_testvec6p, 4);
	if (memcmp(g_buffer, g_testmodmultres6, 4 << 2))
	{
		printf("Error in test 6 mod-mult (karatsuba-sub)\n");
	}

	// TEST7
#ifdef COUNT_CORE_MULT
	g_coreMultNum = 0;
#endif
	modmult_schoolbook(g_buffer, g_testvec7mu, g_testvec7a, g_testvec7b, g_testvec7p, 32);
	if (memcmp(g_buffer, g_testmodmultres7, 32 << 2))
	{
		printf("Error in test 7 mod-mult (schoolbook)\n");
	}
#ifdef COUNT_CORE_MULT
	printf("Core mult num in 1024-bit modmult_schoolbook : %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
	modmult_karatsubasub(g_buffer, g_testvec7mu, g_testvec7a, g_testvec7b, g_testvec7p, 32);
	if (memcmp(g_buffer, g_testmodmultres7, 32 << 2))
	{
		printf("Error in test 7 mod-mult (karatsuba-sub)\n");
	}
#ifdef COUNT_CORE_MULT
	printf("Core mult num in 1024-bit modmult_karatsubasub : %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif

	// TEST8
	modmult_schoolbook(g_buffer, g_testvec8mu, g_testvec8a, g_testvec8b, g_testvec8p, 32);
	if (memcmp(g_buffer, g_testmodmultres8, 32 << 2))
	{
		printf("Error in test 8 mod-mult (schoolbook)\n");
	}
	modmult_karatsubasub(g_buffer, g_testvec8mu, g_testvec8a, g_testvec8b, g_testvec8p, 32);
	if (memcmp(g_buffer, g_testmodmultres8, 32 << 2))
	{
		printf("Error in test 8 mod-mult (karatsuba-sub)\n");
	}

	// TEST9
	modmult_schoolbook(g_buffer, g_testvec9mu, g_testvec9a, g_testvec9b, g_testvec9p, 32);
	if (memcmp(g_buffer, g_testmodmultres9, 32 << 2))
	{
		printf("Error in test 9 mod-mult (schoolbook)\n");
	}
	modmult_karatsubasub(g_buffer, g_testvec9mu, g_testvec9a, g_testvec9b, g_testvec9p, 32);
	if (memcmp(g_buffer, g_testmodmultres9, 32 << 2))
	{
		printf("Error in test 9 mod-mult (karatsuba-sub)\n");
	}

	return 0;//todo: return -1 in failure
}

#define TEST_MONTY_RED(ID) \
	montgomery_reduction(g_buffer, g_montytestvec##ID##mu, g_montytestvec##ID##a, g_montytestvec##ID##b, g_montytestvec##ID##p, 32);\
	if (memcmp(g_buffer, g_montytestmodmultres##ID, 32 << 2))\
	{\
		printf("Error in test %d monty-reduce\n", ID);\
	}

int montgomery_reduction_tests(){
#ifdef COUNT_CORE_MULT
	g_coreMultNum = 0;
#endif
	TEST_MONTY_RED(0)
#ifdef COUNT_CORE_MULT
	printf("Core mult num in montgomery reduction (using two level karatsuba internally): %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
	TEST_MONTY_RED(1)
	TEST_MONTY_RED(2)
	TEST_MONTY_RED(3)
	TEST_MONTY_RED(4)
	TEST_MONTY_RED(5)
	TEST_MONTY_RED(6)
	TEST_MONTY_RED(7)
	TEST_MONTY_RED(8)
	TEST_MONTY_RED(9)
	TEST_MONTY_RED(10)
	TEST_MONTY_RED(11)
	TEST_MONTY_RED(12)
	TEST_MONTY_RED(13)
	TEST_MONTY_RED(14)
	TEST_MONTY_RED(15)
	TEST_MONTY_RED(16)
	TEST_MONTY_RED(17)
	TEST_MONTY_RED(18)
	TEST_MONTY_RED(19)
	TEST_MONTY_RED(20)
	TEST_MONTY_RED(21)
	TEST_MONTY_RED(22)
	TEST_MONTY_RED(23)
	TEST_MONTY_RED(24)
	TEST_MONTY_RED(25)
	TEST_MONTY_RED(26)
	TEST_MONTY_RED(27)
	TEST_MONTY_RED(28)
	TEST_MONTY_RED(29)
	TEST_MONTY_RED(30)
	TEST_MONTY_RED(31)
	TEST_MONTY_RED(32)
	TEST_MONTY_RED(33)
	TEST_MONTY_RED(34)
	TEST_MONTY_RED(35)
	TEST_MONTY_RED(36)
	TEST_MONTY_RED(37)
	TEST_MONTY_RED(38)
	TEST_MONTY_RED(39)
	TEST_MONTY_RED(40)
	TEST_MONTY_RED(41)
	TEST_MONTY_RED(42)
	TEST_MONTY_RED(43)
	TEST_MONTY_RED(44)
	TEST_MONTY_RED(45)
	TEST_MONTY_RED(46)
	TEST_MONTY_RED(47)
	TEST_MONTY_RED(48)
	TEST_MONTY_RED(49)
	TEST_MONTY_RED(50)
	TEST_MONTY_RED(51)
	TEST_MONTY_RED(52)
	TEST_MONTY_RED(53)
	TEST_MONTY_RED(54)
	TEST_MONTY_RED(55)
	TEST_MONTY_RED(56)
	TEST_MONTY_RED(57)
	TEST_MONTY_RED(58)
	TEST_MONTY_RED(59)
	TEST_MONTY_RED(60)
	TEST_MONTY_RED(61)
	TEST_MONTY_RED(62)
	TEST_MONTY_RED(63)
	TEST_MONTY_RED(64)
	TEST_MONTY_RED(65)
	TEST_MONTY_RED(66)
	TEST_MONTY_RED(67)
	TEST_MONTY_RED(68)
	TEST_MONTY_RED(69)
	TEST_MONTY_RED(70)
	TEST_MONTY_RED(71)
	TEST_MONTY_RED(72)
	TEST_MONTY_RED(73)
	TEST_MONTY_RED(74)
	TEST_MONTY_RED(75)
	TEST_MONTY_RED(76)
	TEST_MONTY_RED(77)
	TEST_MONTY_RED(78)
	TEST_MONTY_RED(79)
	TEST_MONTY_RED(80)
	TEST_MONTY_RED(81)
	TEST_MONTY_RED(82)
	TEST_MONTY_RED(83)
	TEST_MONTY_RED(84)
	TEST_MONTY_RED(85)
	TEST_MONTY_RED(86)
	TEST_MONTY_RED(87)
	TEST_MONTY_RED(88)
	TEST_MONTY_RED(89)
	TEST_MONTY_RED(90)
	TEST_MONTY_RED(91)
	TEST_MONTY_RED(92)
	TEST_MONTY_RED(93)
	TEST_MONTY_RED(94)
	TEST_MONTY_RED(95)
	TEST_MONTY_RED(96)
	TEST_MONTY_RED(97)
	TEST_MONTY_RED(98)
	TEST_MONTY_RED(99)

	return 0;//todo: return -1 in failure
}

#define TEST_MONTY_RED_WORDLEVEL_R(ID) \
	montgomery_reduction_wordlevel(g_buffer, g_montywltestvec##ID##mu[0], g_montywltestvec##ID##a, g_montywltestvec##ID##b, g_montywltestvec##ID##p, 32);\
	if (memcmp(g_buffer, g_montywltestmodmultres##ID, 32 << 2))\
		{\
		printf("Error in test %d monty-wordlevel-reduce\n", ID);\
	}\


#define TEST_MONTY_RED_WORDLEVEL_MR(ID) \
		montgomery_reduction_mult_wordlevel(g_buffer, g_montywltestvec##ID##mu[0], g_montywltestvec##ID##a, g_montywltestvec##ID##b, g_montywltestvec##ID##p, 32);\
	if (memcmp(g_buffer, g_montywltestmodmultres##ID, 32 << 2))\
		{\
		printf("Error in test %d monty-wordlevel(mult&reduce)-reduce\n", ID);\
	}\

int montgomery_wl_reduction_tests(){
#ifdef COUNT_CORE_MULT
	g_coreMultNum = 0;
#endif
	TEST_MONTY_RED_WORDLEVEL_R(0)
#ifdef COUNT_CORE_MULT
	printf("Core mult num in montgomery reduction word-level(reduction) (using two-level karatsuba internally): %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
#ifdef COUNT_CORE_MULT
	g_coreMultNum = 0;
	TEST_MONTY_RED_WORDLEVEL_MR(0)
	printf("Core mult num in montgomery reduction word-level(mult&reduce): %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
	TEST_MONTY_RED_WORDLEVEL_R(1)


	TEST_MONTY_RED_WORDLEVEL_R(2)
	TEST_MONTY_RED_WORDLEVEL_R(3)
	TEST_MONTY_RED_WORDLEVEL_R(4)
	TEST_MONTY_RED_WORDLEVEL_R(5)
	TEST_MONTY_RED_WORDLEVEL_R(6)
	TEST_MONTY_RED_WORDLEVEL_R(7)
	TEST_MONTY_RED_WORDLEVEL_R(8)
	TEST_MONTY_RED_WORDLEVEL_R(9)
	TEST_MONTY_RED_WORDLEVEL_R(10)
	TEST_MONTY_RED_WORDLEVEL_R(11)
	TEST_MONTY_RED_WORDLEVEL_R(12)
	TEST_MONTY_RED_WORDLEVEL_R(13)
	TEST_MONTY_RED_WORDLEVEL_R(14)
	TEST_MONTY_RED_WORDLEVEL_R(15)
	TEST_MONTY_RED_WORDLEVEL_R(16)
	TEST_MONTY_RED_WORDLEVEL_R(17)
	TEST_MONTY_RED_WORDLEVEL_R(18)
	TEST_MONTY_RED_WORDLEVEL_R(19)
	TEST_MONTY_RED_WORDLEVEL_R(20)
	TEST_MONTY_RED_WORDLEVEL_R(21)
	TEST_MONTY_RED_WORDLEVEL_R(22)
	TEST_MONTY_RED_WORDLEVEL_R(23)
	TEST_MONTY_RED_WORDLEVEL_R(24)
	TEST_MONTY_RED_WORDLEVEL_R(25)
	TEST_MONTY_RED_WORDLEVEL_R(26)
	TEST_MONTY_RED_WORDLEVEL_R(27)
	TEST_MONTY_RED_WORDLEVEL_R(28)
	TEST_MONTY_RED_WORDLEVEL_R(29)
	TEST_MONTY_RED_WORDLEVEL_R(30)
	TEST_MONTY_RED_WORDLEVEL_R(31)
	TEST_MONTY_RED_WORDLEVEL_R(32)
	TEST_MONTY_RED_WORDLEVEL_R(33)
	TEST_MONTY_RED_WORDLEVEL_R(34)
	TEST_MONTY_RED_WORDLEVEL_R(35)
	TEST_MONTY_RED_WORDLEVEL_R(36)
	TEST_MONTY_RED_WORDLEVEL_R(37)
	TEST_MONTY_RED_WORDLEVEL_R(38)
	TEST_MONTY_RED_WORDLEVEL_R(39)
	TEST_MONTY_RED_WORDLEVEL_R(40)
	TEST_MONTY_RED_WORDLEVEL_R(41)
	TEST_MONTY_RED_WORDLEVEL_R(42)
	TEST_MONTY_RED_WORDLEVEL_R(43)
	TEST_MONTY_RED_WORDLEVEL_R(44)
	TEST_MONTY_RED_WORDLEVEL_R(45)
	TEST_MONTY_RED_WORDLEVEL_R(46)
	TEST_MONTY_RED_WORDLEVEL_R(47)
	TEST_MONTY_RED_WORDLEVEL_R(48)
	TEST_MONTY_RED_WORDLEVEL_R(49)
	TEST_MONTY_RED_WORDLEVEL_R(50)
	TEST_MONTY_RED_WORDLEVEL_R(51)
	TEST_MONTY_RED_WORDLEVEL_R(52)
	TEST_MONTY_RED_WORDLEVEL_R(53)
	TEST_MONTY_RED_WORDLEVEL_R(54)
	TEST_MONTY_RED_WORDLEVEL_R(55)
	TEST_MONTY_RED_WORDLEVEL_R(56)
	TEST_MONTY_RED_WORDLEVEL_R(57)
	TEST_MONTY_RED_WORDLEVEL_R(58)
	TEST_MONTY_RED_WORDLEVEL_R(59)
	TEST_MONTY_RED_WORDLEVEL_R(60)
	TEST_MONTY_RED_WORDLEVEL_R(61)
	TEST_MONTY_RED_WORDLEVEL_R(62)
	TEST_MONTY_RED_WORDLEVEL_R(63)
	TEST_MONTY_RED_WORDLEVEL_R(64)
	TEST_MONTY_RED_WORDLEVEL_R(65)
	TEST_MONTY_RED_WORDLEVEL_R(66)
	TEST_MONTY_RED_WORDLEVEL_R(67)
	TEST_MONTY_RED_WORDLEVEL_R(68)
	TEST_MONTY_RED_WORDLEVEL_R(69)
	TEST_MONTY_RED_WORDLEVEL_R(70)
	TEST_MONTY_RED_WORDLEVEL_R(71)
	TEST_MONTY_RED_WORDLEVEL_R(72)
	TEST_MONTY_RED_WORDLEVEL_R(73)
	TEST_MONTY_RED_WORDLEVEL_R(74)
	TEST_MONTY_RED_WORDLEVEL_R(75)
	TEST_MONTY_RED_WORDLEVEL_R(76)
	TEST_MONTY_RED_WORDLEVEL_R(77)
	TEST_MONTY_RED_WORDLEVEL_R(78)
	TEST_MONTY_RED_WORDLEVEL_R(79)
	TEST_MONTY_RED_WORDLEVEL_R(80)
	TEST_MONTY_RED_WORDLEVEL_R(81)
	TEST_MONTY_RED_WORDLEVEL_R(82)
	TEST_MONTY_RED_WORDLEVEL_R(83)
	TEST_MONTY_RED_WORDLEVEL_R(84)
	TEST_MONTY_RED_WORDLEVEL_R(85)
	TEST_MONTY_RED_WORDLEVEL_R(86)
	TEST_MONTY_RED_WORDLEVEL_R(87)
	TEST_MONTY_RED_WORDLEVEL_R(88)
	TEST_MONTY_RED_WORDLEVEL_R(89)
	TEST_MONTY_RED_WORDLEVEL_R(90)
	TEST_MONTY_RED_WORDLEVEL_R(91)
	TEST_MONTY_RED_WORDLEVEL_R(92)
	TEST_MONTY_RED_WORDLEVEL_R(93)
	TEST_MONTY_RED_WORDLEVEL_R(94)
	TEST_MONTY_RED_WORDLEVEL_R(95)
	TEST_MONTY_RED_WORDLEVEL_R(96)
	TEST_MONTY_RED_WORDLEVEL_R(97)
	TEST_MONTY_RED_WORDLEVEL_R(98)
	TEST_MONTY_RED_WORDLEVEL_R(99)

	return 0;
}



#define TEST_MONTY_MODMULT_WL_MR(ID) \
	montgomery_modmult_wordlevel_multreduce(g_buffer, g_montymodmulttestvec##ID##rsq, g_montymodmulttestvec##ID##mu[0], g_montymodmulttestvec##ID##a, g_montymodmulttestvec##ID##b, g_montymodmulttestvec##ID##p, 32);\
	if (memcmp(g_buffer, g_montymodmulttestres##ID, 32 << 2))\
	{\
		printf("Error in test %d monty mod mult wordlevel(mult&reduce)\n", ID);\
	}\

#define TEST_MONTY_MODMULT_WL_R(ID) \
	montgomery_modmult_wordlevel_reduce(g_buffer, g_montymodmulttestvec##ID##rsq, g_montymodmulttestvec##ID##mu[0], g_montymodmulttestvec##ID##a, g_montymodmulttestvec##ID##b, g_montymodmulttestvec##ID##p, 32);\
	if (memcmp(g_buffer, g_montymodmulttestres##ID, 32 << 2))\
	{\
		printf("Error in test %d monty mod mult wordlevel(reduce)\n", ID);\
	}\


int montgomery_modmult_tests(){
#ifdef COUNT_CORE_MULT
	g_coreMultNum = 0;
#endif
	TEST_MONTY_MODMULT_WL_MR(0)
#ifdef COUNT_CORE_MULT
	printf("Core mult num in montgomery wordlevel(mult&reduce) mod-multiplication: %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif

#ifdef COUNT_CORE_MULT
	g_coreMultNum = 0;
	TEST_MONTY_MODMULT_WL_R(0)
	printf("Core mult num in montgomery wordlevel(reduce) mod-multiplication (using two-level karatsuba internally): %u \n", g_coreMultNum);
	g_coreMultNum = 0;
#endif
	TEST_MONTY_MODMULT_WL_MR(1)
	TEST_MONTY_MODMULT_WL_MR(2)
	TEST_MONTY_MODMULT_WL_MR(3)
	TEST_MONTY_MODMULT_WL_MR(4)
	TEST_MONTY_MODMULT_WL_MR(5)
	TEST_MONTY_MODMULT_WL_MR(6)
	TEST_MONTY_MODMULT_WL_MR(7)
	TEST_MONTY_MODMULT_WL_MR(8)
	TEST_MONTY_MODMULT_WL_MR(9)
	TEST_MONTY_MODMULT_WL_MR(10)
	TEST_MONTY_MODMULT_WL_MR(11)
	TEST_MONTY_MODMULT_WL_MR(12)
	TEST_MONTY_MODMULT_WL_MR(13)
	TEST_MONTY_MODMULT_WL_MR(14)
	TEST_MONTY_MODMULT_WL_MR(15)
	TEST_MONTY_MODMULT_WL_MR(16)
	TEST_MONTY_MODMULT_WL_MR(17)
	TEST_MONTY_MODMULT_WL_MR(18)
	TEST_MONTY_MODMULT_WL_MR(19)
	TEST_MONTY_MODMULT_WL_MR(20)
	TEST_MONTY_MODMULT_WL_MR(21)
	TEST_MONTY_MODMULT_WL_MR(22)
	TEST_MONTY_MODMULT_WL_MR(23)
	TEST_MONTY_MODMULT_WL_MR(24)
	TEST_MONTY_MODMULT_WL_MR(25)
	TEST_MONTY_MODMULT_WL_MR(26)
	TEST_MONTY_MODMULT_WL_MR(27)
	TEST_MONTY_MODMULT_WL_MR(28)
	TEST_MONTY_MODMULT_WL_MR(29)
	TEST_MONTY_MODMULT_WL_MR(30)
	TEST_MONTY_MODMULT_WL_MR(31)
	TEST_MONTY_MODMULT_WL_MR(32)
	TEST_MONTY_MODMULT_WL_MR(33)
	TEST_MONTY_MODMULT_WL_MR(34)
	TEST_MONTY_MODMULT_WL_MR(35)
	TEST_MONTY_MODMULT_WL_MR(36)
	TEST_MONTY_MODMULT_WL_MR(37)
	TEST_MONTY_MODMULT_WL_MR(38)
	TEST_MONTY_MODMULT_WL_MR(39)
	TEST_MONTY_MODMULT_WL_MR(40)
	TEST_MONTY_MODMULT_WL_MR(41)
	TEST_MONTY_MODMULT_WL_MR(42)
	TEST_MONTY_MODMULT_WL_MR(43)
	TEST_MONTY_MODMULT_WL_MR(44)
	TEST_MONTY_MODMULT_WL_MR(45)
	TEST_MONTY_MODMULT_WL_MR(46)
	TEST_MONTY_MODMULT_WL_MR(47)
	TEST_MONTY_MODMULT_WL_MR(48)
	TEST_MONTY_MODMULT_WL_MR(49)
	TEST_MONTY_MODMULT_WL_MR(50)
	TEST_MONTY_MODMULT_WL_MR(51)
	TEST_MONTY_MODMULT_WL_MR(52)
	TEST_MONTY_MODMULT_WL_MR(53)
	TEST_MONTY_MODMULT_WL_MR(54)
	TEST_MONTY_MODMULT_WL_MR(55)
	TEST_MONTY_MODMULT_WL_MR(56)
	TEST_MONTY_MODMULT_WL_MR(57)
	TEST_MONTY_MODMULT_WL_MR(58)
	TEST_MONTY_MODMULT_WL_MR(59)
	TEST_MONTY_MODMULT_WL_MR(60)
	TEST_MONTY_MODMULT_WL_MR(61)
	TEST_MONTY_MODMULT_WL_MR(62)
	TEST_MONTY_MODMULT_WL_MR(63)
	TEST_MONTY_MODMULT_WL_MR(64)
	TEST_MONTY_MODMULT_WL_MR(65)
	TEST_MONTY_MODMULT_WL_MR(66)
	TEST_MONTY_MODMULT_WL_MR(67)
	TEST_MONTY_MODMULT_WL_MR(68)
	TEST_MONTY_MODMULT_WL_MR(69)
	TEST_MONTY_MODMULT_WL_MR(70)
	TEST_MONTY_MODMULT_WL_MR(71)
	TEST_MONTY_MODMULT_WL_MR(72)
	TEST_MONTY_MODMULT_WL_MR(73)
	TEST_MONTY_MODMULT_WL_MR(74)
	TEST_MONTY_MODMULT_WL_MR(75)
	TEST_MONTY_MODMULT_WL_MR(76)
	TEST_MONTY_MODMULT_WL_MR(77)
	TEST_MONTY_MODMULT_WL_MR(78)
	TEST_MONTY_MODMULT_WL_MR(79)
	TEST_MONTY_MODMULT_WL_MR(80)
	TEST_MONTY_MODMULT_WL_MR(81)
	TEST_MONTY_MODMULT_WL_MR(82)
	TEST_MONTY_MODMULT_WL_MR(83)
	TEST_MONTY_MODMULT_WL_MR(84)
	TEST_MONTY_MODMULT_WL_MR(85)
	TEST_MONTY_MODMULT_WL_MR(86)
	TEST_MONTY_MODMULT_WL_MR(87)
	TEST_MONTY_MODMULT_WL_MR(88)
	TEST_MONTY_MODMULT_WL_MR(89)
	TEST_MONTY_MODMULT_WL_MR(90)
	TEST_MONTY_MODMULT_WL_MR(91)
	TEST_MONTY_MODMULT_WL_MR(92)
	TEST_MONTY_MODMULT_WL_MR(93)
	TEST_MONTY_MODMULT_WL_MR(94)
	TEST_MONTY_MODMULT_WL_MR(95)
	TEST_MONTY_MODMULT_WL_MR(96)
	TEST_MONTY_MODMULT_WL_MR(97)
	TEST_MONTY_MODMULT_WL_MR(98)
	TEST_MONTY_MODMULT_WL_MR(99)
	return 0;
}

#define K (6)


#define TEST_SLIDING_WINDOW_EXP(ID) \
	sliding_window_exponentiation(g_buffer, g_windowingexp##ID##rsq, K, g_windowingexp##ID##mu[0], g_windowingexp##ID##g, g_windowingexp##ID##e, g_windowingexp##ID##p, 32);\
	if (memcmp(g_buffer, g_windowingexp##ID##res, 32 << 2))\
	{\
		printf("Error in test %d sliding window exp\n", ID);\
	}\

int sliding_window_exponentiation_tests(){

	TEST_SLIDING_WINDOW_EXP(1)
	TEST_SLIDING_WINDOW_EXP(2)
	TEST_SLIDING_WINDOW_EXP(3)
	TEST_SLIDING_WINDOW_EXP(4)
	TEST_SLIDING_WINDOW_EXP(5)
	TEST_SLIDING_WINDOW_EXP(6)
	TEST_SLIDING_WINDOW_EXP(7)
	TEST_SLIDING_WINDOW_EXP(8)
	TEST_SLIDING_WINDOW_EXP(9)
	TEST_SLIDING_WINDOW_EXP(10)
	TEST_SLIDING_WINDOW_EXP(11)
	TEST_SLIDING_WINDOW_EXP(12)
	TEST_SLIDING_WINDOW_EXP(13)
	TEST_SLIDING_WINDOW_EXP(14)
	TEST_SLIDING_WINDOW_EXP(15)
	TEST_SLIDING_WINDOW_EXP(16)
	TEST_SLIDING_WINDOW_EXP(17)
	TEST_SLIDING_WINDOW_EXP(18)
	TEST_SLIDING_WINDOW_EXP(19)
	TEST_SLIDING_WINDOW_EXP(20)
	TEST_SLIDING_WINDOW_EXP(21)
	TEST_SLIDING_WINDOW_EXP(22)
	TEST_SLIDING_WINDOW_EXP(23)
	TEST_SLIDING_WINDOW_EXP(24)
	TEST_SLIDING_WINDOW_EXP(25)
	TEST_SLIDING_WINDOW_EXP(26)
	TEST_SLIDING_WINDOW_EXP(27)
	TEST_SLIDING_WINDOW_EXP(28)
	TEST_SLIDING_WINDOW_EXP(29)
	TEST_SLIDING_WINDOW_EXP(30)
	TEST_SLIDING_WINDOW_EXP(31)
	TEST_SLIDING_WINDOW_EXP(32)
	TEST_SLIDING_WINDOW_EXP(33)
	TEST_SLIDING_WINDOW_EXP(34)
	TEST_SLIDING_WINDOW_EXP(35)
	TEST_SLIDING_WINDOW_EXP(36)
	TEST_SLIDING_WINDOW_EXP(37)
	TEST_SLIDING_WINDOW_EXP(38)
	TEST_SLIDING_WINDOW_EXP(39)
	TEST_SLIDING_WINDOW_EXP(40)
	TEST_SLIDING_WINDOW_EXP(41)
	TEST_SLIDING_WINDOW_EXP(42)
	TEST_SLIDING_WINDOW_EXP(43)
	TEST_SLIDING_WINDOW_EXP(44)
	TEST_SLIDING_WINDOW_EXP(45)
	TEST_SLIDING_WINDOW_EXP(46)
	TEST_SLIDING_WINDOW_EXP(47)
	TEST_SLIDING_WINDOW_EXP(48)
	TEST_SLIDING_WINDOW_EXP(49)
	TEST_SLIDING_WINDOW_EXP(50)
	TEST_SLIDING_WINDOW_EXP(51)
	TEST_SLIDING_WINDOW_EXP(52)
	TEST_SLIDING_WINDOW_EXP(53)
	TEST_SLIDING_WINDOW_EXP(54)
	TEST_SLIDING_WINDOW_EXP(55)
	TEST_SLIDING_WINDOW_EXP(56)
	TEST_SLIDING_WINDOW_EXP(57)
	TEST_SLIDING_WINDOW_EXP(58)
	TEST_SLIDING_WINDOW_EXP(59)
	TEST_SLIDING_WINDOW_EXP(60)
	TEST_SLIDING_WINDOW_EXP(61)
	TEST_SLIDING_WINDOW_EXP(62)
	TEST_SLIDING_WINDOW_EXP(63)
	TEST_SLIDING_WINDOW_EXP(64)
	TEST_SLIDING_WINDOW_EXP(65)
	TEST_SLIDING_WINDOW_EXP(66)
	TEST_SLIDING_WINDOW_EXP(67)
	TEST_SLIDING_WINDOW_EXP(68)
	TEST_SLIDING_WINDOW_EXP(69)
	TEST_SLIDING_WINDOW_EXP(70)
	TEST_SLIDING_WINDOW_EXP(71)
	TEST_SLIDING_WINDOW_EXP(72)
	TEST_SLIDING_WINDOW_EXP(73)
	TEST_SLIDING_WINDOW_EXP(74)
	TEST_SLIDING_WINDOW_EXP(75)
	TEST_SLIDING_WINDOW_EXP(76)
	TEST_SLIDING_WINDOW_EXP(77)
	TEST_SLIDING_WINDOW_EXP(78)
	TEST_SLIDING_WINDOW_EXP(79)
	TEST_SLIDING_WINDOW_EXP(80)
	TEST_SLIDING_WINDOW_EXP(81)
	TEST_SLIDING_WINDOW_EXP(82)
	TEST_SLIDING_WINDOW_EXP(83)
	TEST_SLIDING_WINDOW_EXP(84)
	TEST_SLIDING_WINDOW_EXP(85)
	TEST_SLIDING_WINDOW_EXP(86)
	TEST_SLIDING_WINDOW_EXP(87)
	TEST_SLIDING_WINDOW_EXP(88)
	TEST_SLIDING_WINDOW_EXP(89)
	TEST_SLIDING_WINDOW_EXP(90)
	TEST_SLIDING_WINDOW_EXP(91)
	TEST_SLIDING_WINDOW_EXP(92)
	TEST_SLIDING_WINDOW_EXP(93)
	TEST_SLIDING_WINDOW_EXP(94)
	TEST_SLIDING_WINDOW_EXP(95)
	TEST_SLIDING_WINDOW_EXP(96)
	TEST_SLIDING_WINDOW_EXP(97)
	TEST_SLIDING_WINDOW_EXP(98)
	TEST_SLIDING_WINDOW_EXP(99)

	return 0;
}

#define TEST_DIV_SUBROUTINE(ID) \
	l_ret = schoolBookDivSubRoutine(l_qBuf, l_rBuf, g_divtest##ID##a, g_divtest##ID##b, sizeof(g_divtest##ID##a) >> 2, sizeof(g_divtest##ID##b) >> 2);\
	if (l_ret != 0 || memcmp(l_rBuf, g_divtest##ID##r, sizeof(g_divtest##ID##r)) || memcmp(l_qBuf, g_divtest##ID##q, sizeof(g_divtest##ID##q)))\
	{\
		printf("Error in test %d div subroutine\n", ID);\
	}\


int schoolbook_div_tests(){

	int l_ret;
	uint32_t *l_qBuf  = g_buffer;
	uint32_t *l_rBuf  = g_buffer + MAX_PRECISION;

	TEST_DIV_SUBROUTINE(0)
	TEST_DIV_SUBROUTINE(1)

	TEST_DIV_SUBROUTINE(2)
	TEST_DIV_SUBROUTINE(3)
	TEST_DIV_SUBROUTINE(4)

	TEST_DIV_SUBROUTINE(6)
	TEST_DIV_SUBROUTINE(7)
	TEST_DIV_SUBROUTINE(8)
	TEST_DIV_SUBROUTINE(9)
	TEST_DIV_SUBROUTINE(10)
	TEST_DIV_SUBROUTINE(11)
	TEST_DIV_SUBROUTINE(12)
	TEST_DIV_SUBROUTINE(13)
	TEST_DIV_SUBROUTINE(14)
	TEST_DIV_SUBROUTINE(15)
	TEST_DIV_SUBROUTINE(16)
	TEST_DIV_SUBROUTINE(17)
	TEST_DIV_SUBROUTINE(18)
	TEST_DIV_SUBROUTINE(19)
	TEST_DIV_SUBROUTINE(20)
	TEST_DIV_SUBROUTINE(21)
	TEST_DIV_SUBROUTINE(22)
	TEST_DIV_SUBROUTINE(23)
	TEST_DIV_SUBROUTINE(24)
	TEST_DIV_SUBROUTINE(25)
	TEST_DIV_SUBROUTINE(26)
	TEST_DIV_SUBROUTINE(27)
	TEST_DIV_SUBROUTINE(28)
	TEST_DIV_SUBROUTINE(29)
}
