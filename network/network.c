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

	net.num_biases = net.num_neurons - sizes[0];

	srand(time(NULL));
	net.biases = malloc(sizeof(double) * net.num_biases);
	for(int i = 0; i < net.num_neurons- sizes[0]; ++i)
	{
		net.biases[i] = 2 * ((double)rand()/(double)RAND_MAX) - 1;
	}

	net.weights = malloc(sizeof(double) * net.num_links);
	for(int i = 0; i < net.num_links; ++i)
	{
		net.weights[i] = 2 * ((double)rand()/(double)RAND_MAX) - 1;
	}

	net.n_outputs = malloc(sizeof(double) * net.num_neurons);

	return net;
}

void saveNetwork(Network net, char* filename)
{
    FILE *file;//declare file
    file = fopen(filename, "w+");//create or rewrite file

    fprintf(file, "%d\n", net.num_layers);//num_layers

    for (int i = 0; i < net.num_layers; ++i)//*sizes
    {
        fprintf(file, "%d\n", net.sizes[i]);
    }

    for (int i = 0; i < net.num_biases; ++i)//*biases
    {
        fprintf(file, "%lf\n", net.biases[i]);
    }

	for (int i = 0; i < net.num_links; ++i)//*weights
    {
        fprintf(file, "%lf\n", net.weights[i]);
	}

	fclose(file);//close file
}

Network loadNetwork(char* filename)
{
	FILE  *file;//declare file

	Network net;//declare network

	file = fopen(filename, "r");//open file to read

	fscanf(file, "%d\n", &net.num_layers);//get num_layers
    net.sizes = malloc(sizeof(int) * net.num_layers);//allocate memory for *sizes

    for(int i = 0; i < net.num_layers; ++i)//get *sizes
    {
        fscanf(file, "%d\n", &net.sizes[i]);
    }

	net.num_biases = net.sizes[1] + net.sizes[2];//get num_biases
	net.num_neurons = net.sizes[0] + net.num_biases;//get num_neurons
	net.num_links = net.sizes[1] * (net.sizes[0] + net.sizes[2]);//get num_links

    net.biases = malloc(sizeof(double) * net.num_biases);//allocate memory for *biases
    for (int i = 0; i < net.num_biases; ++i)//get *biases
    {
		fscanf(file, "%lf\n", &net.biases[i]);
    }
	
	net.weights = malloc(sizeof(double) * net.num_links);//allocate memory for *weights
	for (int i = 0; i < net.num_links; ++i)//get *weights
	{
		fscanf(file, "%lf\n", &net.weights[i]);
	}


	net.n_outputs = malloc(sizeof(double) * net.num_neurons);
		
	fclose(file);//close file
	return net;//return the network
}

void freenet(Network net)
{
	//free(net.sizes);
	free(net.biases);
	free(net.weights);
	free(net.n_outputs);
}

double sigmoid(double z)
{
	return 1 / (1 + exp(-z));
}

double sigmoid_prime(double z)
{
	return sigmoid(z) * exp(-z) / (1 + exp(-z));
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

void feedforward(Network net)
{
	int in_w = net.sizes[0];
	int in_r = 0;
	int ifw = 0;
	int layer = 0;
    while(layer < net.num_layers - 1)
	{
		for(int i = 0; i < net.sizes[layer + 1]; ++i)
		{
			double d = dot(net.weights, net.n_outputs, net.sizes[layer], in_r, ifw);
			net.n_outputs[in_w + i] = sigmoid(d + net.biases[in_w + i - net.sizes[0]]);
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
			printf("Neuron %i, Output: %lf, Bias: %lf ", i, net.n_outputs[i], (i-net.sizes[0]<0)? 0 : net.biases[i - net.sizes[0]]);
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
		printf("Neuron %i, Output: %lf, Bias: %lf\n", i, net.n_outputs[i], net.biases[i - net.sizes[0]]);
		++i;
	}
	printf("#########################################################");
	printf("#######################################################\n");

}
