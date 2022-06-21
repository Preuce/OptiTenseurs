#include "Matrice/MatrixSolver.hpp"
#include "VerticalSplit/VerticalSplitSolver.hpp"
#include "SideIter/SideExSolver.hpp"
#include "SideLimited/SideLimSolver.hpp"
#include "Split/SplitSolver.hpp"
#include "EdgeSplit/EdgeSplit.hpp"
#include "TriScore/TriScore.hpp"
#include "TriScoreMargin/TriScoreM.hpp"
#include "TriScoreNaive/TriScoreN.hpp"
#include "SimpleGreedy/SimpleG.hpp"
#include <signal.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <condition_variable>

using namespace chrono_literals;

MatrixSolver matrix;
SplitRange splitrange;
SimpleG simpleg;
SideIter sideiter;
SLim sidelim;
Split split;
ESplit esplit;
TriScore triscore;
TriScoreM mts;
NTS nts;

ofstream result_file("../results/results.txt");
ofstream cost_file("../results/cost.csv");
ofstream time_file("../results/time.csv");
string instance_dir;
string instance_file;

template<typename T>
void execfile(T& solver)
{
    condition_variable cv;
    mutex mtx;

    /*pid_t idt;
    bool wait = true;
    int loop = 0;*/

    std::thread t1([/*&idt, &wait, */&solver, &cv](){
        //idt = gettid();
        solver.execfile(instance_file);
        cv.notify_all();
        //wait = false;
    });
    pthread_t tid = t1.native_handle();
    t1.detach();
    /*while(wait && loop < 40){
        this_thread::sleep_for(100ms);
        loop++;
    }
    kill(idt, SIGCONT);*/
    std::unique_lock<std::mutex> lock(mtx);
    auto status = cv.wait_for(lock, 1s);

    // si timeout il y a, on tue le thread
    if (status == std::cv_status::timeout) {
        pthread_cancel(tid);
    }
}

void init_csv(){
    /*size, Split, VSplit, EdgeSplit, SideIter, SideLim, SimpleG, TriScore*/
    string header = "size, Matrix, Split, VSplit, EdgeSplit, SideIter, SideLim, SimpleG, TriScore \n";
    cost_file << header;
    time_file << header;
}

void get_size(){
    string path = "../instances/" + instance_file;
    string filename(path);
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        exit(-1);
    }

     while(getline(input_file, line)){
        if(line.size() > 2){
            if(line[0] == 'p'){
                cost_file << &line[2] << ", ";
                time_file << &line[2] << ", ";
                break;
            }
        }
     }

}

void export_display(){
    string path = "../instances/" + instance_file;
    string filename(path);
    string line;
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        exit(-1);
    }

    char mot[200];
    int n = 0;
    int nL = 0;
    while(getline(input_file, line) && line[0] != 'e'){
        if(line.size() > 2){
            istringstream flux(&line[2]);
            switch(line[0]){
                case 'v':
                   while(flux >> mot){
                        if(mot[0] == '*'){
                            char* padding;
                            if(nL == 1){
                                padding = "       ";
                            }else{
                                padding = "   ";
                            }
                            result_file << mot << "(" << n << ")" << padding;
                            n++;
                        }else{
                            result_file << mot << "   ";
                        }
                    }
                    nL++;
                    result_file << '\n';
                break;
                case 'p':
                    result_file << '\n' << "Sommets : " << &line[2] << '\n';
                break;
                default:
                break;
            }
        }
    }
    result_file << '\n';
}

void export_order(Tab O){
    result_file << "Best order : ";
    for(int i : O){
        result_file << i << " ";
    }
    result_file << '\n';
}

void export_matrix(){
    cout << "--- Matrix ---" << '\n';
    result_file << "--- Matrix ---" << '\n';
    execfile(matrix);
    result_file << "Best cost* : " << matrix.bestCost << '\n'; 
    result_file << "Time : " << matrix.time.count() <<endl;

    cost_file << matrix.bestCost << ", ";
    time_file << matrix.time.count() << ", ";
}

void export_split(){
    cout << "--- Split ---" << '\n'; 
    result_file << "--- Split ---" << '\n';
    execfile(split);
    result_file << "Best cost* : " << split.bestCost << '\n';
    result_file << "Time : " << split.time.count() << endl;

    cost_file << split.bestCost << ", ";
    time_file << split.time.count() << ", ";
}

void export_esplit(){
    cout << "--- EdgeSplit ---" << '\n'; 
    result_file << "--- EdgeSplit ---" << '\n';
    execfile(esplit);
    result_file << "Best cost* : " << esplit.bestCost << '\n';
    result_file << "Time : " << esplit.time.count() << endl;

    cost_file << esplit.bestCost << ", ";
    time_file << esplit.time.count() << ", ";
}

void export_simpleg(){
    cout << "--- SimpleGreedy ---" << '\n';
    result_file << "--- SimpleGreedy ---" << '\n';
    execfile(simpleg);
    result_file << "Best cost* : " << simpleg.bestCost << '\n';
    export_order(simpleg.bestOrder);
    result_file << "Time : " << simpleg.time.count() << endl;

    cost_file << simpleg.bestCost << ", ";
    time_file << simpleg.time.count() << ", ";
}

void export_vsplit(){
    cout << "--- VSplit ---" << '\n';
    result_file << "--- VSplit ---" << '\n';
    splitrange.delta = 3;
    result_file << "Delta : " << splitrange.delta << '\n'; 
    execfile(splitrange);
    result_file << "Best cost : " << splitrange.bestCost << '\n';
    result_file << "Time : " << splitrange.time.count() << endl;

    cost_file << splitrange.bestCost << ", ";
    time_file << splitrange.time.count() << ", ";
}

void export_sideiter(){
    cout << "--- SideIter ---" << '\n';
    result_file << "--- SideIter ---" << '\n';
    execfile(sideiter);
    result_file << "Best cost : " << sideiter.bestCost << '\n';
    export_order(sideiter.bestOrder);
    result_file << "Time : " << sideiter.time.count() << endl;

    cost_file << sideiter.bestCost << ", ";
    time_file << sideiter.time.count() << ", ";
}

void export_sidelim(){
    cout << "--- SideLimited ---" << '\n';
    result_file << "--- SideLimited ---" << '\n';
    sidelim.delta = 3;
    result_file << "Delta : " << sidelim.delta << '\n';
    execfile(sidelim);
    result_file << "Best cost : " << sidelim.bestCost << '\n';
    export_order(sideiter.bestOrder);
    result_file << "Time : " << sidelim.time.count() << endl;

    cost_file << sidelim.bestCost << ", ";
    time_file << sidelim.time.count() << ", ";
}

void export_triscore(){
    cout << "--- TriScore ---" << '\n';
    result_file << "--- TriScore ---" << '\n';
    execfile(triscore);
    result_file << "Best cost : " << triscore.bestCost << '\n';
    export_order(triscore.bestOrder);
    result_file << "Time : " << triscore.time.count() << endl;

    cost_file << triscore.bestCost << endl;
    time_file << triscore.time.count() << endl;
}

void export_mts(){
    cout << "--- TriScoreMargin ---" << '\n';
    result_file << "--- TriScoreMargin ---" << '\n';
    mts.delta = 3;
    execfile(mts);
    result_file << "Delta : " << mts.delta << '\n'; 
    result_file << "Best cost : " << mts.bestCost << '\n';
    export_order(mts.bestOrder);
    result_file << "Time : " << mts.time.count() << endl;
}

void export_nts(){
    cout << "--- TriScoreNaive ---" << '\n';
    result_file << "--- TriScoreNaive ---" << '\n';
    execfile(nts);
    result_file << "Best cost : " << nts.bestCost << '\n';
    export_order(nts.bestOrder);
    result_file << "Time : " << nts.time.count() << endl;
}

void execfile(){
    result_file << instance_file << '\n';
    display(instance_file);
    export_display();

    get_size();

    //export_matrix(); //seems ok
    
    //export_split();

    //export_vsplit();

    //export_esplit();

    //export_sideiter();

    //export_sidelim();

    export_simpleg();

    //export_triscore();

    //export_mts();

    //export_nts();
    result_file << "----------" << "\n\n";
}

void execdir(){
    init_csv();
    string directory = "../instances/" + instance_dir + "/";
    DIR* dp = NULL;
    struct dirent *file = NULL;
    dp = opendir(directory.c_str());
    if(dp == NULL){
        cerr << "Could not open directory : " << directory << '\n';
        exit(-1);
    }
    file = readdir(dp);
    
    while(file != NULL){
        if(file->d_name[0] != '.'){
            instance_file = directory + file->d_name;
            execfile();
        }
        file = readdir(dp);
    }
    closedir(dp);
}

int main(int argc, char* argv[]){
    switch (argc){
    case 1:
        cout << "Missing argument : instance file" << '\n';
        cout << "Use 'h' to get infos" << '\n';
        return -1;
        break;
    case 2:
        if(argv[1] == string("h")){
            cout << "d directory : executes all solvers on every instance of the directory" << '\n';
            cout << "v file : displays the graph represented by this instance" << '\n';
            cout << "vd directory : displays all instances in the directory" << '\n';
            cout << "filename : execute all the algorithms on this instance" << '\n';
            cout << "Best costs marked with '*' are the optimal costs calculated using greedy algorithms" << '\n';
            cout << "The root directory's name is 'instances'" << '\n';
        }else{
            instance_file = argv[1];
            execfile();
        }
        break;
    case 3:
        if(argv[1] == string("d")){
            instance_dir = argv[2];
            execdir();
        }else if(argv[1] == string("vd")){
            display_dir(argv[2]);
        }else if(argv[1] == string("v")){
            display(argv[2]);
        }

        break;
    default:
        cout << "Error : check arguments list" << endl;
        break;
    }
    return 0;
}