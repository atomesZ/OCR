#include"network.h"

int main()
{
	int array[] = {2,3,1};
	int *sizes = array;
	Network net = initNetwork(sizes);
	printNet(net);
	freenet(net);

	return 0;
}
