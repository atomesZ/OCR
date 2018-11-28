#include "backprop.h"
#include "network.h"
#include <math.h>

double loss(double out, double expected)
{
    return (expected - out) * (expected - out) ;
}

double derive_output(Network net, int n)
{
    return sigmoid_prime(net.n_outputs[n]);
}

double derive_weight(double derivesuivante, double outputsuivante, double output)
{
    return derivesuivante * sigmoid_prime(outputsuivante) * output;
}

void backprop(Network net, double out[], double expected[], double LR)                
{
	double delta2[net.sizes[2]];
	for(int i = 0; i < net.sizes[2]; ++i)
	{
		delta2[i] = (out[i] - expected[i]) *  derive_output(net, net.num_neurons - net.sizes[2] + i);
	}
	double delta1[net.sizes[1]];

	for(int i = 0; i < net.sizes[1]; ++i)
	{
		double r = 0;
		for(int j = 0; j < net.sizes[2] ; ++j)
		{
			r += delta2[j] * net.weights[net.sizes[0]*net.sizes[1] + j + i*net.sizes[2]];
		}
		delta1[i] = r * derive_output(net, net.sizes[0] + i);
	}

	//maj B2
	for(int i = 0; i < net.sizes[2]; ++i)
		net.biases[net.sizes[1] + i] -= LR * delta2[i];

	//maj B1
	for(int i = 0; i < net.sizes[1] ; ++i)
		net.biases[i] -= LR * delta1[i];

	//maj W1
	for(int i = 0; i < net.sizes[0]; ++i)
	{
		for(int j = 0; j < net.sizes[1]; ++j)
		{
			net.weights[j*net.sizes[0] + i] -= LR * delta1[j] * net.n_outputs[i];
		}
	}	

	//maj W2
	for(int i = 0; i < net.sizes[1]; ++i)
	{
		for(int j = 0; j < net.sizes[2]; ++j)
		{
			net.weights[j*net.sizes[1] + i + net.sizes[0] * net.sizes[1]]
				 -= LR * delta2[j] * net.n_outputs[i + net.sizes[0]];
		}
	}	
}


