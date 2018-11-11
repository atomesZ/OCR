#include"network.h"
#include"backprop.h"

int main()
{
	int array[] = {2,3,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	net.n_outputs[0] = 1;
	net.n_outputs[1] = 1;
	printNet(net);
	feedforward(net, net.n_outputs);
	printf("\n");
	printNet(net);
	printf("\n");
	printf("final output: %lf ", net.n_outputs[net.num_neurons - 1]);

	srand(time(NULL));
	int a;
	int b;
	for(int i = 0; i < 100; ++i)
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
		
		net.n_outputs[0] = a;
		net.n_outputs[1] = b;
		feedforward(net, net.n_outputs);
		backprop(net, net.n_outputs[net.num_neurons - 1], a + b == 1);
		printNet(net);
	}
	freenet(net);

	return 0;
}
