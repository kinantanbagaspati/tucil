#include<bits/stdc++.h>
using namespace std;
//Untuk generate file Directed Acyclic Graph 
//yang ditulis dalam format sesuai input dari Tucil 2 STIMA

const long long NMAX = 100000;
long long pseudoRandomizer;
string majorCode;
long long density; //(peluang sebuah pasangan node connected)*NMAX
long long nbCourse; //(nbCourse**2) * density < 1e12 agar banyaknya edge kurang dari 1e6
long long maxSemester; 
//solusi dari generator ini dapat diselesaikan dengan maksimal (maxSemester) semester saja
//Asumsikan variabel diatas kurang dari 99
long long partitions[1000];
bool included[NMAX];
long long prob;

vector <long long> preqs[NMAX];
ofstream output;

long long randomize(long long i){
    return (pseudoRandomizer*i)%NMAX;
}

string toString (long long x, long long digits){
    string res = "";
    for(int i=0; i<digits; i++){
        res += "0";
    }
    while(x>0){
        digits--;
        res[digits] += x%10;
        x/=10;
    }
    return res;
}
string toString (long long x){
    string result = "";
    while(x>0){
        result = "0"+result;
        result[0] += x%10;
        x/=10;
    }
    return result;
}
void write(){
    string filename = "../test/input"+toString(nbCourse)+".txt";
    output.open (filename.c_str());
    for(int i=0; i<NMAX; i++){
        if(included[i]){
            output << majorCode << toString(i, 5);
            for(int j=0; j<preqs[i].size(); j++){
                output << ", " << majorCode << toString(preqs[i][j], 5);
            }
            output << ".\n";
        }
    }
    output.close();
}

int main(){
    //inisialisasi randomizer dan input
    srand (time(NULL));
    cout << "Masukkan kode jurusan: "; cin >> majorCode;
    cout << "Masukkan density(<=100000): "; cin >> density;
    cout << "Masukkan nbCourse: "; cin >> nbCourse;
    cout << "Masukkan maxSemester (<99): "; cin >> maxSemester;
    cout << "Masukkan pseudorRandomizer(relatif prima dengan 10): "; cin >> pseudoRandomizer;
    for(int i=0; i<NMAX; i++){
        included[i] = false;
    }
    //Randomize barisan yang menjadi partisi
    partitions[0] = 1;
    for(int i=1; i<maxSemester; i++){
        partitions[i] = rand() % nbCourse;
        partitions[i]++;
    }
    partitions[maxSemester] = nbCourse+1;
    sort(partitions, partitions+maxSemester+1);
    //setiap edge yang mungkin untuk ada 
    //(dari node pada partisi i ke node pada partisi j dengan i<j) 
    //akan ditambahkan dengan probabilitas sebesar (density/100000)
    for(int i=0; i<maxSemester; i++){
        for(int j=partitions[i]; j<partitions[i+1]; j++){
            included[randomize(j)] = true;
            for(int k=partitions[0]; k<partitions[i]; k++){
                prob = rand()%NMAX;
                if(prob < density){
                    preqs[randomize(j)].push_back(randomize(k));
                }
            }
        }
    }
    write();
}