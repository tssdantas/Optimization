#include <iostream>
#include <vector>
#include "InputData.h"
#include "Random.h"
#include "SA.h"

#define PI 3.14159265
 
// Rastrigin´s objective function
double objfun_rastrigin(std::vector<double> lX, InputData *data)
{
    double lx_sumterm = 0.00; double Y = 0.0;
    for (unsigned int j = 0; j < lX.size(); j++)
    {
        lx_sumterm += (pow(lX[j],2)) - 10*cos(2*PI*lX[j]);
    }
    Y = data->nRast*data->nr_independent_variables + lx_sumterm;
    return Y;
}

int main()
{
    /// ----------- Initializing Simulated Annealing method -----------  ///
    InputData inputobj;
    SA sim_annealing(&inputobj);
    sim_annealing.Optimize(objfun_rastrigin);
    cout << endl << "----------------------------------------------------------------------" << endl << endl;
}
