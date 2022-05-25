#include "MatrixSolver.hpp"

/**
 * @brief generates all permutations recursively
 * 
 * @param n the depth (amount of remaining tensors)
 * @param cost the current cost
 * @param v the current order
 * @param N the current network
 */
void MatrixSolver::solve(int n, Cost cost, vector<pair<int, int>> v, Network N){
    for(int j = 1; j < n; j++){
        for(int i = 0; i < j; i++){
            vector<pair<int, int>> v2 = v;
            v2.push_back(make_pair(i, j));
            solve(n-1, cost + contractionCost(i, j, N), v2, contract(i, j, N));
        }
    }
    if(n <= 1 && cost < bestCost){
        bestCost = cost;
        bestOrder = v;

        /*cout << "New best cost : " << cost << endl;
        display_best_order();
        cout << endl;
        cout << "--------------" << endl;*/
    }
}

/**
 * @brief contracts an edge in the matrix N and returns the newly formed matrix
 * 
 * @param i 
 * @param j 
 * @param N
 * @return Network
 */
Network contract(int j, int i, Network N){
    for(unsigned int k = 0; k < N.size(); k++){
        //migration des poids vers la j-ième ligne/colonne
        N[k][j] *= N[k][i]; //TODO: trouver une alternative à la matrice symétrique
        N[j][k] *= N[k][i];
    }
    for(unsigned int k = 0; k < N.size(); k++){
        N[k].erase(N[k].begin() + i);
    }
    //suppression de la i-ième colonne
    N.erase(N.begin() + i);
    return N;
}

/**
 * @brief calculates the contractions cost of the edge on (i, j) in the matrix N
 * 
 * @param i 
 * @param j 
 * @param N
 * @return int 
 */
Cost contractionCost(int i, int j, Network N){
    int cost = N[i][j];
    for(unsigned int k = 0; k < N.size(); k++){
        cost *= max(1, N[k][j]*N[k][i]);
    }
    return cost;
}

/**
 * @brief Displays a matrix
 * 
 * @param N 
 */
void display(Network N){
    for(unsigned int i = 0; i < N.size(); i++){
        for(unsigned int j = 0; j < N.size(); j++){
            cout << N[j][i] << " | ";
        }
        cout << endl;
    }
}


void MatrixSolver::get_size(char* Preamble){

	char c;
	char * pp = Preamble;
	int stop = 0;
	//tmp = (char *)calloc(100, sizeof(char));
	int nbT;
	
	while (!stop && (c = *pp++) != '\0'){
		switch (c){
            case 'c':
                while ((c = *pp++) != '\n' && c != '\0');
                break;
                
            case 'p':
                sscanf(pp, "%d\n", &nbT);
                stop = 1;
                break;
                
            default:
                break;
        }
	}
	size = nbT;
}

/**
 * @brief initializes the network based on a textfile
 * 
 * @param path a path to a textfile
 */
void MatrixSolver::init(const char* file){
    bestOrder.clear();
    bestCost = INT32_MAX;
    for(int i = 0; i < network.size(); i ++){
        network[i].clear();
    }
    network.clear();
    //hardcoder les poids voisinant les sommets dans un fichier texte
    int MAX_PREAMBLE = 4000;
	char* Preamble = new char [MAX_PREAMBLE];

    int c, oc;
	char* pp = Preamble;

    int i, j, w;
	FILE* fp;

    if((fp=fopen(file,"r"))==NULL ){ 
        printf("ERROR: Cannot open infile\n"); 
        exit(10); 
    }

    for(oc = '\0';(c = fgetc(fp)) != EOF && (oc != '\n' || c != 'e'); 
    oc = *pp++ = c);

    ungetc(c, fp); 
	*pp = '\0';

    get_size(Preamble);

    network.resize(size);

    for(int k = 0; k < size; k ++){
        network[k].resize(size, 1);
        network[k][k] = 0;
    }
	
	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                network[i][j] = w;
                network[j][i] = w;
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    fclose(fp);
	delete[] Preamble;

    bestOrder.resize(size);
}

/**
 * @brief displays bestOrder
 * 
 */
void MatrixSolver::display_best_order(){
    for(int k = 0; k < bestOrder.size(); k++){
        cout << "(" << bestOrder[k].first << ", " << bestOrder[k].second << ")||";
    }
}

void MatrixSolver::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    auto start = std::chrono::high_resolution_clock::now();
    solve(size, 0, {}, network); //TODO:

    cout << "Best cost : " << bestCost << endl;

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> tempsSeq = end-start;
    cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    cout << "-------------" << endl;
}

void MatrixSolver::execdir(const char* dir){
    char base[100] = "../instances/";
    strcat(base, dir);
    strcat(base, "/");
    DIR* dp = NULL;
    struct dirent *file = NULL;
    dp = opendir(base);
    
    file = readdir(dp);
    
    while(file != NULL){
        if(file->d_name[0] != '.'){
            char path[100];
            strcpy(path, base);
            strcat(path, file->d_name);
            cout << "FILE : " << path << endl;
            execfile(path);
        }
        file = readdir(dp);
    }
    closedir(dp);
}