#ifndef MAIN_HPP
#define MAIN_HPP
#include "Matrice/MatrixSolver.hpp"
#include "VerticalSplit/VerticalSplitSolver.hpp"
#include "SideEx/SideExSolver.hpp"
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

ofstream result_file("../results/results.txt");
ofstream cost_file("../results/cost.csv");
ofstream time_file("../results/time.csv");
string separator(" ");
string instance_dir;
string instance_file;
std::chrono::seconds WAIT_TIME = 1800s;

MatrixSolver matrix;
Split split;
VSplit vsplit;
ESplit esplit;
SideEx sideex;
SimpleG simpleg;
TriScore triscore;
TriScoreM mts;
NTS nts;
vector<Algorithm*> algos;

void init_algos();
void add_separator();

template<class T>
void export_header(T& solver);

template<class T>
void export_results(T& solver);

template<class T>
void launch_exec(T& solver, int delta = -1);

void init_csv();

void get_size();

void export_display();

void export_order(Tab O);

void init_files();

void execfile_on_all();

#endif