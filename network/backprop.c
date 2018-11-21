#include "backprop.h"
#include "network.h"
#include <math.h>

double loss(double out,double expected)
{
    return (expected - out) * (expected - out) / 2;
}

double derive_output(Network net, int n)
{
    return sigmoid_prime(net.n_outputs[n]);
}

double derive_weight(double derivesuivante,double outputsuivante,double output)
{
    return derivesuivante * sigmoid_prime(outputsuivante) * output;
}

void backprop(Network net,double out,double expected,double LR)                
{

	double delta2 = (out - expected) *  derive_output(net, net.num_neurons - 1);

	double delta1 = 0;

	for(int i = 0; i < net.sizes[1] * net.sizes[2]; ++i)
	{
		delta1 += delta2 * net.weights[i + net.sizes[0] * net.sizes[1]];
	}

	double delta11 = delta1 * derive_output(net, net.sizes[0]);
	double delta12 = delta1 * derive_output(net, net.sizes[0] + 1);

	//maj
	net.biases[net.num_biases - 1] -= LR * delta2;

	net.biases[0] -= LR * delta11;
	net.biases[1] -= LR * delta12;
	int ifw = 0;
	int n = 0;/*
	while(ifw < net.sizes[0] * net.sizes[1])
	{
		net.weights[ifw] -= delta11 * LR * net.n_outputs[n];
		++ifw;
		net.weights[ifw] -= delta12 * LR * net.n_outputs[n];
		++ifw;
		++n;
	}*/
	
	net.weights[0] -= delta11 * LR * net.n_outputs[0];
	net.weights[2] -= delta12 * LR * net.n_outputs[0];
	net.weights[1] -= delta11 * LR * net.n_outputs[1];
	net.weights[3] -= delta12 * LR * net.n_outputs[1];
	ifw = 4;
	n = 2;


	while(ifw < net.num_links)
	{
		net.weights[ifw] -= delta2 * LR * net.n_outputs[n];
		++ifw;
		++n;
	}
}

void XOR(Network N,double out,double expected)
{
    //double cost = 0.5*((expected - out) * (expected - out));

    /*double a = -(expected - out);
    double b = out*(1 - out);
    double c = N.n_outputs[];*/

    N.weights[5] = N.weights[5] - 0.5 * ( (-1*(expected - out)) * (out * ( 1 - out )) * N.n_outputs[2] );
    N.weights[4] = N.weights[4] - 0.5 * ( (-1*(expected - out)) * (out * ( 1 - out )) * N.n_outputs[3] );

    /*double d1 = a * b;
    double d2 = N.weights[]; // couche suivante
    double d = d1 * d2;
    double e = N.n_outputs[]*(1 - N.n_outputs[]); // neurone apres
    double f = N.n_outputs[] ; // neurone avant*/

    /*N.weights[5] = N.weights[5] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[8] ) * ( N.n_outputs[4] * (1 - N.n_outputs[4] ) ) * N.n_outputs[1] );
    N.weights[4] = N.weights[4] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[8] ) * ( N.n_outputs[4] * (1 - N.n_outputs[4] ) ) * N.n_outputs[0] );*/

    N.weights[3] = N.weights[3] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[5] ) * ( N.n_outputs[3] * (1 - N.n_outputs[3] ) ) * N.n_outputs[1] );
    N.weights[2] = N.weights[2] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[5] ) * ( N.n_outputs[3] * (1 - N.n_outputs[3] ) ) * N.n_outputs[0] );

    N.weights[1] = N.weights[1] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[4] ) * ( N.n_outputs[2] * (1 - N.n_outputs[2] ) ) * N.n_outputs[1] );
    N.weights[0] = N.weights[0] - 0.5 * ( ( ( (-1*(expected - out)) * (out * ( 1 - out )) )* N.weights[4] ) * ( N.n_outputs[2] * (1 - N.n_outputs[2] ) ) * N.n_outputs[0] );

}
