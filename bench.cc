#include "bench.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
/*
fun_[name]��core_switch_[name]�̓y�A
hc�̓n�C�G���h�R�A�̂�
���������͐��˂���̎�@
lock�͓��J����̎�@
*/

void simulate::fun(double load){
	double task;//The size task of now simulating
	int t = 0;//time
	int state = 0, next_state = 0;//0:little 1:big
	int time_high = 0, time_low = 0;
	int i = 0;
	//cout  << "Load size " << load << br;
	
	while(story.get_task_num() != 0){
		state = 0;
		next_state = 0;
		i = 0;
		TASK_SIZE = story.get_top_size();
		#ifdef debug
		cout << "leave task num = " << story.get_task_num() << br;
		cout << "Task " << TASK_SIZE << "started...\n" << br;
		#endif
		update_wcet();
		task = TASK_SIZE;//load top task size
		story.rm_top();//remove this task in list
		if(first_check){
			state = core_switch(state,t,task,i);
		}
		while(task > 0){
			t++;
			//big
			if(state == 1){
				task -= h.ipc * load;
				time_high++;
			}
			//little
			else{
				task -= l.ipc * load;
				time_low++;
			}
			
			//core switching judge
			if(task > ( ((CHECK_POINT - i) - 1) * (TASK_SIZE / CHECK_POINT) ) &&
			task <= ( (CHECK_POINT - i) * (TASK_SIZE / CHECK_POINT) ) ){
				next_state = core_switch(state,t,task,i);
				i++;
				if(state != next_state){
					t += SWITCH;
					if(state == 1){//h->l
						time_high += SWITCH;
						state = 0;
					}
					else{//l->h
						time_low += SWITCH;
						state = 1;
					}
				}
			}
		}
		#ifdef debug
		printf("Deadline rest: %d\n",DEADLINE-t);
		printf("Time: %d ended...\n",t);
		printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
		#endif
	}
	state=0;
	#ifdef debug
	cout << "All task end!!!" << br;
	#endif
	#ifdef cons
	printf("Deadline rest: %d\n",DEADLINE-t);
	printf("Time: %d ended...\n",t);
	printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
	#endif
	double Wh_h, Wh_l;

	Wh_h = caluculate_Wh(h.ghz,h.W,time_high,load);
	Wh_l = caluculate_Wh(h.ghz,l.W,time_low,load);
	result_t restmp(load,DEADLINE-t,time_high,time_low,Wh_h,Wh_l);
	result.push_back(restmp);

}
//���ƂŃA���S���Y��������N���X�ɂ��Đ؂�ւ����悤�ɂ���Ƃ��Ȃ�Ƃ�
//�M���M���܂ŗ��߂Ă���n�C�ɂ���
int simulate::core_switch(int state,int time, double task,int index){
	int delta_high, delta_low, wcet_rest_high, rest_high;
	delta_high = wcet_task_high / CHECK_POINT;//���̃^�X�N�̂ЂƋ�Ԃ�hith��wcet
	delta_low = wcet_task_low / CHECK_POINT;

	
	//wcet_rest_high = (CHECK_POINT - i) * (wcet_task / CHECK_POINT);
	//���^�X�N�̎c���wcet
	wcet_rest_high = wcet_task_high - (delta_high * index); 	
	//cout << "wcet_rest_high = " << wcet_rest_high << "index = " << index << br;

	if(state == 0){
		/*�����ԁ{�X�C�b�`�^�C���{���̋�Ԃ���n�C�ɂ������̎��ԁ{����Ԃ����E�ł������̎���
		�������X�C�b�`���Ȃ��������̎���
		*/
		rest_high = time + SWITCH + wcet_rest_high - delta_high + delta_low;
		//cout << "DEADLINE:rest_high   " << DEADLINE << ":" << rest_high << br;
		if(DEADLINE < (rest_high + wcet_high - wcet_task_high)){
			state = 1;
			#ifdef debug
			cout << "Change HC mode! in " << time << br;
			#endif
		}
	}
	return state;
}

void simulate::fun_hc(double load){
	double task;//The size task of now simulating
	int t = 0;//time
	int state = 0, next_state = 0;//0:little 1:big
	int time_high = 0, time_low = 0;
	int i = 0;
	//cout  << "Load size " << load << br;
	
	while(story.get_task_num() != 0){
		state = 0;
		next_state = 0;
		i = 0;
		TASK_SIZE = story.get_top_size();
		#ifdef debug
		cout << "leave task num = " << story.get_task_num() << br;
		cout << "Task " << TASK_SIZE << "started...\n" << br;
		#endif
		update_wcet();
		task = TASK_SIZE;//load top task size
		story.rm_top();//remove this task in list
		while(task > 0){
			task -= h.ipc * load;
			time_high++;
		}
		#ifdef debug
		printf("Deadline rest: %d\n",DEADLINE-t);
		printf("Time: %d ended...\n",t);
		printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
		#endif
	}
	state=0;
	#ifdef debug
	cout << "All task end!!!" << br;
	#endif
	#ifdef cons
	printf("Deadline rest: %d\n",DEADLINE-t);
	printf("Time: %d ended...\n",t);
	printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
	#endif
	double Wh_h, Wh_l;

	Wh_h = caluculate_Wh(h.ghz,h.W,time_high,load);
	Wh_l = caluculate_Wh(h.ghz,l.W,time_low,load);
	result_t restmp(load,DEADLINE-t,time_high,time_low,Wh_h,Wh_l);
	result.push_back(restmp);

}



void simulate::fun_lock(double load){
	double task;//The size task of now simulating
	int t = 0;//time
	int state = 0, next_state = 0;//0:little 1:big
	int time_high = 0, time_low = 0;
	int i = 0;
	int lock = LOCK;
	//cout  << "Load size " << load << br;
	
	while(story.get_task_num() != 0){
		state = 0;
		next_state = state;
		i = 0;
		TASK_SIZE = story.get_top_size();
		lock = LOCK;
		#ifdef debug
		cout << "leave task num = " << story.get_task_num() << br;
		cout << "Task " << TASK_SIZE << "started...\n" << br;
		#endif
		update_wcet();
		task = TASK_SIZE;//load top task size
		story.rm_top();//remove this task in list
		if(first_check){
			state = core_switch_lock(state,t,task,i,lock);
		}
		while(task > 0){
			t++;
			//big
			if(state == 1){
				task -= h.ipc * load;
				time_high++;
				lock--;
			}
			//little
			else if(state == 0){
				task -= l.ipc * load;
				time_low++;
			}
			else{
				cout << "error:undefined core " << br;
				return;
			}
			
			//core switching judge
			if(task > ( ((CHECK_POINT - i) - 1) * (TASK_SIZE / CHECK_POINT) ) &&
			task <= ( (CHECK_POINT - i) * (TASK_SIZE / CHECK_POINT) ) ){
				next_state = core_switch_lock(state,t,task,i,lock);
				i++;
				if(state != next_state){
					t += SWITCH;
					if(state == 1){//h->l
						time_high += SWITCH;
						state = 0;
					}
					else if(state == 0){//l->h
						time_low += SWITCH;
						state = 1;
						lock = LOCK;
					}
					else{
						cout << "error:unknown state" << br;
					}
				}
			}
		}
		#ifdef debug
		printf("Deadline rest: %d\n",DEADLINE-t);
		printf("Time: %d ended...\n",t);
		printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
		#endif
	}
	state=0;
	#ifdef debug
	cout << "All task end!!!" << br;
	#endif
	#ifdef cons
	printf("Deadline rest: %d\n",DEADLINE-t);
	printf("Time: %d ended...\n",t);
	printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
	#endif
	double Wh_h, Wh_l;

	Wh_h = caluculate_Wh(h.ghz,h.W,time_high,load);
	Wh_l = caluculate_Wh(h.ghz,l.W,time_low,load);
	result_t restmp(load,DEADLINE-t,time_high,time_low,Wh_h,Wh_l);
	result.push_back(restmp);

}
/*
��s�����̎�@�i��������́j
*/
int simulate::core_switch_lock(int state,int time, double task,int index,int lock){
	int delta_high, delta_low, wcet_rest_high, rest_high;
	delta_high = wcet_task_high / CHECK_POINT;//���̃^�X�N�̂ЂƋ�Ԃ�hith��wcet
	delta_low = wcet_task_low / CHECK_POINT;
	
	//wcet_rest_high = (CHECK_POINT - i) * (wcet_task / CHECK_POINT);
	//���^�X�N�̎c���wcet
	wcet_rest_high = wcet_task_high - (delta_high * index); 


		/*�����ԁ{�X�C�b�`�^�C���{���̋�Ԃ���n�C�ɂ������̎��ԁ{����Ԃ����E�ł������̎���
		�������X�C�b�`���Ȃ��������̎���
		*/
		rest_high = time + SWITCH + wcet_rest_high - delta_high + delta_low;
	if(state == 0){
		//���E�Ō����������Ƀn�C�ɂ����ꍇ�̎��ԁ{�S�̂̍ň����s���ԁ[���̃^�X�N�̍ň����s����
		if(DEADLINE <= (rest_high + wcet_high - wcet_task_high)){
			state = 1;
			#ifdef debug
			cout << "Change HC mode! in " << time << br;
			#endif
		}
	}	
	else if(lock <= 0){
		if(DEADLINE >  (rest_high + wcet_high - wcet_task_high)){
			state = 0;
			#ifdef debug
			cout << "Return LC mode! in " << time << br;
			#endif
		}
	}
	return state;
}




void simulate::fun_nlock(double load){
	double task;//The size task of now simulating
	int t = 0;//time
	int state = 0, next_state = 0;//0:little 1:big
	int time_high = 0, time_low = 0;
	int i = 0;
	int lock = LOCK;
	//cout  << "Load size " << load << br;
	
	while(story.get_task_num() != 0){
		state = 0;
		next_state = state;
		i = 0;
		TASK_SIZE = story.get_top_size();
		lock = LOCK;
		#ifdef debug
		cout << "leave task num = " << story.get_task_num() << br;
		cout << "Task " << TASK_SIZE << "started...\n" << br;
		#endif
		update_wcet();
		task = TASK_SIZE;//load top task size
		story.rm_top();//remove this task in list
		state = core_switch_nlock(state,t,task,i,lock);
		while(task > 0){
			t++;
			//big
			if(state == 1){
				task -= h.ipc * load;
				time_high++;
				lock--;
			}
			//little
			else if(state == 0){
				task -= l.ipc * load;
				time_low++;
			}
			else{
				cout << "error:undefined core " << br;
				return;
			}
			
			//core switching judge
			if(task > ( ((CHECK_POINT - i) - 1) * (TASK_SIZE / CHECK_POINT) ) &&
			task <= ( (CHECK_POINT - i) * (TASK_SIZE / CHECK_POINT) ) ){
				next_state = core_switch_nlock(state,t,task,i,lock);
				i++;
				if(state != next_state){
					t += SWITCH;
					if(state == 1){//h->l
						time_high += SWITCH;
						state = 0;
					}
					else if(state == 0){//l->h
						time_low += SWITCH;
						state = 1;
						lock = LOCK;
					}
					else{
						cout << "error:unknown state" << br;
					}
				}
			}
		}
		#ifdef debug
		printf("Deadline rest: %d\n",DEADLINE-t);
		printf("Time: %d ended...\n",t);
		printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
		#endif
	}
	state=0;
	#ifdef debug
	cout << "All task end!!!" << br;
	#endif
	#ifdef cons
	printf("Deadline rest: %d\n",DEADLINE-t);
	printf("Time: %d ended...\n",t);
	printf("HC Time: %d\nLC Time: %d\n",time_high,time_low);
	#endif
	double Wh_h, Wh_l;

	Wh_h = caluculate_Wh(h.ghz,h.W,time_high,load);
	Wh_l = caluculate_Wh(h.ghz,l.W,time_low,load);
	result_t restmp(load,DEADLINE-t,time_high,time_low,Wh_h,Wh_l);
	result.push_back(restmp);

}
/*
���ǔ�
*/
int simulate::core_switch_nlock(int state,int time, double task,int index,int lock){
	int delta_high, delta_low, wcet_rest_high, rest_high;
	delta_high = wcet_task_high / CHECK_POINT;//���̃^�X�N�̂ЂƋ�Ԃ�hith��wcet
	delta_low = wcet_task_low / CHECK_POINT;
	
	//wcet_rest_high = (CHECK_POINT - i) * (wcet_task / CHECK_POINT);
	//���^�X�N�̎c���wcet
	wcet_rest_high = wcet_task_high - (delta_high * index); 


		/*�����ԁ{�X�C�b�`�^�C���{���̋�Ԃ���n�C�ɂ������̎��ԁ{����Ԃ����E�ł������̎���
		�������X�C�b�`���Ȃ��������̎���
		*/
		rest_high = time + SWITCH + wcet_rest_high - delta_high + delta_low;
	if(state == 0){
		//���E�Ō����������Ƀn�C�ɂ����ꍇ�̎��ԁ{�S�̂̍ň����s���ԁ[���̃^�X�N�̍ň����s����
		if(DEADLINE < (rest_high + wcet_high - wcet_task_high)){
			state = 1;
			#ifdef debug
			cout << "Change HC mode! in " << time << br;
			#endif
		}
	}	
	else if(lock <= 0){
		if(DEADLINE >  (rest_high + wcet_high - wcet_task_high)){
			state = 0;
			#ifdef debug
			cout << "Return LC mode! in " << time << br;
			#endif
		}
	}
	return state;
}
