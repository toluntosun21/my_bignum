/*
 * multiply.c
 *
 *  Created on: 7 Kas 2020
 *      Author: toluntosun
 */
#include <stdio.h>
#include "tests.h"
#include <stdint.h>
#include <gmp.h>
int cpu_supports_rdrand();
int rdrand_fill_array(uint64_t* array, int size);

int main()
{

	//hw1_tests();

	//montgomery_reduction_tests();

	//montgomery_wl_reduction_tests();

	//montgomery_modmult_tests();

	//sliding_window_exponentiation_tests();

	/*const int size = 16;
	uint32_t test_arr[size];
	printf("stating\n");
	printf("0x%X\n", cpu_supports_rdrand());
	int res = rdrand_fill_array(test_arr, size);
	printf("res %d\n", res);

	for(int i=0; i<size; i++)printf("0x%X\n", test_arr[i]);
*/

	schoolbook_div_tests();
	printf("Tests end. If no error prompts are given, all tests were successful\n");

	return 0;

}
