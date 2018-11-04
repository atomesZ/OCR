# ifndef NETWORK_H_
# define NETWORK_H_

#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

typedef struct Network Network;

struct Network
{
	int num_layers; //number of layers
	int *sizes; //number of neurons by layers
	double *biases; //list of biases for neurons
	double *weights; //list of weights for neurons
	int num_neurons; //number of neurons
	int num_links; //number of links
	double *n_outputs; //list of outputs of each neurons
};

Network initNetwork(int *sizes);
void freenet(Network net);
double sigmoid(double z);
double sigmoid_prime(double z);
double dot(double *w, double *n_outputs, int len, int ifo, int ifw);
void feedforward(Network net, double *n_outputs);
void printNet(Network net);

#endif
