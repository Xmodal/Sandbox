//
//  neuronCtrl.cpp
//  SpikingNeuronSimulator
//
//  Created by atsmsmr on 2014/10/20.
//  Copyright (c) 2014年 Atsushi Masumori. All rights reserved.
//

#include "SpikingNet.h"
#include <iomanip>

SpikingNet::SpikingNet(){
    
    stdp_tau = 20;
        
    //set random seed
    if(ConstParams::Random_Device_Flag)
    {
        std::random_device rnd;
        rand_gen.seed(rnd());
    }
    else
    {
        rand_gen.seed(ConstParams::Random_Seed);
    }
    
    
    neurons.resize(ConstParams::Number_Of_Neurons);
    input_neurons.resize(ConstParams::Input_Neuron_Size, 0);
    output_neurons.resize(ConstParams::Output_Neuron_Size, 0);
    spiked_num_of_output_group.resize(ConstParams::Output_Group_Size, 0);
    
    stdp_spiked_time.resize(ConstParams::Number_Of_Neurons, 0);
    stdp_counts.resize(ConstParams::Number_Of_Neurons, 0);
    
    //STP (according to Science paper)
    stp_u = new double[Number_Of_Neurons];
    stp_x = new double[Number_Of_Neurons];
    stp_wf = new double[Number_Of_Neurons];
    
    weights = new double*[Number_Of_Neurons];
    for(int i=0; i<Number_Of_Neurons; ++i){
        weights[i] = new double[Number_Of_Neurons];
    }
    
}

SpikingNet::~SpikingNet(){
    
    // delete STP variables
    delete[] stp_u;
    delete[] stp_x;
    delete[] stp_wf;
    stp_u = 0;
    stp_x = 0;
    stp_wf = 0;
    
    for(int i=0; i<ConstParams::Number_Of_Neurons; ++i){
        delete[] weights[i];
        weights[i] = 0;
    }
    delete[] weights;
    weights = 0;
}


void SpikingNet::init(){
    
    for(int i=0; i<ConstParams::Number_Of_Neurons; ++i){
        for(int j=0; j<ConstParams::Number_Of_Neurons; ++j){
            weights[i][j] = 0.;
        }
    }
    
    neurons.clear();
    input_neurons.clear();
    output_neurons.clear();
    spiked_num_of_output_group.clear();
    stdp_spiked_time.clear();
    stdp_counts.clear();
    
    neurons.resize(ConstParams::Number_Of_Neurons);
    input_neurons.resize(ConstParams::Input_Neuron_Size, 0);
    output_neurons.resize(ConstParams::Output_Neuron_Size, 0);
    spiked_num_of_output_group.resize(ConstParams::Output_Group_Size, 0);
    
    stdp_spiked_time.resize(ConstParams::Number_Of_Neurons, 0);
    stdp_counts.resize(ConstParams::Number_Of_Neurons, 0);
    
    //initialization of some variables
    frameCount = 0;
    
    for(int i=0; i<Number_Of_Neurons; ++i){
        
        stdp_spiked_time[i] = 0;
        stdp_counts[i] = 0;
        
        // initialize STP variables
        stp_wf[i] = 1.0;
        stp_x[i] = 1.0;
        stp_u[i] = 0.0;
    }
    
    for(int i=0; i<Number_Of_Neurons; ++i){
        if(i < Number_Of_Inhibitory){
            neurons[i].setNeuronType(ConstParams::Inhibitory_Neuron_Type);
        }else{
            neurons[i].setNeuronType(ConstParams::Excitatory_Neuron_Type);
        }
    }
    
    setInputNeurons();
    setOutputNeurons();
    
    // set network
    switch(Network_Type){
            
        case Sparse_Network:
            setSparseNetwork(ConstParams::Direct_Connection_Flag);
            break;
            
        case Random_Network:
            setRandomNetwork();
            break;
        case Uniform_Network:// it is for debugging.
            setUniformNetwork();
            break;
    }
}

void SpikingNet::setInputNeurons(){
    
    std::cout << "Input neurons : ";
    
    for(int i=0; i<ConstParams::Input_Neuron_Size; ++i){
        input_neurons[i] = Number_Of_Inhibitory + i;
        std::cout << input_neurons[i] << ",";
    }
    
    std::cout << std::endl;
}

void SpikingNet::setOutputNeurons(){
    
    std::cout << "Output neurons : ";
    
    for(int i=0; i<Output_Neuron_Size; ++i){
        output_neurons[i] = Number_Of_Inhibitory + Input_Neuron_Size + i;
        std::cout << output_neurons[i] << ",";
    }
    std::cout << std::endl;
}


void SpikingNet::setSparseNetwork(bool is_directly_connected){
    
    std::uniform_int_distribution<> rand_uni_neurons(0, Number_Of_Neurons-1);
    std::uniform_real_distribution<> rand_uni(0.0, 1.0);
    
    int sum_connection = 0;
    int* dest_array;
    
    // in case of non fully connected
    if(ConstParams::Number_Of_Connection != ConstParams::Number_Of_Neurons)
    {
        for(int src_id=0; src_id<Number_Of_Neurons; ++src_id){
            
            dest_array = new int[ConstParams::Number_Of_Connection];
            for(int n=0; n<ConstParams::Number_Of_Connection; n++){
                dest_array[n] = 0;
            }
            
            int count = 0;
            while(count < ConstParams::Number_Of_Connection){
                
                int candidate_id = rand_uni_neurons(rand_gen);
                // check wether the candidate_id is already included in dest_array.
                bool is_included = any(candidate_id, dest_array, count);
                
                if(!is_included){
                    dest_array[count] = candidate_id;
                    count++;
                    sum_connection++;
                }
            }
            
            for(int j=0; j<ConstParams::Number_Of_Connection; j++){
                
                int dest_id = dest_array[j];
                if(dest_id != src_id){
                    
                    if(dest_id >=0 && dest_id < ConstParams::Number_Of_Neurons){
                        if(src_id < ConstParams::Number_Of_Inhibitory) {
                            weights[src_id][dest_id] = Init_Weight_Ratio_Inh*rand_uni(rand_gen);
                        }else{
                            weights[src_id][dest_id] = Init_Weight_Ratio_Exc*rand_uni(rand_gen);
                        }
                    }
                }
            }
            
            delete[] dest_array;
            dest_array = 0;
        }
    }
    // in case of fully connected
    else
    {
        for(int i=0; i<Number_Of_Neurons; ++i){
            for(int j=0; j<Number_Of_Neurons; j++){
                
                if(i != j){
                    
                    if(i < ConstParams::Number_Of_Inhibitory) {
                        weights[i][j] = Init_Weight_Ratio_Inh*rand_uni(rand_gen);
                        
                    } else {
                        weights[i][j] = Init_Weight_Ratio_Exc*rand_uni(rand_gen);
                    }
                    sum_connection++;
                }
            }
        }
        
    }
    
    // delete direct connection between input and output
    if(is_directly_connected == false){
        for(int src_id=0; src_id<ConstParams::Input_Neuron_Size; src_id++){
            for(int dest_id=0; dest_id<ConstParams::Output_Neuron_Size; dest_id++){
                weights[input_neurons[src_id]][output_neurons[dest_id]] = 0.0;
            }
        }
    }
    
    printf("sum of connection: %d\n", sum_connection);
}

// fully connected with same weight : This is for debug.
void SpikingNet::setUniformNetwork(){
    
    int sum_connection = 0;
    
    for(int i=0; i<Number_Of_Neurons; ++i){
        for(int j=0; j<Number_Of_Neurons; j++){
            
            if(i != j){
                
                if(i < Number_Of_Inhibitory) {
                    weights[i][j] = Init_Weight_Ratio_Inh;
                    
                } else {
                    weights[i][j] = Init_Weight_Ratio_Exc;
                    if(j%3==0) weights[i][j] = Init_Weight_Ratio_Exc*1.50;
                    
                }
                sum_connection++;
            }
        }
    }
    printf("sum of connection: %d\n", sum_connection);
}

// fully connected network with random weight
void SpikingNet::setRandomNetwork(){
    
    std::uniform_int_distribution<> rand_uni_neurons(0, Number_Of_Neurons-1);
    std::uniform_real_distribution<> rand_uni(0.0, 1.0);
    
    int sum_connection = 0;
    
    for(int i=0; i<Number_Of_Neurons; ++i){
        for(int j=0; j<ConstParams::Number_Of_Neurons; j++){
            
            if(i != j){
                if(i < Number_Of_Inhibitory) {
                    weights[i][j] = Init_Weight_Ratio_Inh*rand_uni(rand_gen);
                    
                } else {
                    weights[i][j] = Init_Weight_Ratio_Exc*rand_uni(rand_gen);
                }
                sum_connection++;
            }
        }
    }
    printf("sum of connection: %d\n", sum_connection);
    
}


int* SpikingNet::getPartOutputNeuron(int start_index, int size){
    
    int* part_output = new int[size];
    for(int i=0; i<size; ++i){
        part_output[i] = output_neurons[i+start_index];
    }
    
    return part_output;
}

void SpikingNet::checkFiring(){
    
    spiked_neuron_id.clear();
    
    for(int i=0; i<Number_Of_Neurons; ++i){
        
        if(neurons[i].checkFiring()) {
            spiked_neuron_id.push_back(i);
            spiked_neuron_id_cum.push_back(i);
        }
    }
}


void SpikingNet::clearSpikedNeuronId(){
    spiked_num_of_output_group.clear();
    spiked_num_of_output_group.resize(ConstParams::Output_Group_Size, 0);
    spiked_neuron_id_cum.clear();
}

void SpikingNet::checkTask(){
    
    // Remove overlaps
    std::sort(spiked_neuron_id_cum.begin(), spiked_neuron_id_cum.end());
    spiked_neuron_id_cum.erase(std::unique(spiked_neuron_id_cum.begin(), spiked_neuron_id_cum.end()), spiked_neuron_id_cum.end() );
    
    
    int group_size = floor((float)ConstParams::Output_Neuron_Size / (float)ConstParams::Output_Group_Size);
    
    for(int i=0; i<spiked_neuron_id_cum.size(); i++){
        
        for(int j=0; j<ConstParams::Output_Group_Size; j++){
            
            int* output_group = getPartOutputNeuron(j*group_size, group_size); // TODO: refactor this for fast computing.
            
            if(any(spiked_neuron_id_cum[i], output_group, group_size)){
                //                std::cout << spiked_neuron_id_cum[i] << " - spiked" << " - frame - " << frameCount << std::endl;
                spiked_num_of_output_group[j] += 1;
            }
            delete output_group;
        }
    }
}

int SpikingNet::getSpikedOutput(int index){
    return spiked_num_of_output_group[index];
}


void SpikingNet::update_input(){
    
    //setup normal distribution random function
    std::normal_distribution<> normalRand(0.0, 1.0);
    
    //pseudo thalamus noise-input
    for(int i=0; i<Number_Of_Neurons; ++i){
        
        if(i < Number_Of_Inhibitory ){
            neurons[i].addToI(Noise_Ratio_Inh*normalRand(rand_gen));
        }else{
            neurons[i].addToI(Noise_Ratio_Exc*normalRand(rand_gen));
        }
    }
    
    //input from connected neurons with STP
    for(int i=0; i<Number_Of_Neurons; ++i){
        if(neurons[i].isFiring()){
            for(int j=0; j<Number_Of_Neurons; ++j){
                
                if(i != j){
                    if(i>Number_Of_Inhibitory)
                    neurons[j].addToI((float)weights[i][j]*(float)stp_wf[i]);
                    else
                    neurons[j].addToI((float)weights[i][j]);
                }
            }
        }
    }

}

void SpikingNet::update_neuron(){
    
    // update differential equation
    for(int i=0; i<Number_Of_Neurons; ++i){
        neurons[i].update();
        neurons[i].setI(0.0);
    }
    
}


void SpikingNet::stdp(){
    
    for(int i=Number_Of_Inhibitory; i<Number_Of_Neurons; ++i){
        if(stdp_counts[i]>0) stdp_counts[i] = stdp_counts[i] - 1;
        if(neurons[i].isFiring()) stdp_counts[i] = stdp_tau;
    }
    
    double d;
    for(int i=Number_Of_Inhibitory; i<Number_Of_Neurons; i++){
        
        if(neurons[i].isFiring()){
            
            for(int j=Number_Of_Inhibitory; j<Number_Of_Neurons; j++){
                
                if(stdp_counts[j]>0 && stdp_counts[j] != stdp_tau && i != j){
                    
                    d = (0.1 * pow(0.55, (stdp_tau-stdp_counts[j])));
                    
                    //check that neuron linked to i fired less than tau_ms before (but is not firing right now)
                    //if j fired before i, then weight j->i ++
                    if(weights[j][i] != 0.0){
                        weights[j][i] += d;
                        if (weights[j][i] > Weight_Max) weights[j][i] = Weight_Max;
                        
                    }
                    
                    //now weight from i to j, should be lowered if i fired before j
                    if(weights[i][j] != 0.0){
                        weights[i][j] -= d;
                        if(weights[i][j] < Weight_Min) weights[i][j] = Weight_Min;
                        
                    }
                }
            }
        }
    }
}

void SpikingNet::stp(){
    
    for(int i=Number_Of_Inhibitory; i<Number_Of_Neurons; ++i){
        
        if(neurons[i].isFiring()){
            stp_wf[i] = getStpValue(i,1);
        }else{
            stp_wf[i] = getStpValue(i,0);
        }
    }
}

double SpikingNet::getStpValue(int index, int is_firing){
    double wf = 0.;
    
    double s = double(is_firing);
    double u = stp_u[index];
    double x = stp_x[index];
    
    double tau_d = 200.;//ms
    double tau_f = 600.;//ms
    double U = 0.2;//mV
    double dx = (1.0-x)/tau_d - u*x*s;
    double du = (U - u)/tau_f + U*(1.0-u)*s;
    
    double nu = u+du;
    double nx = x+dx;
    
    wf = nu*nx;
    stp_u[index] = nu;
    stp_x[index] = nx;
    return wf;
}

void SpikingNet::decay(){
    
    for(int i=Number_Of_Inhibitory; i<Number_Of_Neurons; i++){
        for(int j=Number_Of_Inhibitory; j<Number_Of_Neurons; j++){
            weights[i][j] = weights[i][j] * ConstParams::Decay_Rate;
        }
    }
    
}

void SpikingNet::update(){
    
    checkFiring();
    if(ConstParams::Decay_Flag) decay();
    if(ConstParams::Stdp_Flag)  stdp();
    if(ConstParams::Stp_Flag)   stp();
    
    update_input();
    update_neuron();
    
    frameCount++;
}

void SpikingNet::stimulation(){
    // external input to input neurons.
    for(int i=0; i<ConstParams::Input_Neuron_Size; ++i){
        neurons[input_neurons[i]].addToI(ConstParams::Stim_Strength);
    }
}

void SpikingNet::stimulation(double stim_strength_){
    // external input to input neurons.
    for(int i=0; i<ConstParams::Input_Neuron_Size; ++i){
        neurons[input_neurons[i]].addToI(stim_strength_);
    }
}

void SpikingNet::stimulation(int group_id_, double stim_strength_){
    
    int group_size = floor((float)ConstParams::Input_Neuron_Size / (float)ConstParams::Input_Group_Size);
    
    // external input to input neurons.
    for(int i=0; i<ConstParams::Input_Group_Size; ++i){
        for(int j=(i*group_size); j<((i+1)*group_size); ++j){
            if(i == group_id_){
                neurons[input_neurons[j]].addToI(stim_strength_);
            }else if(i > group_id_){
                break;
            }
        }
    }
}

void SpikingNet::wholeStimulation(){
    //    external input
    for(int i=0; i<ConstParams::Input_Neuron_Size; ++i){
        neurons[input_neurons[i]].addToI(ConstParams::Stim_Strength);
    }
}

void SpikingNet::wholeStimulation(double stim_strengh_){
    //    external input
    
    for(int i=ConstParams::Number_Of_Inhibitory; i<ConstParams::Number_Of_Neurons; ++i){
        if(any(i, input_neurons)==true){
            neurons[i].addToI(stim_strengh_);
        }
    }
}

bool SpikingNet::any(int target, const int *reference, int reference_size){
    bool answer = false;
    for(int i=0; i<reference_size; ++i){
        if(target == reference[i]){
            answer = true;
            break;
        }
    }
    return answer;
}

bool SpikingNet::any(int target, std::vector<int> reference){
    bool answer = false;
    for(int i=0; i<reference.size(); ++i){
        if(target == reference[i]){
            answer = true;
            break;
        }
    }
    return answer;
}
