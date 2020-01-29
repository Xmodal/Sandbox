//
//  Izhikevich.cpp
//
//
//  Created by Atsushi Masumori on 2013/12/21.
//
//

#include "Izhikevich.h"

Izhikevich::Izhikevich(){
    
    firing = false;
    
    //set param as default Ne.
    double re = drand48();
    a = 0.02;
    b = 0.2;
    c = -65. + 15.*re*re;
    d = 8. - 6.*re*re;
    v = -65.;
    u = b*v;
    I = 0.;
    potentialThreshold = 20.;
    spikedTime = 0.;
}

Izhikevich::~Izhikevich(){
}

void Izhikevich::setParam(int _type, double _a, double _b, double _c, double _d, double _u, double _v, double _I){
//    this->firing = false;
    this->type = _type;
    this->a = _a;
    this->b = _b;
    this->c = _c;
    this->d = _d;
    this->v = _u;
    this->d = _v;
    this->I = _I;
}

void Izhikevich::setNeuronType(int _type){
    type = _type;
    switch(type){
        case 1://regular spiking neuron in demo
        {
            double re = drand48();
            a = 0.02;
			b = 0.2;
			c = -65. + 15.*re*re;
			d = 8. - 6.*re*re;
            break;
        }
        case 2://resonator in demo
        {
            double ri = drand48();
			a = 0.02 + 0.08*ri;
			b = 0.25 - 0.05*ri;
			c = -65.;
			d = 2.;
            break;
        }
        case 3://regular spiking neuron
        {
			a = 0.02;
			b = 0.2;
			c = -65.;
			d = 8.;
            break;
        }
        case 4://resonator
        {
			a = 0.1;
			b = 0.2;
			c = -65.;
			d = 2.;
            break;
        }
        case 5://chattering neuron
        {
			a = 0.02;
			b = 0.2;
			c = -50.;
			d = 2.;
            break;
        }
        case 6://inhivitory in izhikebich
        {
			a = 0.1;
			b = 0.2;
			c = -65.;
			d = 2.;
            break;
        }
        case 7://exitatory in itsukivich
        {
            double re = drand48();
            a = 0.02;
			b = 0.2;
			c = -65. + 15.*re*re;
			d = 8. - 6.*re*re;
            break;
        }
        case 8://inhivitory in itsukivich
        {
            double ri = drand48();
			a = 0.1;
			b = 0.25 - 0.05*ri;
			c = -65.;
			d = 2.;
            break;
        }
            
    }
    v = -65.;
    u = d;
    I = 0.;
}


void Izhikevich::update(){
    v = v + 0.5*(0.04*v*v + 5*v +140 - u + I);
    v = v + 0.5*(0.04*v*v + 5*v +140 - u + I);
    u = u + a*(b*v-u);
}

bool Izhikevich::checkFiring(){
    
    if(v>potentialThreshold) {
        firing = true;
        v = c;
        u = u + d;
//        I = 0;
    }else{
        firing = false;
    }
    return(firing);
}

int Izhikevich::getNeuronType(){
    return(type);
}

int Izhikevich::getSpikedTime(){
    return(spikedTime);
}

void Izhikevich::setSpikedTime(int time){
    spikedTime = time;
}

bool Izhikevich::isFiring(){
    return(this->firing);
}

double Izhikevich::getA(){
    return(this->a);
}

double Izhikevich::getB(){
    return(this->b);
}

double Izhikevich::getC(){
    return(this->c);
}

double Izhikevich::getD(){
    return(this->d);
}

double Izhikevich::getU(){
    return(u);
}

double Izhikevich::getV(){
    return(v);
}

double Izhikevich::getI(){
    return(this->I);
}

void Izhikevich::setI(double i){
    this->I = i;
}

void Izhikevich::addToI(double a){
    this->I = this->I + a;
}
