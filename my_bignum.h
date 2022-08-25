#ifndef MY_BIGNUM
#define MY_BIGNUM

#define MAX_PRECISION 32

#define COUNT_CORE_MULT


int schoolBookMult(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);

int add_unequal(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_pre_a, unsigned int p_pre_b);

int add(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);


int add_old(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);


int add_inplace(uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);

/*
 * assume p_a is greater than p_b
 */
int sub(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);

int sub_inplace(uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);

int shiftDigitsLeft(uint32_t *p_num, unsigned int p_amount, unsigned int p_precision);

int compare(uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);



int oneLevelKaratsubaMultOld(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);

/*
 * works for odd p_precision
 */
int oneLevelKaratsubaMult(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);


/*
 * works only if p_precision is divisible by 2
 */
int oneLevelKaratsubaMultwithSub(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);


int twoLevelKaratsubaMultOld(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);

/*
 * works for odd p_precision
 */
int twoLevelKaratsubaMult(uint32_t *p_res, uint32_t *p_a, uint32_t *p_b, unsigned int p_precision);


int modmult_karatsubasub(uint32_t *p_res, uint32_t *p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision);

int modmult_schoolbook(uint32_t *p_res, uint32_t *p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision);


/*
 * The naive method
 */
int montgomery_reduction(uint32_t *p_res, uint32_t *p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision);


/*
 * Montgomery reduction (but not the multiplication inside) is implemented in word-level
 */
int montgomery_reduction_wordlevel(uint32_t *p_res, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision);

/*
 * Montgomery reduction (including the multiplication inside) is implemented in word-level
 */
int montgomery_reduction_mult_wordlevel(uint32_t *p_res, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision);


/*
 * Utilizes montgomery_reduction_wordlevel internally
 */
int montgomery_modmult_wordlevel_reduce(uint32_t *p_res, uint32_t *p_rsq, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision);

/*
 * Utilizes montgomery_reduction_mult_wordlevel internally
 */
int montgomery_modmult_wordlevel_multreduce(uint32_t *p_res, uint32_t *p_rsq, uint32_t p_mu, uint32_t *p_a, uint32_t *p_b, uint32_t *p_p, unsigned int p_precision);

/*
 * Utilizes montgomery_modmult_wordlevel_reduce internally
 * http://cacr.uwaterloo.ca/hac/about/chap14.pdf 14.85
 */
int sliding_window_exponentiation(uint32_t *p_res, uint32_t *p_rsq, unsigned int p_k, uint32_t p_mu, uint32_t *p_g, uint32_t *p_e, uint32_t *p_p, unsigned int p_precision);



int schoolBookDivSubRoutine(uint32_t *p_quotient, uint32_t *p_remainder, uint32_t *p_a, uint32_t *p_b, unsigned int p_pre_a, unsigned int p_pre_b);



#endif
