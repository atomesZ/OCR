#include"network.h"

int main()
{
	int array[] = {2,3,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	net.n_outputs[0] = 0;
	net.n_outputs[1] = 1;
	feedforward(net, net.n_outputs);
	printf("\n");
	printNet(net);
	freenet(net);

	return 0;
}
