#include"network.h"
#include"backprop.h"

int main()
{
	
	/*int array[] = {1024,15,10};
	int *sizes = array;
	Network net = initNetwork(sizes);*/
	Network net = loadNetwork("aybe");

	FILE  *file;//declare file

	double cost;

	//srand(time(NULL));
	//saveNetwork(net, "t");
	double lrat;
	int max = 946;
	int dim = 32;
	int b = 20;
	for(int a = 0; a < b; ++a)
	{
	file = fopen("data_set", "r");//open file to read
	for(int i = 0; i < max; ++i)
	{
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

		printf("%i/%i\n", max*a + i, max*b);

		do
		{
			feedforward(net);
			//printNet(net);
			//printf("%i/1000000\n", i);
			
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

		}while(cost > 0.001);
		
		net.expected[ex] = 0;
	}
	fclose(file);
	}
	saveNetwork(net, "aybe");/*
	
	printf("---------------------------------------------------------------------------------\n");
	int ex;
		file = fopen("data_set", "r");//open file to read
	for(int i = 0; i < max; ++i)
	{		
		//file = fopen("data_set", "r");//open file to read
		lrat = 0.2;

		for(int h = 0; h < dim; ++h)
		{
			for(int j = 0; j < dim; ++j)
			{
				net.n_outputs[h*dim + j] = (int)fgetc(file) - 48;
				//printf("%d", (int)net.n_outputs[h*dim + j]);
			}
			fgetc(file);
			//printf("\n");
		}
		ex = (int)fgetc(file) - 48;
		//net.expected[ex] = 1; 
		fgetc(file);
		

		feedforward(net);
		//printNet(net);
		if(net.n_outputs[net.num_neurons - net.sizes[2] + ex] < 0.1)
		{
			//for(int h = 0; h < dim; ++h)
			{
				for(int j = 0; j < dim; ++j)
				{
					//net.n_outputs[h*dim + j] = (int)fgetc(file) - 48;
					printf("%d", (int)net.n_outputs[h*dim + j]);
				}
				//fgetc(file);
				printf("\n");
			}//
			printf("%i\n", ex);
			for(int l = 0; l < 10; ++l)
			{
				printf("%i: %lf, ", l, net.n_outputs[1039 + l]);
			}
			printf("\n");
		}
		//printf("Input: %i %i Expected: %i Returned: %lf\n", a, b, a + b == 1, net.n_outputs[net.num_neurons - 1]);
	
	}
	printf("\n");
	fclose(file);*/
	
	freenet(net);
/*

	Network neet = loadNetwork("tato");
	neet.n_outputs[0] = a;
	neet.n_outputs[1] = b;
	feedforward(neet);
	printNet(neet);
	freenet(neet);
	free(neet.sizes);*/
	return 0;
}
