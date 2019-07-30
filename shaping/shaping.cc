#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

int main(void){
ifstream input_file("../output/result.csv");
vector<string> file;
string tmp;
int i = 0;
if(input_file.fail()){
    cout << "error:input file can not open\n";
    return 0;
}
while(getline(input_file,tmp)){
    file.push_back(tmp);
    i++;
}
int key = 0, key_flag = 0;
for(i = 0;i < file.size();i++){
    if(file[i] == "" && key_flag == 0){
        key = i;
        key_flag = 1;
    }
}
//cout << key << "\n";
//cout << file.size() << "\n";
key = key * 4;
key += 3;
ofstream outputfile("../output/result_shaping.csv");
for(i = 0;i < key+1;i++){
    for(int j = i;j < file.size();j += key+1){
        outputfile << file[j] << ",,";
    }
    outputfile << "\n";
}

return 0;

}