#include "SimpleG.hpp"

Cost SimpleG::solve(SouG sg){
    Cost cost = 0;
    if(sg.S.size() > 0){
        for(int i = 0; i < sg.S.size(); i++){
            SouG sg2 = sg;
            sg2.S.erase(sg2.S.begin() + i);
            int c = contract(sg.S[i], sg2);
            c += solve(sg2);

            if((c < cost || cost == 0) && c > 0){
                cost = c;
                O[E.size()-sg.S.size()] = sg.S[i];
            }
        }
    }
    return cost;
}

Cost SimpleG::contract(int i, SouG& sg){
    int a = sg.C(E[i].first);
    int b = sg.C(E[i].second);

    if(a != b){
        int res = sg.G[size*a + b];
        for(int j = 0; j < size; j++){
            if(a != j){
                res *= max(1, sg.G[size*b + j]);
            }
            if(b != j){ 
                res *= max(1, sg.G[size*a + j]);
            }
        }

        for(int j = 0; j < size; j++){
            sg.G[size*a + j] *= sg.G[size*b + j];
            sg.G[size*b + j] = 0;
            sg.G[size*j + b] = 0;
            sg.G[size*j + a] = sg.G[size*a + j];
        }
        sg.V[b] = a;
        return res;
    }else{
        return 0;
    }
}

int SouG::C(int i){
    while(V[i] != -1){
        i = V[i];
    }
    return i;
}

void SimpleG::display_order(){
    for(int i = O.size()-1; i >= 0; i--){
        cout << O[i] << " - ";
    }
    cout << '\n';
}

void SimpleG::get_size(char* Preamble){
	char c;
	char * pp = Preamble;
	int stop = 0;
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

void SimpleG::init(const char* file){
    //hardcoder les poids voisinant les sommets dans un fichier texte
    G.clear();
    E.clear();
    O.clear();

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

    G.resize(size*size, 1);
    S.resize(3*size/2 - 2);
    O.resize(3*size/2 - 2, -1);
    
	while ((c = fgetc(fp)) != EOF){
		switch (c){
            case 'e':
                if (!fscanf(fp, "%d %d %d", &i, &j, &w)){ 
                    printf("ERROR: corrupted inputfile\n"); 
                    exit(10);
                }
                E.push_back(make_pair(i, j));
                G[size*i + j] = w;
                G[size*j + i] = w;
                break;
                
            case '\n':
                
            default:
                break;
        }
	}

    for(int i = 0; i < size; i ++){
        G[size*i + i] = 0;
    }

    sort_edges(E);

    for(int i = 0; i < E.size(); i++){
        S[i] = i;
    }

    fclose(fp);
	delete[] Preamble;
}

void SimpleG::execfile(const char* file){
    char path[100] = "../instances/";
    strcat(path, file);
    //cout << "Starting initialisation on : " << file << endl;
    init(path);
    //cout << "End of initialisation" << endl;
    //cout << "Starting solving" << endl;
    
    /*auto start = std::chrono::high_resolution_clock::now();
    int c = solve(getSG());
    cout << "Best cost : " << c << endl;
    cout << "Best order : ";
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempsSeq = end-start;
    display_order();
    std::cout << std::scientific << "Temps : " << tempsSeq.count()<< "s" << std::endl;
    */
    /*SouG sg = getSG();
    sg.S = {0, 5, 4, 6, 1, 2, 3};
    Tab A (sg.S.size());
    solveAlt(sg, A);
    for(int i : A){
        cout << i << '\n';
    }*/
    cout << solve(getSG()) << '\n';
    display_order();
    /*for(int i : O){
        cout << i << '\n';
    }*/
    cout << "--------------" << endl;
}

void SimpleG::solveAlt(SouG sg, Tab& A){
    if(sg.S.size() > 0){
        SouG sg2 = sg;
        sg2.S.erase(sg2.S.begin());
        solveAlt(sg2, A);
        A[E.size() - sg.S.size()] = sg.S[0];
    }
}

void SimpleG::execdir(const char* dir){
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