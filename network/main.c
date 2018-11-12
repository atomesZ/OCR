#include"network.h"
#include"backprop.h"

int main()
{
	int array[] = {2,3,1};
	int *sizes = array;
	Network net = initNetwork(sizes);

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
		feedforward(net);
		backprop(net, net.n_outputs[net.num_neurons - 1], a + b == 1);
		printNet(net);
		printf("Input: %i %i Expected: %i Returned: %lf\n", a, b, a + b == 1, net.n_outputs[net.num_neurons - 1]);
	}
	freenet(net);

	return 0;
}
