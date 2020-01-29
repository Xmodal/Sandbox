//
//  Izhikevich.h
//  
//
//  Created by Atsushi Masumori on 2013/12/21.
//
//
#pragma once
#ifndef ____Izhikevich__
#define ____Izhikevich__

#include <iostream>
#include <vector>

using namespace std;

class Izhikevich
{
private:
    double potentialThreshold;
    double u;//membrane recovery potential
    double v;//membrane potential
    int type;
    double I;
    int spikedTime;
    double a, b, c, d;// parameter for the differential equation

public:
    Izhikevich();
    ~Izhikevich();

    void update();
    void setParam(int type, double a, double b, double c, double d, double _u, double _v, double _I);
    void setNeuronType(int type);
    bool checkFiring();
    int getNeuronType();
    int getSpikedTime();
    void setSpikedTime(int time);
    bool isFiring();
    
    void setA(double a){ this->a = a; };
    void setB(double b){ this->b = b; }
    void setC(double c){ this->c = c; }
    void setD(double d){ this->d = d; }

    double getA();
    double getB();
    double getC();
    double getD();
    double getU();
    double getV();
    double getI();
    void setI(double i);
    void addToI(double a);
    
    bool firing;

};

#endif /* defined(____Izhikevich__) */
