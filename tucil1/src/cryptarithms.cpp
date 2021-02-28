#include<bits/stdc++.h>
using namespace std;

const int mxOperand = 100000;
string operand[mxOperand];

int main () {
    clock_t start, end;
    string line, add = " ";
    cout << "Pastikan untuk menginput test yang benar pada file input.txt dalam folder test" << endl;
    ifstream input ("../test/input.txt");
    if (input.is_open()){
        //Membaca file dan menyimpan semua operand
        int nbOperand = 0, len = 0;
        bool isOperand = true;
        while(getline(input, line)){
            if(isOperand){
                if(line[0] == '-'){
                    isOperand = false;
                }else{
                    operand[nbOperand] = "";
                    for(int i=0; i<line.length(); i++){
                        if(line[i]!=' ' && line[i]!='+'){
                            add[0] = line[i];
                            operand[nbOperand] += add;
                        }
                    }
                    len = max(len, (int) operand[nbOperand].length());
                    nbOperand++;
                }
            }else{
                operand[nbOperand] = "";
                for(int i=0; i<line.length(); i++){
                    if(line[i]!=' '){
                        add[0] = line[i];
                        operand[nbOperand] += add;
                    }
                }
                len = max(len, (int) operand[nbOperand].length());
            }
        }

        //Prekomputasi tiap karakter disimpan dalam letters,
        //karakter pertama atau tidak disimpan dalam isFirst,
        //pengali tiap karakter dalam multiplier, 
        //serta lokasi penyimpanan (idx) tiap karakter dalam letters disimpan dalam loc sesuai ASCII
        start = clock();
        char letters[10];
        bool isFirst[10], found;
        int nbLetters = 0, curOperand = 0, slot, multiplier[11], mul, loc[256];
        for(int i=0; i<10; i++){
            isFirst[i] = false;
            multiplier[i] = 0;
        }
        while(nbLetters <= 10 && curOperand<=nbOperand){
            int mul = 1;
            for(int i=operand[curOperand].length()-1; i>0 && nbLetters<=10; i--){
                found = false;
                slot = 0;
                while(!found && slot<nbLetters){
                    if(letters[slot] == operand[curOperand][i]){
                        found = true;
                    }else{
                        slot++;
                    }
                }
                if(found){
                    multiplier[slot] += mul;
                }else{
                    if(slot<10){
                        letters[slot] = operand[curOperand][i];
                        loc[operand[curOperand][i]] = slot;
                        multiplier[slot] += mul;
                    }
                    nbLetters++;
                }
                mul*=10;
            }
            found = false;
            slot = 0;
            while(!found && slot<nbLetters){
                if(letters[slot] == operand[curOperand][0]){
                    found = true;
                }else{
                    slot++;
                }
            }
            if(found){
                multiplier[slot] += mul;
                isFirst[slot] = true;
            }else{
                if(slot<10){
                    letters[slot] = operand[curOperand][0];
                    loc[operand[curOperand][0]] = slot;
                    multiplier[slot] += mul;
                    isFirst[slot] = true;
                }
                nbLetters++;
            }
            mul*=10;
            curOperand++;
        }

        //Penulisan persamaan kembali dalam terminal secara rata kanan
        for(int i=0; i<nbOperand; i++){
            cout << endl;
            int curlen = operand[i].length();
            for(int j=0; j<len - curlen; j++){
                cout << " ";
            }
            cout << operand[i];
        }
        cout << "+" << endl;
        for(int i=0; i<=len; i++){
            cout << "-";
        }
        cout << endl;
        for(int i=0; i<len - operand[nbOperand].length(); i++){
            cout << " ";
        }
        cout << operand[nbOperand] << endl;

        if(nbLetters>10){
            cout << endl << "More than 10 letters included, hence no solution" << endl;
        }else{
            //Inisialisasi permutasi, nilai-nilai faktorial, banyak solusi yang diperoleh
            int permutation[10], used[10], factorial[11], sum, result, value[256], curlen, solutions = 0;
            bool violate, useCheck;
            factorial[0] = 1;
            for(int i=1; i<=10; i++){
                factorial[i] = factorial[i-1]*i;
                used[i-1] = false;
            }
            int skip = factorial[10-nbLetters], counter, countUse, currentUse, comparations = 0;
            for(int brute=0; brute < factorial[10]; brute += skip){
                //Blok kode dibawah men-generate permutasi ke-brute terkecil (leksikografis)
                useCheck = used[0];
                counter = brute;
                for(int i=9; i>=0; i--){
                    countUse = counter/factorial[i];
                    currentUse = 0;
                    while((used[currentUse] != useCheck) || countUse>0){
                        if(used[currentUse] == useCheck){
                            countUse--;
                        }
                        currentUse++;
                    }
                    used[currentUse] = !useCheck;
                    permutation[9-i] = currentUse;
                    counter %= factorial[i];
                }
                //sum dan result akan dibandingkan, sambil melihat pelanggaran huruf pertama
                sum = 0;
                violate = false;
                for(int i=0; i<nbLetters; i++){
                    sum += permutation[i]*multiplier[i];
                    value[letters[i]] = permutation[i];
                    if(permutation[i] == 0 && isFirst[i]){
                        violate = true;
                    }
                }
                result = 0;
                for(int i=0; i<operand[nbOperand].length(); i++){
                    result *=10;
                    result += value[operand[nbOperand][i]];
                }
                if(sum == 2*result && !violate){
                    //Print solusi
                    for(int i=0; i<nbOperand; i++){
                        cout << endl;
                        curlen = operand[i].length();
                        for(int j=0; j<len - curlen; j++){
                            cout << " ";
                        }
                        for(int j=0; j<curlen; j++){
                            cout << value[operand[i][j]];
                        }
                    }
                    cout << "+" << endl;
                    for(int i=0; i<=len; i++){
                        cout << "-";
                    }
                    cout << endl;
                    curlen = operand[nbOperand].length();
                    for(int i=0; i<len - curlen; i++){
                        cout << " ";
                    }
                    for(int i=0; i<curlen; i++){
                        cout << value[operand[nbOperand][i]];
                    }
                    cout << endl;
                    solutions++;
                }
                if(!violate){
                    comparations++;
                }
            }
            cout << endl << "Total: " << solutions << " solution(s) acquired" << endl;
            cout << "Total comparations: " << comparations << endl;
        }
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        cout << "Time taken by program is : " << fixed  
            << time_taken << setprecision(5); 
        cout << " sec " << endl;
        input.close();
    }else{
        cout << "Unable to open file";
    }
    return 0;
}