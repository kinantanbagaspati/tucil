#include<bits/stdc++.h>
using namespace std;

const long long NMAX = 200000;
map <string, long long> courseCodetoID; //untuk setiap (x,y) dalam map, berarti string x diberi ID y
string IDtoCourseCode[NMAX]; //IDtoCourseCode[i] menyimpan CourseCode dengan id i

ifstream input;
string line, toInsert;

void mapping(string filename){ //prosedur memetakan setiap string yang muncul dalam file
    filename = "../test/" + filename;
    input.open(filename.c_str());
    long long currentID = 0;
    while(getline(input, line)){
        //Mengambil kode kuliah pertama dalam tiap baris dan memasukkan ke dalam map
        toInsert = "";
        for(int i=0; line[i]!='.' && line[i]!=','; i++){
            toInsert += " ";
            toInsert[i] = line[i];
        }
        courseCodetoID.insert({toInsert, currentID});
        IDtoCourseCode[currentID] = toInsert;
        currentID++;
    }
    input.close();
}

vector<long long> preqOf[NMAX]; //matkul dengan ID x ialah prasyarat dari matkul dengan ID preqOf[x][i]
long long nbPreq[NMAX]; //banyak prasyarat untuk matkul x ialah nbPreq[x]
long long courseID, preqID;

long long queueCourse[NMAX], head, tail, tempTail;
//queue dengan implementasi array, 
//saya tidak menggunakan std::queue karena ingin menyimpan index head dan tail yang nantinya digunakan untuk tempTail 

void setPreqOf(string filename){
//prosedur menyimpan setiap data prasyarat ke dalam array of vector preqOf sesuai definisi di atas
//prosedur ini juga mengisi array nbPreq dengan banyaknya prasyarat tiap ID mata kuliah terkait, 
//serta mengisi queue dengan ID mata kuliah yang sudah bisa diambil (nbPreq[ID] == 0)
    filename = "../test/" + filename;
    input.open(filename.c_str());
    long long itr;
    //menginisialisasi queue
    head = 0;
    tail = 0;
    courseID = 0;
    while(getline(input, line)){
        nbPreq[courseID] =0;
        itr = IDtoCourseCode[courseID].length();
        while(line[itr] != '.'){
            itr+=2;
            //Menentukan preqID menggunakan map yang sudah ada
            toInsert = "";
            for(int i=itr; line[i]!='.' && line[i]!=','; i++){
                toInsert += " ";
                toInsert[toInsert.length()-1] = line[i];
                itr++;
            }
            preqID = courseCodetoID[toInsert];
            //Sesuai definisi preqOf, courseID lah yang dimasukkan dalam vector preqOf[preqID]
            preqOf[preqID].push_back(courseID);
            nbPreq[courseID]++;
        }
        //Memasukkan ke queue jika mata kuliah dapat langsung diambil
        if(nbPreq[courseID] == 0){
            queueCourse[tail] = courseID;
            tail++;
        }
        courseID++;
    }
    input.close();
}

string toRoman(int number)
{
    int num[13] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
    string sym[13] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};
    int i=12;
    string result = "";
    while(number>0){
        int div = number/num[i];
        number = number%num[i];
        while(div--){
            result += sym[i];
        }
        i--;
    }
    return result;
}

ofstream output;
void solve(long long semester){
    if(head<tail){
        output << "Semester " << toRoman(semester) << ":";
        tempTail = tail;
        bool first = true;
        //setiap matakuliah yang ada di queue saat ini juga bisa diambil untuk semester ini
        while(head < tempTail){
            if(first){
                output << " " << IDtoCourseCode[queueCourse[head]];
                first = false;
            }else{
                output << ", " << IDtoCourseCode[queueCourse[head]];
            }
            //setiap matakuliah yang diambil akan mengurangi jumlah prasyarat mata kuliah lain
            //yang memiliki matakuliah ini sebagai prasyaratnya, oleh karena itu array of vector
            //preqOf cocok untuk melakukan hal tersebut
            for(int i=0; i<preqOf[queueCourse[head]].size(); i++){
                preqID = preqOf[queueCourse[head]][i];
                nbPreq[preqID]--;
                //bila prasyarat habis, masukkan ke queue
                if(nbPreq[preqID] == 0){
                    queueCourse[tail] = preqID;
                    tail++;
                }
            }
            head++;
        }
        output << endl;
        //lakukan rekursi untuk semester berikutnya
        solve(semester+1);
    }else{
        output.close();
    }
}

clock_t start, endtime;
int main(){
    
    string filename;
    cout << "Masukkan namafile: "; cin >> filename;
    
    start = clock();
    mapping(filename);
    setPreqOf(filename);
    output.open("../test/output.txt");
    solve(1);
    
    endtime = clock();
    double time_taken = double(endtime - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by program is : " << fixed  
        << time_taken << setprecision(5); 
    cout << " sec " << endl;
}