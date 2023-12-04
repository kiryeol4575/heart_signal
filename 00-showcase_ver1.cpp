#include "openai.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum _look{
	GENDER,
	HEIGHT,
	BODY,
	SKIN_COLOR,
	EYE,
	EYE_COLOR,
	HAIR,
	HAIR_COLOR,
	PASHION,
};

class data_look{
private:
	vector<string> param;
	vector<vector<string>> list;
	int param_len;
	vector<int> list_len;
public:


	data_look(){
		param = {"gender", "weight", "height", "skin color", "eye", "eye color", "hair", "hair color", "pashion"};
		list.push_back({"female", "male"});
		list.push_back({"standard", "slender", "plump", "hot"});
		list.push_back({"155~160cm", "160~165cm", "165~170cm", "170~175cm"});
		list.push_back({"dark", "bright"});
		list.push_back({"like dog", "like cat", "like deer", "like fox"});
		list.push_back({"black", "blue", "brown", "red"});
		list.push_back({"long hair"});
		list.push_back({"black", "brown", "red", "blonde"});
		list.push_back({"onepiece"});
		
		param_len = (int)param.size();
		for(int i = 0; i<param_len; i++){
			list_len.push_back( (int)list[i].size() );
		}
	}
	int get_param_len(){
		return param_len;
	}
	int get_list_len(int i){
		return list_len[i];
	}
	string get_param_data(int i){
		return param[i];
	}
	string get_list_data(int i, int j){
		return list[i][j];
	}
	
};

class data_mbti{
private:
	vector<string> list;
	int list_len;

public:
	data_mbti(){
		
		list={"kind", "humourous", "extroverted", "bright", "introverted", "shy", "glum"};
		list_len = list.size();
	}
	int get_list_len(){
		return list_len;
	}
};


class npc{
private:
	vector<string> look;
	vector<string> mbti;
	string image;
	int look_len = 0;
	int mbit_len = 0;
	int love = 50;
	int day = 0;
public:
	
	void set_look(string data){
		look.push_back(data);
		look_len++;
	}
	string get_look(int i){
		return look[i];
	}

	/*set_mbti();	
	set_love();*/
	void add_day(){ day++; }
}
;

class program{
private:
	class data_look data_look;
	class data_mbti data_mbti;

	class npc main_npc;
public:
	void init(){
		cout << "Game Start!" << endl;
		set_main_npc();
		print_main_npc();
	}


	void set_main_npc(){
		cout << "Please Enter Your Ideal Type" << endl;
		for(int i = 0; i< data_look.get_param_len(); i++){
			cout << data_look.get_param_data(i) << endl;
			for(int j = 0; j<data_look.get_list_len(i); j++){
				cout << j+1 << ". " <<  data_look.get_list_data(i,j) << "\t\t\t";
			}
			cout << data_look.get_list_len(i)+1 << ". direct input";
			cout << endl;
			cout << "Your Choice: ";

			int j;
			cin >> j;
			j--;
			if(j > data_look.get_list_len(i)){
				cout << "Invalid Choice, Set default" << endl;
				main_npc.set_look(data_look.get_list_data(i,0));
			}
			else if(j == (data_look.get_list_len(i)) ){
				cout << "Direct Input: ";
				string direct_input;
				cin >> direct_input;	
				main_npc.set_look(direct_input);

			}
			else{
				main_npc.set_look(data_look.get_list_data(i,j));
			}
		}
	
	}
	void print_main_npc(){
		cout << "Printing Main NPC" << endl;
		string ai_cmd = "Colored Anime Character Look Front with ";
		for(int i = 0; i< data_look.get_param_len(); i++){

			//if(i>3) continue;
			ai_cmd += data_look.get_param_data(i);
			ai_cmd += " is ";
			ai_cmd += main_npc.get_look(i);
			ai_cmd += ", ";
		}
		cout << ai_cmd << endl;
/*
		auto image = openai::image().create(R"(
		{
			{"model", "dall-e-3"},
			{"prompt", ai_cmd},
			{"n", 1},
			{"size", "1024x1024"}		
		}
		)"_json);
		std::cout << image.dump(2) << endl;
		*/

		const char * a = ai_cmd.c_str();
		auto image = openai::image().create(
		{
			{"model", "dall-e-3"},
			{"prompt", a},
			{"n", 1},
			{"size", "1024x1024"}		
		}
		);
		std::cout << image["data"][0]["url"] << endl;


	
	}


	void run(){
	
	
	}


};

int main(){
	openai::start();

	class program program;
	program.init();
	program.run();


	return 0;

}
