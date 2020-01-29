//
//  SpikeNetWriter.h
//  spiking_neurons_simulator
//
//  Created by atsmsmr on 2016/05/30.
//  Copyright (c) 2016年 Atsushi Masumori. All rights reserved.
//
#pragma once
#ifndef __spiking_neurons_simulator__SpikeNetWriter__
#define __spiking_neurons_simulator__SpikeNetWriter__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <time.h>

#include "Parameters.h"

class SpikeNetWriter{
private:
    
public:
    SpikeNetWriter();
    ~SpikeNetWriter();
    void open(std::string filepath);
    void open(char* filepath);
    void write (int frame_current, double value);
    void writeWeights(int frame_current, double** weights);
    void writeSpikes(int frame_current, std::vector<int> spiked_neuron_ids);
    void writeSome(std::vector<double> some_values);
    void writeSettings();
    void close();
    
    std::ofstream sn_stream;
};
#endif /* defined(__spiking_neurons_simulator__SpikeNetWriter__) */
