#include"network.h"
#include"backprop.h"

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

	net.n_outputs = malloc(sizeof(double) * net.num_neurons);

	return net;
}

void freenet(Network net)
{
	free(net.biases);
	free(net.weights);
	free(net.n_outputs);
}

double sigmoid(double z)
{
	return 1.0 / (1.0 + exp(-z));
}

double sigmoid_prime(double z)
{
	return sigmoid(z) * (1 - sigmoid(z));
}

double dot(double *w, double *n_outputs, int len, int ifo, int ifw)
{
	//this function calculate the input of the neuron
	double r = 0;
	
	for(int j = 0; j < len; ++j)
	{
		r += w[j + ifw] * n_outputs[j + ifo];
	}

	return r;
}
/*
void feedforward(Network net, double *n_outputs)
{
	int ifo = 0; //index of the first output
	int ifw = 0; // index of the first weight

	for(int layer = 1; layer < net.num_layers - 1; ++layer)
	{
		for(int i = ifo + net.sizes[0]; i < ifo + net.sizes[layer + 1] + net.sizes[0]; ++i)
		{
			double d = dot(net.weights, n_outputs, net.sizes[layer], ifo, ifw);
			n_outputs[i] = sigmoid(d + net.biases[i]);
			ifw += net.sizes[layer];
		}

		ifo += net.sizes[layer];
	}

	int layer = net.num_layers - 1;
	for(int i = ifo; i < ifo + net.sizes[layer] + net.sizes[0]; ++i) //net.sizes[0] ????
	{
		double d = dot(net.weights, n_outputs, net.sizes[layer], ifo, ifw);
		n_outputs[i] = sigmoid(d + net.biases[i]);
		ifw += net.sizes[layer - 1];
	}

}*/



void feedforward(Network net, double *n_outputs)
{
	int in_w = net.sizes[0];
	int in_r = 0;
	int ifw = 0;
	int layer = 0;
    while(layer < net.num_layers - 1)
	{
		for(int i = 0; i < net.sizes[layer + 1]; ++i)
		{
			double d = dot(net.weights, n_outputs, net.sizes[layer], in_r, ifw);
			n_outputs[in_w + i] = sigmoid(d + net.biases[in_w + i]);
			ifw += net.sizes[layer];
		}
		in_r += net.sizes[layer];
		in_w += net.sizes[layer + 1];
		++layer;
	}


}

void printNet(Network net)
{
	printf("###################################################Neural");
	printf("network################################################\n");

	int h = 0;
	int i = 0;
	int j = 0;
	
	while(h < net.num_layers - 1)
	{
		int a = i;
		int b = j;
	    while(i - a < net.sizes[h])
		{
			printf("Neuron %i, Output: %lf, Bias: %lf ", i, net.n_outputs[i], net.biases[i]);
			printf("Weights: ");
			while(j - b < net.sizes[h+1] )
			{
				printf(" w%i: %lf", j, net.weights[j]);
				++j;
			}
			b = j;
			printf("\n");
			++i;
		}
		++h;
	}

	for(int k = 0; k < net.sizes[net.num_layers -1]; ++k)
	{
		printf("Neuron %i, Output: %lf, Bias: %lf\n", i, net.n_outputs[i], net.biases[i]);
		++i;
	}
	printf("#########################################################");
	printf("#######################################################\n");

}
