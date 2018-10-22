#ifndef OCR_BACKPROP_H
#define OCR_BACKPROP_H

#include "network.h"

double cost(double out,double expected);

double cost_prime_out(double out,double expected);

double out_prime_sum(double out);

/*double sum_prime_weight(int n)*/

//double cost_prime_weight(double out,double expected,double n)
double cost_prime_weight(double out,double n,double a);

double cost_prime_bias(double out,double expected);

double cost_prime_sum(double out,double expected,double weight);

//double total_cost(Matrix out,Matrix in)
double total_cost(double out,double expected);

void backprop(Network N,double out,double expected);

#endif //OCR_BACKPROP_H
