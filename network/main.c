#include"network.h"
#include"backprop.h"

int main()
{
	
	int array[] = {2,2,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	//Network net = loadNetwork("l");

	srand(time(NULL));
	int a = 0;
	int b = 0;
	saveNetwork(net, "t");
	double lrat;
	for(int i = 0; i < 1000000; ++i)
	{
		lrat = 0.01;
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
		/*
		if(i==100)
			lrat = 5;

		if(i == 1000)
			lrat = 1;

		if (i== 1000)
			lrat = 0.1;
	*/	
		

		net.n_outputs[0] = a;
		net.n_outputs[1] = b;
		do
		{
		feedforward(net);
		backprop(net, net.n_outputs[net.num_neurons - 1], a + b == 1, lrat);
		/*
		if (ez == 15)
			lrat = 0.1;
		if (ez == 50)
			lrat = 0.01;*/
		}while(loss(net.n_outputs[net.num_neurons - 1], a + b == 1) > 0.001);
	//	feedforward(net);
	//	printNet(net);
	//	printf("Input: %i %i Expected: %i Returned: %lf\n", a, b, a + b == 1, net.n_outputs[net.num_neurons - 1]);
	}
	
	printf("---------------------------------------------------------------------------------\n");
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
