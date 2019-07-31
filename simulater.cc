#include <iostream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "bench.h"

using namespace std;
#include "scenario.cc"

//CSVを見やすく整列するやつ
void allocate_scenario(string name_a,int line_num){
	string name = name_a + ".csv";
	ifstream input_file(name.c_str());
	vector<string> file;
	string tmp;
	int i = 0;
	if(input_file.fail()){
		cout << "error:input file can not open\n";
		return ;
	}
	while(getline(input_file,tmp)){
		file.push_back(tmp);
		i++;
	}
	result_t result_num;
	string separate(result_num.column_num-1,',');
	//cout << result_num.column_num << br;
	//cout << separate << br;

	int key = 0, key_flag = 0;
	for(i = 0;i < file.size();i++){
		if(file[i] == separate && key_flag == 0){
			//key = i;
			key_flag = 1;
		}
		if(key_flag == 1 && file[i] != separate){
			key = i;
			key_flag = -1;
		}
	}
	
	//int key = 20;
	//key--;
	//cout << key << "\n";
	//cout << file.size() << "\n";
	key = key * line_num;
	key--;
	//key += line_num - 1;
	name = name_a + "_shaping.csv";
	ofstream outputfile(name.c_str());
	for(i = 0;i < key+1;i++){
		for(int j = i;j < file.size();j += key+1){
			outputfile << file[j] << ",,";
		}
		outputfile << "\n";
	}
	return ;
}

vector<simulate> make_list(void){
	vector<simulate> list;
	/*
	list.push_back(pre_scenario());
	list.push_back(scenario1(0));
	list.push_back(scenario1(1));
	list.push_back(scenario2(0));
	list.push_back(scenario2(1));
	list.push_back(scenario3(0));
	list.push_back(scenario3(1));
	list.push_back(scenario1_rarity(0));
	list.push_back(scenario1_rarity(1));
	list.push_back(scenario4(0));
	list.push_back(scenario4(1));
	list.push_back(scenario5(0));
	list.push_back(scenario5(1));
	list.push_back(scenario5_eva(0));
	list.push_back(scenario5_eva(1));
	*/
	list.push_back(scenario7_a(0));
	list.push_back(scenario7_a(1));
	list.push_back(scenario7_b(0));
	list.push_back(scenario7_b(1));
	list.push_back(scenario7_c(0));
	list.push_back(scenario7_c(1));
	list.push_back(scenario7_d(0));
	list.push_back(scenario7_d(1));
	list.push_back(scenario7_e(0));
	list.push_back(scenario7_e(1));
	return list;
}

//突貫工事だから仕方なく
void print_result(vector<result_t> result){
	int num = result.size();
	int i;
	result[0].print_column();
	for(i = 0;i < num;i++){
		result[i].print_var();
	}	
}
vector<result_t> add_result(vector<result_t> all, vector<result_t> sim){
	int num = all.size();
	int i;
	for(i = 0;i < num;i++){
		all[i].deadline_rest += sim[i].deadline_rest;
		all[i].Wh_h += sim[i].Wh_h;
		all[i].Wh_l += sim[i].Wh_l;
		all[i].Wh_sum += sim[i].Wh_sum;
	}
	return all;
}
vector<result_t> exec_result_calc(vector<result_t> result,int num){
	int n = result.size();
	int i;
	for(i = 0;i < n;i++){
		result[i].deadline_rest /= num;
		result[i].Wh_h /= num;
		result[i].Wh_l /= num;
		result[i].Wh_sum /= num;
	}
	return result;
}

void print_csv(vector<result_t> result,string name_a){
	int num = result.size();
	cout << num << br ;
	int i;
	string name = name_a + "_ave.csv";
	ofstream outputfile(name.c_str(),ios::app);

	for(i = 0;i < result.size();i++){
		outputfile << result[i].Wh_sum << ",";
	}
	
	outputfile << br;
}

int main(int argc, char *argv[]){
	int num,i,opt;
	simulate sim;
	opterr = 0;//getopt()のエラーメッセージ無効 g:とかすると引数取れるとかなんとか
	int sim_flag = 0;//シナリオがちゃんと設定されてるか
	int csv_flag = 0;
	int run_mode = 0;//デフォルト
	int scenario_id;
	vector<simulate> list;
	list = make_list();
	int trial = 1;
	double Wh_sum = 0;
	vector<result_t> result_all;
	int check_point = 100;
	
	string csv_name;
	string options = "s:c:m:ha:wen:";
	string method;

	while( (opt = getopt(argc,argv, options.c_str()) ) != -1){
		//オプションがなくなるまで繰り返す
		switch(opt){
			case 'n':{
				trial = atoi(optarg);
				break;
			}
			case 's':{
				sim_flag = 1;
				scenario_id = atoi(optarg) - 1;
			//sim = list[( atoi(optarg) ) - 1];
				break;
			}
			case 'c' : csv_flag = 1; 
			csv_name = optarg;
			break;
			case 'm':
			method = optarg;
			break;
			case 'w':{
				run_mode = 1;
				break;
			}
			case 'h':{
				cout << "!!filename not required extension!!" << br;
				cout << "-s [ID]:scenario list.It is required" << br;
				for(int i = 0;i < list.size();i++){
					cout << "ID:" << setw(2) << i+1 << "    " << list[i].scenario_name << br;
				}
				cout << br;
				cout << "-m [method name]    :method list.It is required" << br;
				cout << "normal" << br;
				cout << "hc" << br;
				cout << "no_lock" << br;
				cout << "lock" << br;
				cout << "nlock(It is not use.new lock method)" << br;
				cout << br;
				cout << "-c [file name]      :Can output csv file of result" << br;
				cout << br;
				cout << "-a [result file]    :result file is csv file created by -c option" << br;
				cout << "                     if exec with -a, this program is not simulating." << br;
				cout << "                     it is shaping result file." << br;
				cout << "                     note:please see run* of makefile.this function support only this." << br;
				cout << br;
				cout << "-w                  :In default, load to ipc load. but mode -w, load to size of task." << br;
				cout << "                     actual task size is ideal size ~ WCET size. IPC is no change in this mode." << br;
				cout << br;
				cout << "-e                  :easy run. for debug." << br;
				cout << br;
				cout << "CHECK_POINT | c [number]            :Can Overwrite check point value." << br;
				cout << br;
				cout << "allocate | a linenum filename       :Can allocate resultfile." << br;
				cout << "                                 linenum = methodnum." << br;
				cout << br;
				cout << "non _first_check | nfc              :No switching check in first of tasks." << br;
				cout << br;
				cout << "-n [number]         :Number of trials";
				return 0;
				break;
			}
			case 'e':{
				//for debug
				sim_flag = 1;
				sim = list[2 - 1];
				method = "lock";
				run_mode = 0;
				break;
			}
			case 'a':{
			string tmp = optarg;
			//メソッド追加時は引数を変更する
			allocate_scenario(tmp,4);
			return 0;
			}
			default:{
				cout << "error:undefined option. defined op's =  " << options << br;
				cout << "please execute with option -h" << br; 
				return 0;
			}
		}
	}
	int flag = 0;
	int check_f = 0;
	for (i = optind; i < argc; i++) {
		if(strcmp(argv[i],"CHECK_POINT") == 0 || strcmp(argv[i],"c") == 0){
			flag = 1;
			//sim.CHECK_POINT = atoi(argv[i+1]);
			check_point = atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"non_first_check") == 0 || strcmp(argv[i],"nfc") == 0){
			sim.first_check = 0;
		}
		else if(strcmp(argv[i],"allocate") == 0 || strcmp(argv[i],"a") == 0 ){
			allocate_scenario(argv[i+2],atoi(argv[i+1]));
			return 0;
		}
		else if(flag == 0){
			cout << "!!!error undefined arg!!!" << br;
			return 0;
		}
		else{
			flag = 0;
		}
	}



for(i = 1;i <= trial;i++)//trial回行う
	if(sim_flag){
		list = make_list();//シナリオ再生成
		sim = list[scenario_id];
		#ifdef cons
		sim.print_params();
		#endif
		sim.method_name = method;
		sim.CHECK_POINT = check_point;
		switch(run_mode){
			case 0:{
				#ifdef cons
				cout << "run with changing ipc" << br;
				#endif
				sim.run_load_ipc(0.25);
				break;
			}
			case 1:{
				cout << "run with changing task size ideal ~ WCET" << br;
				sim.run_load_size(0.25);
				break;
			}
			default:
				cout << "!!!error: un defined run function!!!" << br;
				break;
		}
	if(i == 1){
		cout << "first trial!!" << br;
		result_all = sim.result;
	}
	else{
		result_all = add_result(result_all, sim.result);
	}
	//print_result(result_all);
	
	if(csv_flag == 1){
		//sim.result_csv(csv_name);
		print_csv(exec_result_calc(result_all, i),csv_name + "_" + sim.scenario_name + "_" + method + "_" + "CP_" + to_string(sim.CHECK_POINT));
	}
	}
	else{
		cout << "error:not set scenario" << br;
		cout << "please execute with option -h" << br;
	}
	result_all = exec_result_calc(result_all, trial);
	cout << "print average result. trial num = " << trial << br;
	print_result(result_all);
	if(csv_flag == 1){
		sim.result = result_all;
		sim.result_csv(csv_name);
	}
	return 0;
}
