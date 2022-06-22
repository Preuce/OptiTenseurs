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
#include <sys/types.h>
#include <sys/wait.h>

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
string separator(" ");
string instance_dir;
string instance_file;

void add_separator(){
    cost_file << separator << flush;
    time_file << separator << flush;
}

template<class T>
void export_header(T& solver){
    cout << "--- "<< typeid(solver).name() <<" ---" << '\n';
    result_file << "--- " << typeid(solver).name() << " ---" << '\n';
    if(solver.delta != -1){
        result_file << "Delta : " << solver.delta << '\n'; 
    }
}

template<class T>
void export_results(T& solver){
    result_file << "Best cost : " << solver.bestCost << '\n';
    result_file << "Time : " << solver.time.count() << endl;

    cost_file << solver.bestCost;
    time_file << solver.time.count();
    add_separator();
}

template<class T>
void launch_exec(T& solver, int delta = -1){
    int* status;
    pid_t pid = fork();
    if(pid == 0){ //processus fils
        condition_variable cv;
        mutex mtx;

        std::thread t1([&solver, &cv, &delta](){
            solver.refdelta = delta;
            export_header<T>(solver);
            //Template défini dans components, permettant d'initialiser l'algorithme et de résoudre l'instance
            execfile<T>(solver, instance_file);
            
            cv.notify_all();
        });

        pthread_t tid = t1.native_handle();
        t1.detach();

        //un mutex lock le thread "principal" tant que l'autre n'a pas fini (ou jusqu'à la fin du délai)
        std::unique_lock<std::mutex> lock(mtx);
        auto status = cv.wait_for(lock, 1s);

        // si timeout il y a, on tue le thread
        if (status == std::cv_status::timeout) {
            pthread_cancel(tid);
            solver.bestCost = -1;
        }
        export_results<T>(solver);
        exit(-1);

    }else{ //processus parent
        wait(status);
    }
}

void init_csv(){
    /*size, Split, VSplit, EdgeSplit, SideIter, SideLim, SimpleG, TriScore*/
    string header = "size" + separator + 
    "Matrix" + separator + 
    "Split" + separator + 
    "VSplit" + separator + 
    "EdgeSplit" + separator + 
    "SideIter" + separator + 
    "SideLim" + separator + 
    "SimpleG" + separator +
    "TriScore" + separator +
    "TriScoreMargin" + separator +
    "TriScoreNaive"
    + '\n';
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
                cost_file << &line[2];
                time_file << &line[2];
                add_separator();
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

void init(){
    result_file << instance_file << '\n';
    display(instance_file);
    export_display();
    get_size();
}

void execfile_on_all(){
    init();
    
    launch_exec(matrix);
    launch_exec(split);
    launch_exec(splitrange, 3);
    launch_exec(esplit);
    launch_exec(sideiter);
    launch_exec(sidelim, 3);
    launch_exec(simpleg);
    launch_exec(triscore);
    launch_exec(mts, 10);
    launch_exec(nts);

    cost_file << endl;
    time_file << endl;
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
            execfile_on_all();
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
            execfile_on_all();
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