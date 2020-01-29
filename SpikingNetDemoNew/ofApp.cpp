#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetBackgroundAuto(true);
    ofSetVerticalSync(false);
    ofBackground(50);
    
    //ofSetFrameRate(50);
    
    spike_net.init();
    gui.setup();
    gui.add(view_mode.setup( "View mode" , false));

    
    // settings for log
    std::string save_dir = "/path/to/save/directory/";
    std::cout << save_dir + "spike_data_" << std::endl;
    
    if(ConstParams::SaveSpikes){
        writer_spike.open(save_dir + "spike_data_" + ofGetTimestampString() + ".txt");
    }
    
    if(ConstParams::SaveWeights && ofGetFrameNum() % 10000 == 0){
        writer_weight.open(save_dir + "weight_data_" + ofGetTimestampString() + ".txt");
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    spike_net.update();
    
    if(ofGetFrameNum()%100 == 0){
        spike_net.stimulation(0, 100); // (input group id, stimlation strength)
        //spike_net.stimulation(1, 100); // (input group id, stimlation strength)
    }
    
    if(ofGetFrameNum()%70 == 0){
        //spike_net.stimulation(0, 100); // (input group id, stimlation strength)
        spike_net.stimulation(1, 100); // (input group id, stimlation strength)
    }
    
    // log
    if(ConstParams::SaveSpikes){
        writer_spike.writeSpikes(ofGetFrameNum(), spike_net.getSpikedIds());
    }
    
    if(ConstParams::SaveWeights && ofGetFrameNum() % 1000 == 0){
        writer_weight.writeWeights(ofGetFrameNum(), spike_net.getWeights());
    }

}

//--------------------------------------------------------------
void ofApp::drawNeuron(){
    
    int rectsize = 20;
    int draw_mode = 1;
    float d_angle = 360./(float)ConstParams::Number_Of_Neurons;
    float angle = 0;
    
    int position_x, position_y;
    int radius_x = ofGetWindowWidth()/2 - rectsize;
    int radius_y = ofGetWindowHeight()/2 - rectsize;
    int center_x = ofGetWindowWidth()/2;
    int center_y = ofGetWindowHeight()/2;

    
    int n2 = sqrt(ConstParams::Number_Of_Neurons);
    
    for(int i=0; i<ConstParams::Number_Of_Neurons; i++){
        
        //color
        if(spike_net.neurons[i].isFiring()){
            if(spike_net.neurons[i].getNeuronType() == ConstParams::Regular_Spiking_Demo || spike_net.neurons[i].getNeuronType() == ConstParams::Regular_Spiking){
                ofSetColor(255,255,255,200);
            }else{
                ofSetColor(0,255,0,200);
            }
        }else {
            ofSetColor(0,0,0);
        }
        double val = spike_net.neurons[i].getV();
        ofSetColor(spike_net.neurons[i].getV()+100,spike_net.neurons[i].getV()+100,spike_net.neurons[i].getV()+100,spike_net.neurons[i].getV()+100);
        
        // position
        if(view_mode == 0){
            //if(spike_net.neurons[i].isFiring()){
            int margin_y = 2;
            position_x = rectsize*(i%9)+20;
            position_y = rectsize*int(i/9)+20;
            ofDrawRectangle(position_x,position_y,rectsize-2,rectsize-2);
            //}
        }else if(view_mode == 1){
            float node_size = 2*rectsize/3;

            float rad = ofDegToRad(angle);
            position_x = center_x + (radius_x * cos(rad));
            position_y = center_y + (radius_y * sin(rad));
            angle += d_angle;
            
            ofDrawEllipse(position_x,position_y,node_size,node_size);

        }
        
    }
    
}
//--------------------------------------------------------------
void ofApp::drawSynapse(){
    
    int rectsize = 20;
    int draw_mode = 1;
    
    int n2 = sqrt(ConstParams::Number_Of_Neurons);
    ofSetLineWidth(3);
    // ofSetColor(255,255,255);
    
    for(int i=0; i<ConstParams::Number_Of_Neurons; ++i){// only excitatory synapse
        for(int j=0; j<ConstParams::Number_Of_Neurons; ++j){
            
            if(spike_net.weights[i][j] > 5){
                
                // color
                int color_value = 2*spike_net.weights[i][j]*(255/ConstParams::Weight_Max);
                ofSetColor(color_value, color_value, 0);
                
                // positions in lattice mode
                if(view_mode == 0){
                    
                    int margin_y = 0;
                    int x1 = rectsize*(i%9)+rectsize;
                    int y1 = rectsize*int(i/9)+rectsize;
                    
                    int x2 = rectsize*(j%9)+rectsize;
                    int y2 = rectsize*int(j/9)+rectsize;
                    
                    
                    ofDrawLine(x1,y1+margin_y, x2,y2+margin_y);
                    
                // positions in circular mode
                }else if(view_mode == 1){
                    
                    float node_size = 2*rectsize/3;
                    
                    float radius_x = ofGetWindowWidth()/2  - node_size;
                    float radius_y = ofGetWindowHeight()/2 - node_size;
                    float center_x = ofGetWindowWidth()/2;
                    float center_y = ofGetWindowHeight()/2;
                    
                    float d_angles = 360./ConstParams::Number_Of_Neurons;
                    
                    float x1 = center_x + (radius_x * cos(ofDegToRad(d_angles*i)));
                    float y1 = center_y + (radius_y * sin(ofDegToRad(d_angles*i)));
    
                    float x2 = center_x + (radius_x * cos(ofDegToRad(d_angles*j)));
                    float y2 = center_y + (radius_y * sin(ofDegToRad(d_angles*j)));
                    
                    ofDrawLine(x1, y1, x2, y2);
                    
                }
                
            }
        }
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    drawSynapse();
    drawNeuron();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
