#include"network.h"
#include"backprop.h"

int main()
{
	int array[] = {2,2,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	//Network net = loadNetwork("ttt");

	srand(time(NULL));
	int a = 0;
	int b = 0;
	saveNetwork(net, "t");
	double lrat = 25;
	for(int i = 0; i < 100000; ++i)
	{
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
		/*
		
		
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

		

		net.n_outputs[0] = a;
		net.n_outputs[1] = b;
		feedforward(net);
		backprop(net, net.n_outputs[net.num_neurons - 1], a + b == 1, lrat);
		feedforward(net);
		printNet(net);
		printf("Input: %i %i Expected: %i Returned: %lf\n", a, b, a + b == 1, net.n_outputs[net.num_neurons - 1]);
	}
	
	saveNetwork(net, "tfin");
	freenet(net);/*

	Network neet = loadNetwork("tato");
	neet.n_outputs[0] = a;
	neet.n_outputs[1] = b;
	feedforward(neet);
	printNet(neet);
	freenet(neet);
	free(neet.sizes);*/
	return 0;
}
