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

	net.expected = malloc(sizeof(double) * sizes[net.num_layers - 1]);
	for(int i = 0; i < sizes[net.num_layers-1]; ++i)
	{
		net.expected[i] = 0;
	}

	return net;
}

void saveNetwork(Network net, char* filename)
{
    FILE *file;//declare file
    file = fopen(filename, "w+");//create or rewrite file

	if(file == NULL)
		errx(1, "Could not open file");

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
	if(file == NULL)
		errx(1, "Could not open file");

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

	net.expected = malloc(sizeof(double) * net.sizes[net.num_layers - 1]);
	for(int i = 0; i < net.sizes[net.num_layers-1]; ++i)
	{
		net.expected[i] = 0;
	}

	fclose(file);//close file
	return net;//return the network
}

void freenet(Network net)
{
	//free(net.sizes);
	free(net.biases);
	free(net.weights);
	free(net.n_outputs);
	free(net.expected);
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

int num_errors(Network net, char* filename)
{
	int max = 946;
	int dim = 32;
	int nbfails = 0;//Init the number of errors
	FILE *file;//declare file
	file = fopen(filename, "r");//open file to read
	if(file == NULL)
		errx(1, "Could not open file");
	int ex;

	for(int i = 0; i < max; ++i)
	{
		for(int h = 0; h < dim; ++h)
		{
			for(int j = 0; j < dim; ++j)
			{
				net.n_outputs[h*dim + j] = (int)fgetc(file) - 48;
			}

			fgetc(file);
		}
		ex = (int)fgetc(file) - 48;
		fgetc(file);

		feedforward(net);
		
		int imax = 0;
		double max = net.n_outputs[net.num_neurons - net.sizes[2]];
		for(int e = 1; e < net.sizes[2]; ++e)
		{
			if(net.n_outputs[net.num_neurons - net.sizes[2] + e] > max)
			{
				max = net.n_outputs[net.num_neurons - net.sizes[2] + e];
				imax = e;
			}
		}
		

		if(imax != ex)
		{
			++nbfails;
			
			for(int h = 0; h < dim; ++h)
			{
				for(int j = 0; j < dim; ++j)
				{
					printf("%i", (int)net.n_outputs[h*dim + j]);
				}
				printf("\n");
			}

			
			for(int e = 0; e < net.sizes[2]; ++e)
			{
				printf("%lf ",net.n_outputs[net.num_neurons - net.sizes[2] + e]);
			}
			printf("%i", ex);	
			printf("\n");
		}
	}

	fclose(file);

	return nbfails;
}

void readtxt(Network net, char* filename, char* s, int num_char)
{
	FILE  *file;//declare file
	file = fopen(filename, "r");//open file to read
	
	int dim = 32;
	for(int i = 0; i < num_char; ++i)
	{
		for(int h = 0; h < dim; ++h)
		{
			for(int j = 0; j < dim; ++j)
			{
				net.n_outputs[h*dim + j] = (int)fgetc(file) - 48;
			}

			fgetc(file);
		}
		fgetc(file);//out the value
		fgetc(file);//out "\n"

		feedforward(net);
		
		int imax = 0;
		double max = net.n_outputs[net.num_neurons - net.sizes[2]];
		for(int e = 1; e < net.sizes[2]; ++e)
		{
			if(net.n_outputs[net.num_neurons - net.sizes[2] + e] > max)
			{
				max = net.n_outputs[net.num_neurons - net.sizes[2] + e];
				imax = e;
			}
		}
		
		s[i] = imax + 48;//33 pour OCR
	}
	s[num_char] = 0;
}

void train(Network net, char* filename)
{
	FILE  *file;//declare file

	double cost;
	double lrat;
	long max = 946;
	int dim = 32;
	long b = 30000;
	srand(time(NULL));
	file = fopen(filename, "r");//open file to read
	for(long a = 1; a <= b; ++a)
	{
		//file = fopen("data_set", "r");//open file to read
		if(file == NULL)
			errx(1, "Could not open file");
		//for(int i = 1; i <= max; ++i)
		{
			long d = rand() % max * (dim*dim + dim + 2);
			fseek(file, d,SEEK_SET);
			lrat = 0.2;
			for(int h = 0; h < dim; ++h)
			{
				for(int j = 0; j < dim; ++j)
				{
					net.n_outputs[h*dim + j] = (int)fgetc(file) - 48;
				}
				fgetc(file);
			}
			int ex = (int)fgetc(file) - 48;
			net.expected[ex] = 1; 
			fgetc(file);
		
			for(int c = 0; c < 15; ++c)
				printf("\b");
			printf("%ld/%ld", a, b);

			do
			{
				feedforward(net);
			
				cost = 0;
				for(int k = 0; k < 10; ++k)
				{
					cost += loss(net.n_outputs[net.num_neurons - net.sizes[net.num_layers - 1] + k], net.expected[k]);
				}
				cost /= 10;

				if(cost < 0.1)
					lrat = 0.02;
				if(cost < 0.01)
					lrat = 0.002;

				backprop(net, &net.n_outputs[net.num_neurons - net.sizes[net.num_layers - 1]], net.expected, lrat);

			}while(cost > 0.01 ||(cost > 0.001 && a < 2 * b / 3));
		
			net.expected[ex] = 0;
		}
	//rewind(file);
	}
	fclose(file);
	saveNetwork(net, "aybe");
	printf("\n");
}
