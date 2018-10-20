#include"network.h"

Network initNetwork(int *sizes)
{
	//initialize network
	Network net;

	net.num_layers = 3;

	net.sizes = sizes;

	net.num_neurons = sizes[0] + sizes[1] + sizes[2];

	net.num_links = sizes[1] * (sizes[0] + sizes[2]); 

	srand(time(NULL));
	net.biases = malloc(sizeof(double) * net.num_neurons);
	for(int i = 0; i < net.num_neurons; ++i)
	{
		net.biases[i] = ((double)rand()/(double)RAND_MAX);
	}

	net.weights = malloc(sizeof(double) * net.num_links);
	for(int i = 0; i < net.num_links; ++i)
	{
		net.weights[i] = ((double)rand()/(double)RAND_MAX);
	}

	return net;
}

void freenet(Network net)
{
	free(net.biases);
	free(net.weights);
}

double sigmoid(double z)
{
	return 1.0 / (1.0 + exp(-z));
}

double sigmoid_prime(double z)
{
	return sigmoid(z) * (1 - sigmoid(z));
}

double dot(double *w, double *x_in, int len, int i)
{
	double r = 0;

	for(int j = 0; j < len; ++j)
		r += w[j + i] * x_in[j];

	return r;
}

void feedforward(Network net, double *inpt, double *outpt, int layer)
{
	int s = net.sizes[layer];

	for(int i = 0; i < s; ++i)
	{
		double d = dot(net.weights, inpt, net.sizes[layer], i * s);
		outpt[i] = sigmoid(d + net.biases[i]);	
	}
}
