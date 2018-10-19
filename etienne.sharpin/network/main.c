#include"network.h"

int main()
{
	int array[] = {2,2,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	freenet(net);
	printf("%f", net.biases[1]);
	double x = 0;
	printf("The exponential value of %lf is %lf\n", x, exp(x));

	return 0;
}
