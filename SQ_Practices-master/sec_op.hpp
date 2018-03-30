/**
* @file sec_sum.hpp
* @brief Secure operations functions
*
*/
#ifndef SEC_OP_HPP
#define SEC_OP_HPP

#include <climits>
#include <stdexcept>

/**
 * Add two signed integers preventing overflow
 * @param a		operand a
 * @param b		operand b
 * @return 		sum
 */
int sec_sum(int a, int b)
{
	int sum;
	if (((b > 0) && (a > (INT_MAX - b))) ||
	  ((b < 0) && (a < (INT_MIN - b)))) {
	/* Handle error */
		throw std::overflow_error("Integer Overflow");
	} else {
	sum = a + b;
	}
	return sum;
}

/**
 * Subtract two signed integers preventing overflow
 * @param a		operand a
 * @param b		operand b
 * @return 		diff
 */
int sec_diff(int a, int b)
{
	signed diff;
	if ((b > 0 && a < INT_MIN + b) ||
	  (b < 0 && a > INT_MAX + b)) {
	/* Handle error */
		throw std::overflow_error("Integer Overflow");
	} else {
	diff = a - b;
	}
	return diff;
}

#endif /* SEC_OP_HPP */
