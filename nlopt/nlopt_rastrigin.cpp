#include <iomanip>
#include <iostream>
#include <vector>
#include <math.h>
#include <omp.h>
#include <nlopt.hpp>
#include <vector>
#include "Random.h"

#define PI 3.14159265

typedef struct {
    double a, b;
} my_constraint_data;

// Rastrigin´s objective function
double objfun_rastrigin(const std::vector<double> &lX, std::vector<double> &grad, void *my_func_data)
{
    // // This code re-interprets void *my_func_data as pointer of the correct type during runtime:
    // static double wrap(const std::vector<double> &x, std::vector<double> &grad, void *data) {
    //     return (*reinterpret_cast<MyFunction*>(data))(x, grad); }

    double lx_sumterm = 0.00; double Y = 0.0;
    for (unsigned int j = 0; j < lX.size(); j++)
    {
        lx_sumterm += (pow(lX[j],2)) - 10*cos(2*PI*lX[j]);
    }
    // Y = data->nRast*data->nr_independent_variables + lx_sumterm;
    Y = (10)*2.0 + lx_sumterm;
    return Y;
}

// double myvconstraint(const std::vector<double> &x, std::vector<double> &grad, void *data)
// {
//     my_constraint_data *d = reinterpret_cast<my_constraint_data*>(data);
//     double a = d->a, b = d->b;
//     if (!grad.empty()) {
//         grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
//         grad[1] = -1.0;
//     }
//     return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
// }

int main ()
{
    //nlopt::opt opt(nlopt::LD_MMA, 2);

    // nlopt::opt opt_Rec(nlopt::GN_DIRECT, 2);
    // nlopt::opt opt_Rec(nlopt::GN_DIRECT_L, 2);
    // //nlopt::opt opt_Rec(nlopt::GN_DIRECT_L_RAND, 2);
    // nlopt::opt opt_Rec(nlopt::GN_DIRECT_NOSCAL, 2);
    // //nlopt::opt opt_Rec(nlopt::GN_DIRECT_L_NOSCAL, 2);
    // // nlopt::opt opt_Rec(nlopt::GN_DIRECT_L_RAND_NOSCAL, 2);

    // nlopt::opt opt_Rec(nlopt::GN_CRS2_LM, 2); // Low performance
    // nlopt::opt opt_Rec(nlopt::GD_STOGO, 2); 
    // nlopt::opt opt_Rec(nlopt::GN_AGS, 2); 
    // nlopt::opt opt_Rec(nlopt::GN_ISRES, 2); 
    nlopt::opt opt_Rec(nlopt::GN_ESCH, 2); // Low performance

    // // nlopt::opt opt_Rec(nlopt::G_MLSL, 2); //OK
    // nlopt::opt opt_Rec(nlopt::G_MLSL_LDS, 2); //OK
    // //nlopt::opt opt_Rec_local(nlopt::LN_COBYLA, 2);
    // nlopt::opt opt_Rec_local(nlopt::LN_BOBYQA, 2);
    // opt_Rec.set_local_optimizer(opt_Rec_local);

    std::vector<double> lb_Rec(2, 0);
    std::vector<double> ub_Rec(2, 0);
    for (int i = 0 ; i < 2; i++ )
    {
        ub_Rec[i] = 10;
        lb_Rec[i] = -10;
    }
    opt_Rec.set_lower_bounds(lb_Rec);
    opt_Rec.set_upper_bounds(ub_Rec);
    //opt_Rec.set_min_objective(MyFunction::wrap, &myFuncObj); // in case of function-object input
    opt_Rec.set_min_objective(objfun_rastrigin, NULL);



    std::vector<double> x_Rec(2, 0);
    Random mrsn;
    mrsn.SeedInput();
    for (int i = 0; i < 2; i++)
    {
        x_Rec[i] = mrsn.dRN(lb_Rec[i], ub_Rec[i]);
        std::cout << "x0[" << i << "] : " << x_Rec[i] << std::endl;
    }
    
    //opt.set_xtol_abs(0.60*Fobj_preajuste);
    opt_Rec.set_maxeval(20);
    //opt_Rec.set_xtol_rel(1e-4);

    double minf_Rec;
    try
    {
        //nlopt::result result = opt_Rec.set_local_optimizer(opt_Rec_local);
        nlopt::result result = opt_Rec.optimize(x_Rec, minf_Rec);
        std::cout << std::fixed << std::setprecision(2) << "found minimum at f("<< x_Rec[0] <<"," << x_Rec[1] << ") = "  << minf_Rec << std::endl;
    }
    catch (std::exception &e) 
    {
        std::cout << "nlopt failed: " << e.what() << std::endl;
    }
    
    std::cout << std::endl<< "END" << std::endl;
}



