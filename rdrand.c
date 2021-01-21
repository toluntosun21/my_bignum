#define __STDC_FORMAT_MACROS
#define _GNU_SOURCE
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpuid.h"


#ifndef bit_RDRAND
#define bit_RDRAND 0x40000000
#endif

int cpu_supports_rdrand() {
  uint32_t eax=0, ebx=0, ecx=0, edx=0;
  __get_cpuid(1, &eax, &ebx, &ecx, &edx);
  return ecx & bit_RDRAND;
}

int rdrand_fill_array2(uint32_t* array, uint32_t size) {
  // We want to do this as quickly as possible. Assembly hoy!
  uint32_t successes = 0;
  uint32_t temp = 0;

  __asm volatile(
      "top_of_rdrand_loop%=:"
      "jcxz end_of_rdrand_loop%= ;\n"   // jump if ecx (size) == 0

      "rdrand %1 ;\n"                    // Generate random value
      "adc $0, %0 ;\n"                   // Check if successul

      "mov %1, (%2) ;\n "                // Store value in array
      "add $4, %2 ;\n "                  // Move array to next spot

      "dec %4 ;\n"                       // Decrement size
      "jmp top_of_rdrand_loop%= ;\n"

      "end_of_rdrand_loop%=:\n"
    : "=r" (successes), "=r" (temp), "=r"(array)
    : "0" (successes), "c" (size), "2"(array)
    );

  return (int)successes;
}

int rdrand_fill_array(uint32_t *p_array, int p_size) {
  // We want to do this as quickly as possible. Assembly hoy!
  uint32_t l_successes = 0;
  uint32_t l_ret = 0;
  uint32_t l_temp = 0;
  int i;

  for(i = 0; i < p_size; i++)
  {
	  __asm volatile(
		  "rdrand %0 ;\n"                    // Generate random value
	      "adc $0, %1 ;\n"                   // Check if successul
		: "=r" (l_temp), "=r" (l_successes)
		);
	  p_array[i] = l_temp;
  }


  return (int)l_successes;
}
