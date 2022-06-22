#include "Export.hpp"

void export_results(int bestCost){
    fstream ofile("../results/cost.csv", _S_app);
    if(ofile.is_open()){
        ofile << ", " << bestCost;
    }else{
        cerr << "Could not open cost.csv" << endl;
        exit(-1);
    }
}

void export_time(double time){
    ofstream ofile("../results/time.csv", _S_app);

    if(ofile.is_open()){
        ofile << ", " << time;
    }else{
        cerr << "Could not open cost.csv" << endl;
        exit(-1);
    }
}

void end_line(){
    ofstream ofile("../results/cost.csv", _S_app);
    if(ofile.is_open()){
        ofile << '\n';
    }else{
        cerr << "Could not open cost.csv" << endl;
        exit(-1);
    }

    ofile.close();
    ofile.open("../results/time.csv", _S_app);
    if(ofile.is_open()){
        ofile << '\n';
    }else{
        cerr << "Could not open time.csv" << endl;
        exit(-1);
    }
}