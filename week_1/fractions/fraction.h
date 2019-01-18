#ifndef FRACTION_H
#define FRACTION_H

/*! @file */

/*! \breif Fraction object and method definitions
 *
 *  A fraction object is a struct with a numerator, denoted num, and
 *  a denominator, denoted den. Varions Methods that take fractions and return 
 *  fractions do arithmetical operations on them.
 */
typedef struct {
    int num;
    int den;
} Fraction;

/*! Add two fractions together
 *  \param a The first summand
 *  \param b The second summand
 */
Fraction add ( Fraction a, Fraction b );

/*! Multiply two fractions together (does not reduce)
 *  \param a The first term
 *  \param b The second term
 */
Fraction multiply ( Fraction a, Fraction b );

#endif