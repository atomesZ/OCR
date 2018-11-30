#include "backprop.h"
#include "network.h"
#include <math.h>

double loss(double out,double expected)
{
    return (expected - out) * (expected - out) / 2;
}

double derive_output(Network net, int n)
{
    return sigmoid_prime(net.n_outputs[n]);
}

double derive_weight(double derivesuivante,double outputsuivante,double output)
{
    return derivesuivante * sigmoid_prime(outputsuivante) * output;
}

void SGD(Network N, double s[]/*data_set*/, int set_size, double LR, int epochs)
{
    double delta_w[N.num_links ];
		
    double delta_b[N.num_biases];

    ////double LR = 0.1;


    int epoch_n = 0;

    int a = 0;
    int b = 0;   

    for (;epoch_n < epochs ; ++epoch_n)
    {
	//bash
	
	int data_i = rand(); // < TAILLE DATA_SET 

	// parcourir element i
	
            //XOR

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

	//feedforward

        N.n_outputs[0] = a;
	N.n_outputs[1] = b;

        feedforward(N);

	//backprop
	
	    //XOR

        int out = N.n_outputs[N.num_neurons - 1];
     
        int expected = a + b == 1;
        
        double delta2 = (out - expected) *  derive_output(N, N.num_neurons - 1);

	double delta1 = 0;
    
        for(int i = 0; i < N.sizes[1] * N.sizes[2]; ++i)
        {
                delta1 += delta2 * N.weights[i + N.sizes[0] * N.sizes[1]];
        }

        double delta11 = delta1 * derive_output(N, N.sizes[0]);
        double delta12 = delta1 * derive_output(N, N.sizes[0] + 1);

                //M.A.J

        delta_b[N.num_biases - 1] -= LR * delta2;

	delta_b[0] -= LR * delta11;
	delta_b[1] -= LR * delta12;

        int ifw = 0;
        int n = 0;

        delta_w[0] -= delta11 * LR * N.n_outputs[0];
        delta_w[2] -= delta12 * LR * N.n_outputs[0];
        delta_w[1] -= delta11 * LR * N.n_outputs[1];
        delta_w[3] -= delta12 * LR * N.n_outputs[1];

        ifw = 4;
        n = 2;

        while(ifw < N.num_links)
        {
                delta_w[ifw] -= delta2 * LR * N.n_outputs[n];
                ++ifw;
                ++n;
        }	
    
    
    
    
    }

    //UPDATE

        //weights

    for (int w_n; w_n < N.num_links; ++w_n)
    {
        N.weights[w_n] = delta_w[w_n]/epochs;    
    }

        //bias
    
    for (int b_n; b_n < N.num_biases; ++b_n)
    {
        N.biases[b_n] = delta_b[b_n]/epochs;
    }
 



}
