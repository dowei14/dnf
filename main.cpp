#include <math.h>
#include <iostream>
#include <iomanip> 
#include "dnf/dnf.h"

using namespace std;
#include <fstream>
#define INPUTS 8
#define SEQUENCES 1


void printVec(std::vector<double> in){
	for (unsigned int i=0; i<in.size();i++) std::cout<<in[i]<<" ";
//	std::cout<<std::endl;
}
void printVecLine(std::vector<double> in){
	for (unsigned int i=0; i<in.size();i++) std::cout<<i<<": "<<in[i]<<std::endl;
}


int getState(std::vector<double> inputVec){
	if (inputVec.size() != 90) {
		std::cout<<"--- Wrong Size ---"<<std::endl;
		return -1;
	}
	double max = -9999;
    int maxID = 0;
	std::cout<<" Sum: ";
    for (int i=0; i<8;i++){    
        int start = 5+i*10;
        int stop =5+(i+1)*10+1;
		double currentSum = 0.0;
        for (int s=start;s<stop;s++) currentSum += inputVec[s];
        std::cout<<std::setprecision(0)<<std::fixed<<currentSum<<" ";		
        if (currentSum > max){
            max = currentSum;
            maxID = i;
		}
	}
    return maxID;
}

std::vector<std::vector<double> > loadInput(std::string filename){
	// load file and store all weights(lines with . in it) to float vector
	std::ifstream infile(filename.c_str());
	std::vector<std::vector<double> > inputs;
	float in = 0.0;
	for (int i=0;i<SEQUENCES;i++) {
		std::vector<double> tmp;
		for (int j=0;j<INPUTS;j++) {
			infile >> in;
			tmp.push_back((double)in*50.0);
		}
		inputs.push_back(tmp);
	}
	return inputs;
}


int main(int argc, const char *argv[]){
	cout<<"--- start ---"<<endl<<endl<<endl;

	DNF* dnf;
	dnf = new DNF();
	int size = 90;
	double tau = 5.0;
	double h = -5.0;
	double beta = 4.0;
	dnf->setup(size,tau,h,beta);

	double sigmaExc 		= 5.0;
    double amplitudeExc 	= 50.0;
    double sigmaInh 		= 12.5;
    double amplitudeInh 	= 50.0;
    double amplitudeGlobal 	= 0.0;
	double cutoffFactor		= 5.0;
	dnf->setupLateral(sigmaExc,amplitudeExc,sigmaInh,amplitudeInh,amplitudeGlobal,cutoffFactor);

	int numStims = 8;
	double sigma_input = 10;
	vector<double> amps;
	for (int i=0;i<numStims;i++) {
		dnf->addStim((i+1)*10,sigma_input);
		amps.push_back(0.0);
	}

	std::string inputFilename = "inputs";	
	std::vector<std::vector<double> > inputs = loadInput(inputFilename);
	for (int i=0;i<SEQUENCES;i++){
		std::cout<<"Input: ";printVec(inputs[i]);
		dnf->setAmplitudes(inputs[i]);
		//for (unsigned int j=0; j<inputs[i].size();j++) std::cout<<inputs[i][j]<<" ";
		dnf->step();
		//for (unsigned int j=0; j<dnf->getOutput().size();j++) std::cout<<std::setprecision(10)<<std::fixed<<dnf->getOutput()[j]*10000<<" ";
		std::cout<<"Class: "<<getState(dnf->getOutput())<<std::endl;  
	}  

	cout<<endl<<endl<<endl<<"--- done ---"<<endl;
}
