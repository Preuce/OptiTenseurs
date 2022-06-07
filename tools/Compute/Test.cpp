#include "Test.hpp"

void import_test(const char* test_file){

    strcat(path_test, "../calc/");
    strcat(path_test, test_file);
    //cout << path_test << '\n';
    ifstream ifile(path_test);
    if(ifile.fail()){
        cout << "Could not open test file : " << test_file << '\n';
        exit(1);
    }
    string line;
    int i;
    while(getline(ifile, line)){
        switch(line[0]){
            case 'f':
                strcpy(path_instance, "");
                strcat(path_instance, "../instances/");
                strcat(path_instance, &line[2]); //dir/instance
                import_instance(path_instance);
                break;
            case 'o':
                O.clear();
                istringstream order(&line[2]);
                while(order >> i){
                    O.push_back(i);
                }
                cout << &line[2] << " : " << follow_order(O) << '\n';

                break;
        }        
    }
}

void display(char* test_file){

}

void import_instance(const char* file){
    G.clear();
    E.clear();

    ifstream ifile(file);
    if(ifile.fail()){
        cout << "Could not open instance file : " << file << endl;
        exit(1);
    }else{
        cout << file << '\n';
    }
    string line;
    int i, j, w;
    while(getline(ifile, line)){
        istringstream l(&line[2]);
        if(line[0] == 'v'){

            string red("\033[0;31m");
            string reset("\033[0m");
            for(auto& c : line){
                switch(c){
                    case '*':
                        cout << red;
                        break;
                    case ' ':
                        cout << reset << c;
                        break;
                    case '\n':
                        cout << reset << c;
                        break;
                    case '\0':
                        cout << reset << c;
                        break;
                    case 'v':
                        cout << reset;
                        break;
                    default:
                        cout << c;
                        break;
                }
            }
            cout << '\n';
        }
        if(line[0] == 'p'){
            //istringstream taille(&line [2]);
            l >> size;

            G.resize(size*size, 1);
            for(int i = 0; i < size; i++){
                G[size*i + i] = 0;
            }
        }
        if(line[0] == 'e'){
            //istringstream edge(&line[2]);
            l >> i >> j >> w;

            G[size*i + j] = w;
            G[size*j + i] = w;
            E.push_back(make_pair(i, j));
        }
    }
    sort_edges(E);
}

void Graph::init(){
    V.resize(size, -1);
    G = ::G;
}

Cost follow_order(Tab O){
    Graph graph;
    graph.init();
    Cost cost = 0;
    if(E.size() > 3*size/2 - 2){
        cout << "[WARNING] Too many edges given, skipping order" << '\n';
    }else{
        for(int i : O){
            cost += graph.contract(i);
        }
    }
    return cost;
}

int Graph::C(int i){
    while(V[i] != -1){
        i = V[i];
    }
    return i;
}

Cost Graph::contract(int i){
    int a = C(E[i].first);
    int b = C(E[i].second);

    if(a != b){
        int res = G[size*a + b];
        for(int j = 0; j < size; j++){
            if(a != j){
                res *= max(1, G[size*b + j]);
            }
            if(b != j){ 
                res *= max(1, G[size*a + j]);
            }
        }

        for(int j = 0; j < size; j++){
            G[size*a + j] *= G[size*b + j];
            G[size*b + j] = 0;
            G[size*j + b] = 0;
            G[size*j + a] = G[size*a + j];
        }
        V[b] = a;
        return res;
    }else{
        return 0;
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        import_test("test.txt");
    }else{
        import_test(argv[1]);
    }
}