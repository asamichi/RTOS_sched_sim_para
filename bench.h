
/********************************
本ファイルはシミュレータの汎用的な機能のファイルです。
手法によって変わるスケジューリング関係はbench.ccに書いてます。
*********************************/

/*
ベクター型のメモ（調べれば出る）
std::vector<double> v;//生成
v.push_back(232);//末尾に２３２追加


class tmp{
	tmp(double a,double b);//コンストラクタ
};

vector<tmp> a;
a.emplace_back(3,5);//aの末尾に3,5で初期化されたものを追加(嘘っぽい、調べて)
a[3]とかで参照

要素数
a.size();
a.front(); 最初の要素 a[0]
a.back();　最後の a[a.size() - 1]２つとも代入にも使えるし参照にも
*/

#ifndef __SIM_H_INCLUDED__
#define __SIM_H_INCLUDED__

//#define debug 
//#define cons
#define cons_result

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <omp.h>

using namespace std;

class scenario;
class task;
class core;

#define br "\n";
#define  chank  "--------------------" << br;

/*
シナリオ内の１タスクのクラス
確率遷移の関係はシナリオ生成時に処理するようになってます。
simulater.ccの方で繰り返し試行する場合に試行毎にシナリオを再度読みこむようになってます。
*/
class task{
public:
	double ideal_size;//最良実行時間
	double WCET_size;//最悪実行時間
	double size;//実際の実行時間
	
	task(double ideal,double WCET){
		ideal_size = ideal;
		WCET_size = WCET;
		size = -1;
	}
	//loadで実際の実行時間が最良からなん％最悪によってるか指定できる。
	int set_size(double load){
		if(load < 0 || load > 1){
			cout << "error:load value" << br;
			return 0;
		}
		size = ideal_size + ( (WCET_size - ideal_size) * load);
		return 1;
	}
	//いろいろ表示する
	void print(void){
		cout << chank;
		cout << "ideal_size = " << ideal_size << br; 
		cout << "WCET_size  = " << WCET_size << br;
		cout << "size       = " << size << br;
		cout << chank;
	}
};

/*
コアの設定クラス
*/
class core{
public:
	double ghz;
	int ipc;
	double W;//ワット
	core(void){
	}
	core(double hz,int ip){
		ghz = hz;
		ipc = ip;
	}
	core(double hz,int ip,double W_a){
		ghz = hz;
		ipc = ip;
		W = W_a;
	}
	//1msに実行できる命令数（未使用）
	int get_inst_1ms(void){
		return ghz * ipc * 1000000;
	}
	
};

/*
taskのvector配列のクラス。
１本のシナリオを表現するクラス。
使い方とかはscenario.cc見たらわかると思う。
クラスのオブジェクト作ってベクター配列にタスク登録していく感じ
*/
class scenario{
public:
	//一連のタスクからなる
	vector<task> story;
	//constracta
	scenario(void){
	}
	scenario(double ideal,double wcet){
		story.push_back( task(ideal,wcet) );
		(story.back()).set_size(0);
	}
	/*最良と最悪の間でどのくらいか指定できるやつ*/
	scenario(double ideal,double wcet,int load){
		story.push_back( task(ideal,wcet) );
		(story.back()).set_size(load);
	}
	scenario(vector<task> stor){
		story = stor;	
	}
	

	void add_task(double ideal,double wcet){
		story.push_back( task(ideal,wcet) );
		(story.back()).set_size(0);
	}
	void add_task(double ideal,double wcet,double load){
		story.push_back( task(ideal,wcet) );
		(story.back()).set_size(load);
	}
	int set_task_load(double load){
		int num = story.size();
		int i;
		for(i = 0;i < num;i++){
			story[i].set_size(load);
		}
		return 0;
	}

	int get_wcet_size(void){
		int size = story.size();
		int ans = 0;
		int i;
		for(i = 0;i < size;i++){
			ans += story[i].WCET_size;
		}
		return ans;
	}
	int get_size(void){
		int size = story.size();
		int ans = 0;
		int i;
		for(i = 0;i < size;i++){
			if(story[i].size == -1)
				cout << "!!! size = -1 !!!" << br;
			ans += story[i].size;
		}
		return ans;
	}
	//シナリオは配列の最後から実行される
	int get_top_size(void){
		task tmp = story.back();
		return tmp.size;
	}
	int get_top_wcet_size(void){
		return (story.back()).WCET_size;
	}
	void rm_top(void){
		if(story.size() == 0){
			cout << "error in rm_top" << br;
		}
		story.pop_back();
	}
	int get_task_num(void){
		return story.size();	
	}	
	void print(void){
		int size = story.size();
		int i;
		cout << chank;
		cout << "task num = " << size << br;
		//最後から実行されるので最後から表示する
		for(i = size-1;i >= 0;i--){
			story[i].print();
		}
		cout << chank;
	}

};
/*結果を格納するためのクラス
*/
class result_t{
public:
	double load;
	int deadline_rest;
	int time_high;
	int time_low;
	double Wh_h;
	double Wh_l;
	double Wh_sum;
	int column_num;
	result_t(double load_a,int deadline_rest_a,int time_high_a,int time_low_a,double Wh_h_a,double Wh_l_a){
		load = load_a;
		deadline_rest = deadline_rest_a;
		time_high = time_high_a;
		time_low = time_low_a;
		Wh_h = Wh_h_a;
		Wh_l = Wh_l_a;
		Wh_sum = Wh_h + Wh_l;
		result_t_init();
	}
	result_t(void){
		result_t_init();
	}
	void result_t_init(void){
		column_num = 7;
	}
	//あとで１カラムの文字数を指定する形に変えてもいいかもしれない
	void print_column(void){
		cout << "load          deadline_rest time_high     time_low      Wh_h          Wh_l          Wh_sum" << br;
	}
	void print_var(void){
		cout << setw(14) << left << load;
		cout << setw(14) << left <<deadline_rest;		
		cout << setw(14) << left <<time_high;
		cout << setw(14) << left <<time_low;
		cout << setw(14) << left <<Wh_h;
		cout << setw(14) << left <<Wh_l;
		cout << setw(14) << left <<Wh_sum;
		cout << br;
	}
	//for output csv
	string print_column_csv(void){
		string ans =  "load,deadline_rest,time_high,time_low,Wh_h,Wh_l,Wh_sum";
		ans += br;
		//cout << ans;
		return ans;
	}
	string print_var_csv(void){
		string ans;
		stringstream ansst;
		ansst << load << "," << deadline_rest << "," << time_high << "," << time_low << "," << Wh_h <<"," << Wh_l  << "," << Wh_sum << br;
		/*
		ans += load; 			ans += ",";
		ans += deadline_rest;	ans += ",";
		ans += time_high;		ans += ",";
		ans += time_low;		ans += ",";
		ans += Wh_h;			ans += ",";
		ans += Wh_l;			ans += ",";
		ans += Wh_sum;			ans += ",";
		ans += br;
		*/
		//cout << ansst.str();
		ans = ansst.str();
		return ans;
	}
};


/*
シミュレート本体
シナリオとその他パラメータ受け取って、シミュレートして、
リザルトクラスのオブジェクトに結果入れる。
必要ならCSVファイルに出力する。
*/
class simulate{
public:
	core h;
	core l;
	long long PERIOD;
	long long DEADLINE;
	double TASK_SIZE;
	long long SWITCH;
	long long wcet_high;
	long long wcet_low;
	long long wcet_task_high;
	long long  wcet_task_low;
	long long CHECK_POINT;
	int LOCK;
	double load_max;
	int init_flag;
	int first_check;
	string scenario_name;
	string method_name;
	scenario story,hold_story;
	vector<result_t> result;
	string comment;
	//vector<vector<result_t> >  sim_result;
	
	void fun_hc(double load);
	void fun(double load);
	void fun_lock(double load);
	void fun_nlock(double load);
	int core_switch(int state,int time, double task,int index);
	int core_switch_lock(int state,int time, double task,int index,int lock);
	int core_switch_nlock(int state,int time, double task,int index,int lock);
	simulate(void){
		//作るだけ。後で初期化する。
		init_flag = 0;
	}
	simulate(core big,core little,scenario stor){
		init_flag = 0;
		simulate_init(big,little,stor);
	}
	simulate(core big,core little,scenario stor,string name){
		init_flag = 0;
		simulate_init(big,little,stor);
		scenario_name = name;
	}
	void simulate_init(core big,core little,scenario stor){
		if(init_flag != 0){
			cout << "allready inited" << br;
			exit(1);
		}
		h = big;
		l = little;
		hold_story = stor;
		load_max = -1;//負荷設定忘れ防止用
		simulate_init();
		//story = stor;
		//copy(stor);
	}
	void rename_scenario(string name){
		scenario_name = name;
	}
	void copy_story(scenario a){
		//copy(a.begin(),a.end(),back_inserter(story) );
	}

	int check_init(void){
		if(DEADLINE < wcet_high){
			cout << "!!!DEADLINE < wcet_high!!!" << br;
			cout << "DEADLINE = " << DEADLINE << br;
			cout << "wcet_high =" << wcet_high << br;
			return 1;
		}
		if(PERIOD < CHECK_POINT){
			cout << "!!!PERIOD < CHECKPOINT!!!" << br;
			return 1;		
		}
		if(load_max == -1){
			cout << "!!!load_max == -1" << br;
			return 1;
		}
		if(init_flag == 0){
			cout << "error:not init" << br;
			return 1;
		}
		return 0;
	}
	void simulate_init(void){
		if(init_flag != 0){
			cout << "already inited" << br;
			exit(1);
		}
		story = hold_story;
		reverse(story.story.begin(),story.story.end() );
		PERIOD = h.ghz * 1000000;
		DEADLINE = PERIOD*0.5;//0.5 ~
		TASK_SIZE = story.get_top_size();//命令数 * high ipc
		SWITCH = PERIOD / 1000;
		wcet_high = (story.get_wcet_size() / h.ipc) / load_max;//負荷最大でのWCET
		wcet_low = (story.get_wcet_size() / l.ipc) / load_max;
		wcet_task_high = (story.get_top_wcet_size() / h.ipc) / load_max;//負荷最大でのWCET
		wcet_task_low = (story.get_top_wcet_size() / l.ipc) / load_max;
		CHECK_POINT = 100;//default value
		LOCK = PERIOD / 50;
		first_check = 1;
		init_flag = 1;
		//check_init();
	}
	void reload_story(void){
		story = hold_story;
		reverse(story.story.begin(),story.story.end() );
		TASK_SIZE = story.get_top_size();//命令数 * high ipc
		wcet_high = (story.get_wcet_size() / h.ipc) / load_max;//負荷最大でのWCET
		wcet_low = (story.get_wcet_size() / l.ipc) / load_max;
		wcet_task_high = (story.get_top_wcet_size() / h.ipc) / load_max;//負荷最大でのWCET
		wcet_task_low = (story.get_top_wcet_size() / l.ipc) / load_max;
	}
	void update_wcet(void){
		wcet_high = (story.get_wcet_size() / h.ipc) / load_max;//負荷最大でのWCET
		wcet_low = (story.get_wcet_size() / l.ipc) / load_max;
		wcet_task_high = (story.get_top_wcet_size() / h.ipc) / load_max;//負荷最大でのWCET
		wcet_task_low = (story.get_top_wcet_size() / l.ipc) / load_max;
		#ifdef debug
		cout << chank;
		cout << "update wcets" << br;
		cout << "wcet_high = " << wcet_high << br;
		cout << "wcet_low = " << wcet_low << br;
		cout << "wcet_task_high = " << wcet_task_high << br;
		cout << "wcet_task_low = " << wcet_task_low << br;
		cout << chank;
		#endif
	}

	void print_params(void){
//		#ifdef debug
		cout << chank;
		cout << "PERIOD      = " << PERIOD << br;
		cout << "DEADLINE    = " << DEADLINE << br;
		cout << "TASK_SIZE   = " << fixed <<TASK_SIZE << br;
		cout << "SWITCH      = " << SWITCH << br;
		cout << "wcet_high   = " << wcet_high << br;
		cout << "wcet_low    = " << wcet_low << br;
		cout << "CHECK_POINT = " << CHECK_POINT << br;
		cout << "load_max    = " << load_max << br;
		cout << "LOCK        = " << LOCK << br;
		story.print();
		cout << chank;
		cout << "---not params---" << br;
		cout << "ALL TASK SIZE = " << story.get_size() << br;
		cout << "ALL WCET      = " << story.get_wcet_size() << br;
		cout << chank;
//		#endif
	}
	/*-------------------------------------------------*/


	double caluculate_Wh(double W,int second){
		int h;
		h = second / 60;
		h = h / 60;
		return W * h;
	}
	int caluculate_time(double GHz,int time){
		int Hz;
		Hz = GHz * 1000000000;
		return time / Hz;
	}
	double caluculate_Wh(double GHz,double W,int time_a,double load){
		double reciprocal;
		double real_cycle;
		double time;
		double Wh;
		#ifdef debug
		cout << "Ghz,W,time_a,load" << br;
		cout << GHz << " " << W << " " << time_a << " " << load << br;
		#endif
		real_cycle = time_a * load;
		reciprocal = 1 /    (GHz * 1000000000); 
		time = real_cycle / (GHz * 1000000000); // * reciprocal; // (GHz * 1000000000);
		#ifdef debug
		cout << "reciprocal = " << reciprocal << br;
		cout << "real_cycle,time" << br;
		cout << real_cycle << " " << time << br;
		cout << "return " << W*time*3600 << br;
		#endif
		//return W * time * 3600;
		return (W * real_cycle * 3600) / (GHz * 1000000000);
		//return  ((time / (GHz * 1000000000)) * 3600 ) * W * load; 
	}

	void print_result(void){
		int num = result.size();
		int i;
		result[0].print_column();
		for(i = 0;i < num;i++){
			result[i].print_var();
		}
	}

	void result_csv(string name_a){
		int num = result.size();
		int i;
		string name = name_a + ".csv";
		ofstream outputfile(name.c_str(),ios::app);
		//outputfile << method_name << "," << scenario_name 	<< ",,,,," << br;
		reload_story();
		outputfile << method_name << "," << scenario_name << "," << 
					  "CHECK_POINT = " << CHECK_POINT << "," <<
					  "DEAD_LINE = " << DEADLINE << ","
					  "ALL TASK SIZE =" << story.get_wcet_size() << ",";
		outputfile << "first check = " << first_check << "," << br;
		outputfile << result[0].print_column_csv();
		if(comment != ""){
			outputfile << "load,";
			for(int j = 1;j < result[0].column_num;j++){
			outputfile << comment;
			if(j != (result[0].column_num - 1))
			outputfile << ",";
			}
			outputfile << br;
		}
		for(i = 0;i < num;i++){
			outputfile << result[i].print_var_csv();
		}
		for(int j = 1;j < result[0].column_num;j++)
		outputfile << ",";
		outputfile << br;
		if(comment == ""){
			for(int j = 1;j < result[0].column_num;j++)
			outputfile << ",";
		outputfile << br;
		}
	}

	

/*通常*/
	void run_load_ipc(double load_a){
		load_max = load_a;
		update_wcet();
		if(check_init() ){
			exit(1);
		}
		//cout << scenario_name << " start!!" << br;
		//print_params();
		double i = TASK_SIZE;
		double load;
		#ifdef cons
		cout << "Start Task" << br;
		#endif
		for(load = 1;load >= load_max - 0.01;load-=0.05){
			#ifdef cons
			cout << chank;
			cout << chank;
			#endif
			i = TASK_SIZE;
			//負荷変えるときにタスク数減ったままだとまずいから再読み込み
			//シナリオの再生成ではない
			reload_story();
			#ifdef cons
			cout << "Load size " << load << br;
			#endif
			if(method_name == "normal"){
			fun(load);
			}
			else if(method_name == "hc"){
				fun_hc(load);
			}
			else if(method_name == "no_lock"){
				LOCK = 0;
				fun_lock(load);
			}
			else if(method_name == "lock"){
				fun_lock(load);
			}
			else if(method_name == "nlock"){
				fun_nlock(load);
			}
			else{
				cout << "error:undefined method name" << br;
				cout << "please execute with option -h" << br;
				return ;
			}

			//fun_lock(i,load);
		}
		#ifdef cons_result
		print_result();
		cout << chank;
		#endif
		//result_csv();
	}

	/*
	タスクサイズを最良?最悪まで割合変動させるモード
	*/
	void run_load_size(double load_a){
		DEADLINE = (story.get_wcet_size() / h.ipc);
		load_max = 1;
		update_wcet();
		if( check_init() ){
			exit(1);
		}
		cout << scenario_name << " start!!" << br;
		print_params();
		double i = TASK_SIZE;
		double load;
		cout << "Start Task" << br;
		for(load = 0;load <= 1;load+=0.1){
			cout << chank;
			cout << chank;
			i = TASK_SIZE;
			reload_story();
			story.set_task_load(load);
			cout << "Load size = " << load << br;
			//story.print();
			if(method_name == "normal"){
			fun(1);
			}
			else if(method_name == "hc"){
				fun_hc(1);
			}
			else if(method_name == "no_lock"){
				LOCK = 0;
				fun_lock(1);
			}
			else if(method_name == "lock"){
				fun_lock(1);
			}
			else if(method_name == "nlock"){
				fun_nlock(1);
			}
			else{
				cout << "error:undefined method name" << br;
				cout << "please execute with option -h" << br;
				return ;
			}
			result.back().load = load;
			//fun_lock(i,load);
		}
		print_result();
		cout << chank;
		//result_csv();
	}
};



#endif