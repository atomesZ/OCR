#include"network.h"
#include"backprop.h"

int main()
{
	int array[] = {2,3,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	net.n_outputs[0] = 0;
	net.n_outputs[1] = 0;
	printNet(net);
	feedforward(net, net.n_outputs);
	printf("\n");
	printNet(net);
	printf("\n");
	printf("final output: %lf ", net.n_outputs[net.num_neurons - 1]);

	for(int i = 0; i < 300; ++i)
	{
		backprop(net, net.n_outputs[net.num_neurons - 1], 0);
		feedforward(net, net.n_outputs);
		printNet(net);
	}
	freenet(net);

	return 0;
}
