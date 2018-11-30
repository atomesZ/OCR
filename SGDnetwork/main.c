#include"network.h"
#include"backprop.h"
#include "stochastic.h"

int main()
{
	
	int array[] = {2,2,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	//Network net = loadNetwork("l");

        char s[1];

        saveNetwork(net, "t");

        SGD(net,s,4,1,100000);

	/*srand(time(NULL));
	int a = 0;
	int b = 0;
	saveNetwork(net, "t");
	double lrat = 0.1;
	int max = 100000;
	for(int i = 0; i < max; ++i)
	{
		lrat = 0.2;
		a = rand();
		b = rand();
		if(a < RAND_MAX / 2)
			a = 0;
		else
			a = 1;

		if(b < RAND_MAX / 2)
			b = 0;
		else
			b = 1;
		
		
		
		if(!a && !b)
		{
			b = 1;
			
		}
		else if (!a && b)
			a = 1;
		else if (a && b)
		{
			
			b = 0;
		}
		else if(a && !b)
			a = 0;*/
		/*
		if(i==100)
			lrat = 5;

		if(i == 1000)
			lrat = 1;

		if (i== 1000)
			lrat = 0.1;
		
		

		printf("%i/%i\n", i, max);
		net.n_outputs[0] = a;
		net.n_outputs[1] = b;
		do
		{
		feedforward(net);
		//printNet(net);
		//printf("%i/1000000\n", i);
	
		//if(loss(net.n_outputs[net.num_neurons - 1], a + b == 1) < 1)
		//	lrat = 0.1;
		if(loss(net.n_outputs[net.num_neurons - 1], a + b == 1) < 0.1)
			lrat = 0.02;
		if(loss(net.n_outputs[net.num_neurons - 1], a + b == 1) < 0.01)
			lrat = 0.002;

		double ou[] = { net.n_outputs[net.num_neurons - 1]};
		double ex[] = { a + b == 1};
		backprop(net, ou, ex, lrat);

		}while(loss(net.n_outputs[net.num_neurons - 1], a + b == 1) > 0.001);
	//	feedforward(net);
	//	printNet(net);
	//	printf("Input: %i %i Expected: %i Returned: %lf\n", a, b, a + b == 1, net.n_outputs[net.num_neurons - 1]);
	}*/
	
	printf("---------------------------------------------------------------------------------\n");
	int a = 0;
	int b = 0;
	for(int i = 0; i < 4; ++i)
	{		
		if(!a && !b)
			b = 1;
		else if (!a && b)
			a = 1;
		else if (a && b)
			b = 0;
		else if(a && !b)
			a = 0;	

		net.n_outputs[0] = a;
		net.n_outputs[1] = b;
		feedforward(net);
		printNet(net);
		printf("Input: %i %i Expected: %i Returned: %lf\n", a, b, a + b == 1, net.n_outputs[net.num_neurons - 1]);
	}
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