#include "InstanceGenerator.hpp"
#include <vector>

void initInstances(int n, int min, int max, int amount, char* file){
    char s [200];
    char* path = "../instances/";
    strcpy(s, path);
    strcat(s, file);
    strcat(s, "/");
    for(int i = 1; i <= amount; i++){
        string name = "instance_" + to_string(i) + "_" + to_string(n) + ".txt";
        ofstream file(s + name);
        if(file.is_open()){
            //c name
            //c Source
            file << "c FILE: " << name <<endl;
            file << "c SOURCE: initInstances" << endl;
            file << "c min : " << min << endl;
            file << "c max : " << max << endl;

            vector<string> display(3, "v ");
            vector<int> weights(3*(n/2) - 2);
            for(int i = 0; i < weights.size(); i++){
                weights[i] = min + rand()%(max+1 - min);
            }
        
            for(int i = 0; i < n/2-1; i++){
                display[0].append(to_string(i) + "  *" + to_string(weights[i]) + "  "); //i "  " weights[i]
                display[1].append("*" + to_string(weights[n/2-1+i]) + "     "); //weights[n/2-1 + i]
                display[2].append(to_string(i+n/2) + "  *" + to_string(weights[n-1+i]) + "  "); //i+n/2 "  " wheights[n-1+i]
            }
            display[0].append(to_string(n/2-1));
            display[1].append("*" + to_string(weights[n-2]));
            display[2].append(to_string(n-1));
        
            file << display[0] << '\n';
            file << display[1] << '\n';
            file << display[2] << '\n';

            //p carac
            file << "p " << n << endl;
            
            //e edges
            //tant qu'on a pas placé ..., on fait
            for(int i = 0; i < n/2 - 1; i++){
                file << "e " << i << " " << i+1 << " " << weights[i] << endl;
                file << "e " << i << " " << i+n/2 << " " << weights[n/2-1+i]  << endl;
                file << "e " << i+n/2 << " " << i+n/2+1 << " " << weights[n-1+i] << endl;
            }
            file << "e " << n/2 - 1 << " " << n - 1 << " " << weights[n-2] << endl;
            //for(int i = n/2; i < n - 1; i++){
            //    file << "e " << i << " " << i+1 << " " <<  << endl;
            //}
        }else{
            printf("ERROR: Cannot open outfile\n");
        }
    }
}


int main(int argc, char* argv[]){

    for(int i = 1; i < argc - 1; i++){
        switch(*argv[i]){
            case 'm':
                m = atoi(argv[i+1]);
                i++;
                break;
            case 'M':
                M = atoi(argv[i+1]);
                i++;
                break;
            case 'n':
                amount = atoi(argv[i+1]);
                i++;
                break;
            case 's':
                size = atoi(argv[i+1]);
                i++;
                break;
            case 'd':
                dir = argv[i+1];
                i++;
                break;
        }
    }
    initInstances(max(4, size), max(1, m), M, amount, dir);
    cout << "Generation complete" << endl;
}