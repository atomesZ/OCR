#include "backprop.h"
#include "network.h"
#include <math.h>

double cost(double out,double expected)
{
    if((out - expected) < 0)
        return (out - expected) * (out - expected);
    else
        return (out - expected);
    /*return (out - expected) * (out - expected);*/
}

double cost_prime_out(double out,double expected)
{
    return 2*(out - expected);
}

double out_prime_sum(double out)
{
    return sigmoid_prime(out);
}

/*double sum_prime_weight(int n)
{
    return 1;
}*/

//double cost_prime_weight(double out,double expected,double n)
double cost_prime_weight(double out,double n,double a)           //XOR
{
    return a*out_prime_sum(out)*n;
}

double cost_prime_bias(double out,double expected)
{
    return cost_prime_out(out,expected)*out_prime_sum(out);
}

double cost_prime_sum(double out,double expected,double weight)
{
    return cost_prime_out(out,expected)*out_prime_sum(out)*weight;
}

//double total_cost(Matrix out,Matrix in)
double total_cost(double out,double expected)         //XOR
{
    double total_cost = 0;
    //int n = out.columns*out.rows;
    int n = 1;                                          //XOR
    for (int i = 0; i < n; ++i)
    {
        //total_cost += cost(out.m[i],in.m[1]);
        total_cost += cost(out,expected);                         //XOR
    }

    printf("%lf",total_cost);

    return total_cost;
}

/*double new_weight(double weight,double LR)
{
    return weight -LR*cost_prime_weight();
}

double new_bias(double bias,double LR)
{
    return bias -LR*2;
}*/



//int backprop(Network N,double *outpt,Matrix in)
void backprop(Network N,double out,double expected)                    // XOR
{
    //double cost = total_cost(out,expected);

    /*for (int L = N.num_layers; L < 0; ++L)
    {



    }*/
                                                                                                //XOR :

    double a = cost_prime_sum(out,expected,N.weights[5]) + cost_prime_sum(out,expected,N.weights[4]) + cost_prime_sum(out,expected,N.weights[3]);

    N.weights[8] = N.weights[8] - 0.1*cost_prime_weight(out,N.n_outputs[2],cost_prime_out(out,expected));
    N.weights[7] = N.weights[7] - 0.1*cost_prime_weight(out,N.n_outputs[2],cost_prime_out(out,expected));
    N.weights[6] = N.weights[6] - 0.1*cost_prime_weight(out,N.n_outputs[2],cost_prime_out(out,expected));

    N.biases[4] = N.biases[4] - 0.1*cost_prime_bias(out,expected);
    N.biases[3] = N.biases[3] - 0.1*cost_prime_bias(out,expected);

    N.weights[5] = N.weights[5] - 0.1*cost_prime_weight(out,N.n_outputs[1],a);
    N.weights[4] = N.weights[4] - 0.1*cost_prime_weight(out,N.n_outputs[1],a);
    N.weights[3] = N.weights[3] - 0.1*cost_prime_weight(out,N.n_outputs[1],a);
    N.weights[2] = N.weights[2] - 0.1*cost_prime_weight(out,N.n_outputs[0],a);
    N.weights[1] = N.weights[1] - 0.1*cost_prime_weight(out,N.n_outputs[0],a);
    N.weights[0] = N.weights[0] - 0.1*cost_prime_weight(out,N.n_outputs[0],a);
}


