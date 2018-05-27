#include "ExampleFunction.h"
#include <cmath>
#include <iostream>

// minimize 3*x^2 + 2*x*y + 2*y^2 + 7
using namespace std;

ExampleFunction::ExampleFunction(unsigned dimension, double gamma, double lambda)
{
    _dimension = dimension;
    _gamma = gamma;
    _lambda = lambda;
}

void ExampleFunction::evaluateFG(const vector<double> &x, double &f, vector<double> &g)
{
    double xExp = 0, yExp = 0, mXExp = 0, mYExp = 0, xExpMul = 0, yExpMul = 0, mXExpMul = 0, mYExpMul = 0;
    double xMax = 0, yMax = 0, xMin = 0, yMin = 0;
    double dense = 0;
    for(unsigned i = 0; i < (x.size() / 2); ++i)
    {
        xExp     += exp(x[2 * i + 0] / _gamma)     ;
        yExp     += exp(x[2 * i + 1] / _gamma)     ;
        mXExp    += exp(x[2 * i + 0] / _gamma * -1);
        mYExp    += exp(x[2 * i + 1] / _gamma * -1);
        xExpMul  += exp(x[2 * i + 0] / _gamma)      * x[2 * i + 0];
        yExpMul  += exp(x[2 * i + 1] / _gamma)      * x[2 * i + 1];
        mXExpMul += exp(x[2 * i + 0] / _gamma * -1) * x[2 * i + 0];
        mYExpMul += exp(x[2 * i + 1] / _gamma * -1) * x[2 * i + 1];
    }
    xMax = xExpMul / xExp;
    yMax = yExpMul / yExp;
    xMin = mXExpMul / mXExp;
    yMin = mYExpMul / mYExp;
    double WA = xMax - xMin + yMax - yMin;
    f = WA + _lambda * dense * dense; // objective function

    for(unsigned i = 0; i < (x.size() / 2); ++i)
    {
        double son;
        double mom;
        
        double xTmp = x[2 * i + 0];
        double xExpTmp = exp(xTmp / _gamma);
        son = ( xExp * xExpTmp ) + ( 1 / _gamma * xExp * xTmp * xExpTmp ) - ( 1 / _gamma * xExpMul * xExpTmp );
        mom = xExp * xExp;
        g[2 * i + 0] = son / mom; // gradient function of X
        double mXExpTmp = exp(xTmp / _gamma * -1);
        son = ( mXExp * mXExpTmp ) - ( 1 / _gamma * mXExp * xTmp * mXExpTmp ) + ( 1 / _gamma * mXExpMul * mXExpTmp );
        mom = mXExp * mXExp;
        g[2 * i + 0] -= son / mom; // gradient function of X
        
        double yTmp = x[2 * i + 1];
        double yExpTmp = exp(yTmp / _gamma);
        son = ( yExp * yExpTmp ) + ( 1 / _gamma * yExp * yTmp * yExpTmp ) - ( 1 / _gamma * yExpMul * yExpTmp );
        mom = yExp * yExp;
        g[2 * i + 1] = son / mom; // gradient function of Y
        double mYExpTmp = exp(yTmp / _gamma * -1);
        son = ( mYExp * mYExpTmp ) - ( 1 / _gamma * mYExp * yTmp * mYExpTmp ) + ( 1 / _gamma * mYExpMul * mYExpTmp );
        mom = mYExp * mYExp;
        g[2 * i + 1] -= son / mom; // gradient function of Y
    }
}

void ExampleFunction::evaluateF(const vector<double> &x, double &f)
{
    double xExp = 0, yExp = 0, mXExp = 0, mYExp = 0, xExpMul = 0, yExpMul = 0, mXExpMul = 0, mYExpMul = 0;
    double xMax = 0, yMax = 0, xMin = 0, yMin = 0;
    double dense = 0;
    for(unsigned i = 0; i < (x.size() / 2); ++i)
    {
        xExp     += exp(x[2 * i + 0] / _gamma)     ;
        yExp     += exp(x[2 * i + 1] / _gamma)     ;
        mXExp    += exp(x[2 * i + 0] / _gamma * -1);
        mYExp    += exp(x[2 * i + 1] / _gamma * -1);
        xExpMul  += exp(x[2 * i + 0] / _gamma)      * x[2 * i + 0];
        yExpMul  += exp(x[2 * i + 1] / _gamma)      * x[2 * i + 1];
        mXExpMul += exp(x[2 * i + 0] / _gamma * -1) * x[2 * i + 0];
        mYExpMul += exp(x[2 * i + 1] / _gamma * -1) * x[2 * i + 1];
    }
    xMax = xExpMul / xExp;
    yMax = yExpMul / yExp;
    xMin = mXExpMul / mXExp;
    yMin = mYExpMul / mYExp;
    double WA = xMax - xMin + yMax - yMin;
    f = WA + _lambda * dense * dense; // objective function
}

unsigned ExampleFunction::dimension()
{
    return _dimension; // num_blocks*2 
    // each two dimension represent the X and Y dimensions of each block
}
