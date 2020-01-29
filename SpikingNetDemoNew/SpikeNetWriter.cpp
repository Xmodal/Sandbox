//
//  SpikeNetWriter.cpp
//  spiking_neurons_simulator
//
//  Created by atsmsmr on 2016/05/30.
//  Copyright (c) 2016年 Atsushi Masumori. All rights reserved.
//

#include "SpikeNetWriter.h"

SpikeNetWriter::SpikeNetWriter(){
    
}

SpikeNetWriter::~SpikeNetWriter(){
    
}

void SpikeNetWriter::open(std::string filepath){
    sn_stream.open(filepath);
};

void SpikeNetWriter::open(char* filepath){
    sn_stream.open(filepath);
};

void SpikeNetWriter::write(int frame_current, double value){
    sn_stream << frame_current << "," << value << std::endl;
};

void SpikeNetWriter::writeWeights(int frame_current, double** weights){
    
    for(int i=0; i<ConstParams::Number_Of_Neurons; i++){
        for(int j=0; j<ConstParams::Number_Of_Neurons; j++){
            
            sn_stream << frame_current << "," << i << "," << j << "," << weights[i][j] << "\n";
        }
    }
    
};

void SpikeNetWriter::writeSpikes(int frame_current, std::vector<int> spiked_neuron_ids){
    
    for(int i=0; i<spiked_neuron_ids.size(); i++){
        sn_stream << frame_current << "," << spiked_neuron_ids[i] << "\n";
    }
    
};


void SpikeNetWriter::writeSome(std::vector<double> some_values){
    
    for(int i=0; i<some_values.size(); i++){
        sn_stream << some_values[i];
        if(i != some_values.size()-1) sn_stream << ",";
        else sn_stream << "\n";
    }
};

void SpikeNetWriter::writeSettings(){
    
    struct tm *date;
    time_t now;
    int year, month, day;
    int hour, minute, second;
    
    time(&now);
    date = localtime(&now);
    
    year = date->tm_year + 1900;
    month = date->tm_mon + 1;
    day = date->tm_mday;
    hour = date->tm_hour;
    minute = date->tm_min;
    second = date->tm_sec;
    
    sn_stream << "----------Information------------------------------\n";
    sn_stream << "Start Date :                   " << year << "/" << month << "/" << day << "/ " << hour << ":" << minute << ":" << second << "\n";
    sn_stream << "Spike Data :                   " << ConstParams::SaveSpikes             << "\n";
    sn_stream << "Weight Data :                  " << ConstParams::SaveWeights            << "\n";
    
    sn_stream << "----------Network settings-------------------------\n";
    sn_stream << "Neurons :                      " << ConstParams::Number_Of_Neurons      << "\n";
    sn_stream << "Connections :                  " << ConstParams::Number_Of_Connection   << "\n";
    sn_stream << "Inhibitory Neurons:            " << ConstParams::Number_Of_Inhibitory   << "\n";
    
    sn_stream << "Exc Neuron Type :              " << ConstParams::Excitatory_Neuron_Type << "\n";
    sn_stream << "Inh Neuron Type :              " << ConstParams::Inhibitory_Neuron_Type << "\n";
    sn_stream << "Network Type :                 " << ConstParams::Network_Type           << "\n";
    
    sn_stream << "Input Neurons :                " << ConstParams::Input_Neuron_Size      << "\n";
    sn_stream << "Input Groups :                 " << ConstParams::Input_Group_Size       << "\n";
    sn_stream << "Output Neurons :               " << ConstParams::Output_Neuron_Size     << "\n";
    sn_stream << "Output Groups :                " << ConstParams::Output_Group_Size      << "\n";
    
    
    sn_stream << "----------Experimental settings--------------------\n";
    sn_stream << "Direct Connection I/O :        " << ConstParams::Direct_Connection_Flag << "\n";
    sn_stream << "STP :                          " << ConstParams::Stp_Flag               << "\n";
    sn_stream << "STDP :                         " << ConstParams::Stdp_Flag              << "\n";
    
    sn_stream << "----------Model settings---------------------------\n";
    sn_stream << "Initial Weight Ratio - Exc :   " << ConstParams::Init_Weight_Ratio_Exc  << "\n";
    sn_stream << "Initial Weight Ratio - Inh :   " << ConstParams::Init_Weight_Ratio_Inh  << "\n";
    sn_stream << "Noise Ratio - Exc :            " << ConstParams::Noise_Ratio_Exc        << "\n";
    sn_stream << "Noise Ratio - Inh :            " << ConstParams::Noise_Ratio_Inh        << "\n";
    sn_stream << "Weight Max :                   " << ConstParams::Weight_Max             << "\n";
    sn_stream << "Weight Min :                   " << ConstParams::Weight_Min             << "\n";
    sn_stream << "Stim Rate :                    " << ConstParams::Stim_Rate              << "\n";
    sn_stream << "Stim Strength :                " << ConstParams::Stim_Strength          << "\n";
    
    sn_stream << std::endl;
    
}

void SpikeNetWriter::close(){
    sn_stream.close();
}
