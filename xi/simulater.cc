#include <iostream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <random>
#define br "\n";

using namespace std;



int main(int argc, char *argv[]){
	random_device rd;
	
		mt19937 mt(rd());	
		uniform_int_distribution<int> dice(1,10);
		int a[10];
		int i;
		int cnt = 0;
		for(i = 0;i < 10;i++){
			a[i] = 0;
			cout << a[i] << br;
		}
	
		for(i = 0;i < 10000;i++){
			cnt++;
	switch(dice(mt)){
		case 1 :cout << "1" << br; a[0]++; break;
		case 2 :cout << "2" << br; a[1]++; break;
		case 3 :cout << "3" << br; a[2]++; break;
		case 4 :cout << "4" << br; a[3]++; break;
		case 5 :cout << "5" << br; a[4]++; break;
		case 6 :cout << "6" << br; a[5]++; break;
		case 7 :cout << "7" << br; a[6]++; break;
		case 8 :cout << "8" << br; a[7]++; break;
		case 9 :cout << "9" << br; a[8]++; break;
		case 10 :cout << "10" << br; a[9]++; break;
		default: cout << "xi error" << br; return 0;	
	}
}
cout << "cnt =" << cnt << br;
	for(i = 0;i < 10;i++){
		cout << i+1 << ": " << a[i] << br;
	}
	return 0;
}


