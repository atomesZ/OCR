#ifndef OCR_STOCHASTIC_H
#define OCR_STOCHASTIC_H

#include "network.h"

double loss(double out,double expected);

double derive_output(Network net, int n);

double derive_weight(double derivesuivante,double outputsuivante,double output);

void SGD(Network N, double s[]/*data_set*/, size_t set_size, double LR, size_t epochs);

#endif //OCR_STOCHASTIC_H

