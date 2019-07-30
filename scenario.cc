#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <random>
#include "bench.h"
//http://siv3d.hateblo.jp/entry/2013/02/17/231829
simulate pre_scenario(void){
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	//scenario story(280000,280000);
	scenario story(1120000,1120000);
	//story.story[0].set_size(0);
	//story.add_task(112000,112000);
	simulate sim(big,little,story,"pre_scenario");
	//sim.simulate_init();
	//sim.print_params();
	return sim;
}

simulate scenario1(int flag){
	//cout << "scenario1" << br;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	scenario story(620000,620000);
	story.add_task(25000, 100000);//1/4
	story.add_task(50000, 500000);//1/10
	simulate sim(big,little,story,"scenario1");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("scenario1_re");
		sim.comment = "Proposed";
		}
	return sim;
}



simulate scenario1_rarity(int flag){
	//cout << "scenario1" << br;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	scenario story(620000,620000);
	story.add_task(25000, 100000);//1/4
	story.add_task(500000, 500000);//1/10
	simulate sim(big,little,story,"scenario1_rarity");
	//sim.simulate_init()
//	sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("scenario1_rarity_re");
		sim.comment = "Proposed";
		}
	return sim;
}

simulate scenario2(int flag){
	//cout << "scenario1" << br;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	scenario story(620000,620000);
	story.add_task(50000, 100000);//1/2
	story.add_task(150000, 500000);//3/10
	simulate sim(big,little,story,"scenario2");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
	reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
	sim.rename_scenario("scenario2_re");
	sim.comment = "Proposed";
	}
	return sim;
}

simulate scenario3(int flag){
	//cout << "scenario1" << br;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	scenario story(620000,620000);
	story.add_task(75000, 100000);//3/4
	story.add_task(250000, 500000);//1/2
	simulate sim(big,little,story,"scenario3");
	//sim.simulate_init()
//	sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
	reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
	sim.rename_scenario("scenario3_re");
	sim.comment = "Proposed";
	}
	return sim;
}

simulate scenario4(int flag){
	//cout << "scenario1" << br;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	scenario story(620000,620000);
	story.add_task(100000, 100000);//3/4
	story.add_task(500000, 500000);//1/2
	simulate sim(big,little,story,"scenario4");
	//sim.simulate_init()
//	sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
	reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
	sim.rename_scenario("scenario4_re");
	sim.comment = "Proposed";
	}
	return sim;
}


simulate scenario5(int flag){
	//cout << "scenario1" << br;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	scenario story(620000,620000);
	story.add_task(25000, 100000);//1/4
	story.add_task(5000, 500000+180000);//1/100
	simulate sim(big,little,story,"scenario5");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("scenario5_re");
		sim.comment = "Proposed";
		}
	return sim;
}


simulate scenario7_a(int flag){
	//cout << "scenario1" << br;
	int i;
	int xi;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	random_device rd;
	
		mt19937 mt(rd());	
		uniform_int_distribution<int> dice(1,100);
	scenario story;
	//画面描写
	xi = dice(mt);
	if(xi < 91){
		story.add_task(620000,620000);
	}
	else if(xi < 96){
		story.add_task(610000,620000);
	}		
	else if(xi < 101){
		story.add_task(600000,620000);
	}
	else{
		cout << "xi error" << br;
	}
	//scenario story(620000,620000);
	//位置情報取得
	xi = dice(mt);
	if(xi < 92){
		story.add_task(10000, 100000);
	}
	else if(xi < 98){
		story.add_task(50000, 100000);
	}		
	else if(xi < 101){
		story.add_task(100000, 100000);
	}
	else{
		cout << "xi error" << br;
	}
	//story.add_task(25000, 100000);//1/4
	//ルート計算
	/*
	xi = dice(mt);
	if(xi < 100){
		story.add_task(5000, 500000+180000);//1/100
	}
	else if(xi < 101){
		xi = dice(mt);
		if(xi < 100){
			story.add_task(5000, 500000+180000);
		}
		else if(xi < 101){
			if(xi < 100){
				story.add_task(100000, 500000+180000);
			}
			else{
				story.add_task(150000, 500000+180000);
			}
		}
		else {
			cout << "xi error" << br;
		}
	}
	else{
		cout << "xi error" << br;
	}
	*/
	story.add_task(5000, 500000+180000);//1/100

	simulate sim(big,little,story,"scenario5_highway");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("scenario5_highway_re");
		sim.comment = "Proposed";
		}
	return sim;
}


simulate scenario7_b(int flag){
	//cout << "scenario1" << br;
	int i;
	int xi;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	random_device rd;
	scenario story;
	
		mt19937 mt(rd());	
		uniform_int_distribution<int> dice(1,100);
	//画面描写
	xi = dice(mt);
	if(xi < 91){
		story.add_task(620000,620000);
	}
	else if(xi < 96){
		story.add_task(610000,620000);
	}		
	else if(xi < 101){
		story.add_task(600000,620000);
	}
	else{
		cout << "xi error" << br;
	}
	//scenario story(620000,620000);
	//位置情報取得
	xi = dice(mt);
	if(xi < 97){
		story.add_task(10000, 100000);
	}
	else if(xi < 101){
		story.add_task(25000, 100000);
	}	
	else{
		cout << "xi error" << br;
	}
	//story.add_task(25000, 100000);//1/4
	//ルート計算
	xi = dice(mt);
	if(xi < 100){
		story.add_task(5000, 500000+180000);//1/100
	}
	else if(xi < 101){
		xi = dice(mt);
		if(xi < 101){
			xi = dice(mt);
			if(xi < 51){
				story.add_task(100000, 500000+180000);
			}
			else{
				story.add_task(300000, 500000+180000);
			}
		}
		else{
			cout << "xi error" << br;
		}
	}
	//story.add_task(5000, 500000+180000);//1/100

	simulate sim(big,little,story,"scenario5_city");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("scenario5_city_re");
		sim.comment = "Proposed";
		}
	return sim;
}

simulate scenario7_c(int flag){
	//cout << "scenario1" << br;
	int i;
	int xi;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	random_device rd;
	scenario story;
	
		mt19937 mt(rd());	
		uniform_int_distribution<int> dice(1,100);
	//画面描写
	xi = dice(mt);
	if(xi < 91){
		story.add_task(620000,620000);
	}
	else if(xi < 96){
		story.add_task(610000,620000);
	}		
	else if(xi < 101){
		story.add_task(600000,620000);
	}
	else{
		cout << "xi error" << br;
	}
	//scenario story(620000,620000);
	//位置情報取得
	xi = dice(mt);
	if(xi < 98){
		story.add_task(10000, 100000);
	}
	else if(xi < 101){
		story.add_task(25000, 100000);
	}	
	else{
		cout << "xi error" << br;
	}
	//story.add_task(25000, 100000);//1/4
	//ルート計算
	xi = dice(mt);
	if(xi < 98){
		story.add_task(5000, 500000+180000);//1/100
	}
	else if(xi <  100){
		story.add_task(100000, 500000+180000);	
	}
	else if(xi < 101){
		xi = dice(mt);
		if(xi < 51){
		story.add_task(100000, 500000+180000);
		}
		else{
			story.add_task(500000, 500000+180000);			
		}
	}
	else{
		cout << "xi error" << br;
	}
	//	story.add_task(5000, 500000+180000);//1/100

	simulate sim(big,little,story,"scenario5_homearea");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("scenario5_homearea_re");
		sim.comment = "Proposed";
		}
	return sim;
}


simulate scenario7_d(int flag){
	//cout << "scenario1" << br;
	int i;
	int xi;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	random_device rd;
	scenario story;
	
		mt19937 mt(rd());	
		uniform_int_distribution<int> dice(1,100);
		uniform_int_distribution<int> dice2(1,10);

	//処理計算
	xi = dice(mt);
	if(xi < 71){
		story.add_task(100000, 700000);
	}
	else if(xi < 86){
		story.add_task(350000, 700000);		
	}
	else if(xi < 96){
		story.add_task(500000, 700000);		
	}
	else if(xi < 101){
		story.add_task(690000, 700000);
	}	
	else{
		cout << "xi error" << br;
	}

	//入力処理
	//xi = dice2(mt);
	if(xi < 51){
		story.add_task(7000, 140000);
	}
	else if(xi < 76){
		story.add_task(7000*2, 140000);
	}
	else if(xi < 101){
		story.add_task(7000*3, 140000);
	}
	else{
		cout << "xi error" << br;
	}

	
	//画面描写
	xi = dice(mt);
	if(xi < 71){
		story.add_task(10000,560000);
	}
	else if(xi < 81){
		story.add_task(200000,560000);
	}
	else if(xi < 96){
		story.add_task(400000,560000);		
	}
	else if(xi < 101){
		story.add_task(550000,560000);
	}		
	else{
		cout << "xi error" << br;
	}


	simulate sim(big,little,story,"game");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("game_re");
		sim.comment = "Proposed";
		}
	return sim;
}


simulate scenario7_e(int flag){
	//cout << "scenario1" << br;
	int i;
	int xi;
	core big(2.8,4,46.44);
	core little(0.7,1,3.44);
	random_device rd;
	scenario story;
	
		mt19937 mt(rd());	
		uniform_int_distribution<int> dice(1,100);
		uniform_int_distribution<int> dice2(1,10);

	//処理計算
	xi = dice(mt);
	if(xi < 71){
		story.add_task(100000, 700000);
	}
	else if(xi < 86){
		story.add_task(350000, 700000);		
	}
	else if(xi < 96){
		story.add_task(500000, 700000);		
	}
	else if(xi < 101){
		story.add_task(690000, 700000);
	}	
	else{
		cout << "xi error" << br;
	}

	//入力処理
	xi = dice2(mt);
	if(xi < 11){
		story.add_task(14000*xi, 140000);	
	}
	else{
		cout << "xi error" << br;
	}

	
	//画面描写
	xi = dice(mt);
	if(xi < 71){
		story.add_task(10000,560000);
	}
	else if(xi < 81){
		story.add_task(200000,560000);
	}
	else if(xi < 96){
		story.add_task(400000,560000);		
	}
	else if(xi < 101){
		story.add_task(550000,560000);
	}		
	else{
		cout << "xi error" << br;
	}


	simulate sim(big,little,story,"game2");
	//sim.simulate_init()
	//sim.print_params();
	sim.comment = "Previous";
	if(flag == 1){
		reverse(sim.hold_story.story.begin(),sim.hold_story.story.end());
		sim.rename_scenario("game2_re");
		sim.comment = "Proposed";
		}
	return sim;
}
