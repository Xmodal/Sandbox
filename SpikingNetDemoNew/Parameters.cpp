//
//  File.cpp
//  SpikingNeuronsSimulator_Robot
//
//  Created by atsmsmr on 2015/06/21.
//
//

#include "Parameters.h"

// network settings
int ConstParams::Number_Of_Neurons = 324;
int ConstParams::Number_Of_Inhibitory = Number_Of_Neurons/5;
int ConstParams::Number_Of_Connection = 20; //each neuron

int ConstParams::Network_Type = ConstParams::Sparse_Network;
const int ConstParams::Excitatory_Neuron_Type = ConstParams::Regular_Spiking;
const int ConstParams::Inhibitory_Neuron_Type = ConstParams::Resonator;

int ConstParams::Input_Neuron_Size  = ConstParams::Number_Of_Neurons/5;
int ConstParams::Output_Neuron_Size = ConstParams::Number_Of_Neurons/5;
int ConstParams::Output_Group_Size = 2; //2;
int ConstParams::Input_Group_Size  = 2; //2;

// synapse settings
double ConstParams::Init_Weight_Ratio_Exc = 5.0;//5.0, 6.0
double ConstParams::Init_Weight_Ratio_Inh = -5.0;//-5.0, -5.0
double ConstParams::Weight_Max = 20.0;
double ConstParams::Weight_Min = 0.0;
const double ConstParams::Decay_Rate = 0.9999995;

// input-current settings
double ConstParams::Noise_Ratio_Exc = 5.0;
double ConstParams::Noise_Ratio_Inh = 3.0;
int    ConstParams::Stim_Rate = 1;
double ConstParams::Stim_Strength = 6.0;

// flags
bool ConstParams::Stp_Flag               = true;
bool ConstParams::Stdp_Flag              = true;
bool ConstParams::Decay_Flag             = true;
bool ConstParams::Direct_Connection_Flag = true;

// utilities settings
bool ConstParams::Debug_Flag       = false;
bool ConstParams::Show_Msg_Flag    = false;
bool ConstParams::Random_Device_Flag = true;
int  ConstParams::Random_Seed = (unsigned) time(NULL);
const bool ConstParams::SaveSpikes   = false;
const bool ConstParams::SaveWeights  = false;


