#include "openai.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

enum _look{
	GENDER,
	AGE,
	HEIGHT,
	BODY,
	SKIN_COLOR,
	EYE_SHAPE,
	EYE_COLOR,
	HAIR,
	HAIR_COLOR,
	FASHION,
};

class data_look{
private:
	vector<string> param;
	vector<vector<string>> list;
	int param_len;
	vector<int> list_len;
public:


	data_look(){
		param = {"gender", "age", "body", "height", "skin color", "eye shape", "eye color", "hair", "hair color", "fashion"};
		list.push_back({"female", "male"});
		list.push_back({"young", "mature", "old"});
		list.push_back({"standard", "slender", "plump", "hot"});
		list.push_back({"standard", "tall", "short"});
		list.push_back({"bright", "dark"});
		list.push_back({"round", "sharp" });
		list.push_back({"black", "blue", "brown", "red"});
		list.push_back({"long straight", "long permed", "short"});
		list.push_back({"black", "brown", "red", "blonde"});
		list.push_back({"suit", "coat", "T-shirt", "dress"});
		
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
	string get_list_data(int i){
		return list[i];
	
	}
};


class npc{
private:
	string name;
	vector<string> look;
	vector<string> mbti;
	string image;

	int look_len = 0;
	int mbti_len = 0;
	int love = 50;
	int day = 0;
	int last_date = 0;

public:
	void set_name(string data){
		name = data;
	}
	string get_name(){
		return name;
	}

	void set_look(string data){
		look.push_back(data);
		look_len++;
	}
	string get_look(int i){
		return look[i];
	}

	void set_mbti(string data){
		mbti.push_back(data);
		mbti_len++;
	}
	string get_mbti(int i){
		return mbti[i];
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
	string plot;
	string note;
	string place;

	int day = 1;



public:
	void init(){
		cout << "Game Start!" << endl << endl;
		set_look_main_npc();
		while(!print_main_npc());
		set_mbti_main_npc();
		set_name_main_npc();
		set_first_plot();
	}


	void set_look_main_npc(){
		cout << "Please Enter Your Ideal Type" << endl << endl;
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
				cout << "Invalid Choice, Set Default" << endl;
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

			cout << endl;
		}
	
	}
	bool print_main_npc(){
		cout << "Printing Main NPC..." << endl;
		/*string ai_cmd = "Shoulder Shot of Colored 2D Anime Character Look Front with ";
		for(int i = 0; i< data_look.get_param_len(); i++){

			ai_cmd += data_look.get_param_data(i);
			ai_cmd += " is ";
			ai_cmd += main_npc.get_look(i);
			ai_cmd += ", ";
		}
		cout << ai_cmd << endl;

		cout << "Wait for Image Loading..." << endl;
		auto image = openai::image().create(
		{
			{"model", "dall-e-3"},
			{"prompt", ai_cmd},
			{"n", 1},
			{"size", "1024x1024"}		
		}
		);

	        FILE *stream1 = freopen("./main_npc.sh", "w", stdout);
		cout<< "wget -O ./src/main_npc.png ";
    		cout << image["data"][0]["url"];
		
		fflush(stream1);
		fclose(stream1);
		freopen("/dev/tty", "w", stdout);
		fflush(stdout);

		system("sh ./main_npc.sh");	
*/


		cout << "Image Loding Done " << endl;
		string sys_cmd = "tiv -w 100 -h 100 ./src/main_npc.png";
		system(sys_cmd.c_str());
		
		cout<< "Is That Your Ideal Type? (Y/N): ";
		string usr_asr;
		cin >> usr_asr;

		if((usr_asr == "Y")||(usr_asr == "y")) return true;
		else return false;

	}
	void set_name_main_npc(){
		cout << "Please Type Her Name: ";
		string name;
		cin >> name;
		main_npc.set_name(name);
	}
	void set_mbti_main_npc(){
		cout << "Please Choose Her 3 Personalities" << endl << endl;
		for(int i = 0; i< data_mbti.get_list_len(); i++){
			cout << i+1 << ". " <<  data_mbti.get_list_data(i) << "\t\t";
		}
		cout << endl;
		for(int i= 0; i<3; i++){
			int mbti;
			cin >> mbti;
			main_npc.set_mbti(data_mbti.get_list_data(i));
		}
	
	}
	void set_first_plot(){
		plot = "오늘은 군대에서 5개월만에 휴가를 나와, 고등학교 때부터 알고 지내던  그녀와 부산 해운대의 어느 카페에서 만나기로 했다. ";
		place = "부산 해운대의 어느 카페";
	}


	void run(){
	
		nlohmann::json input;
		string sys_cmd;
		string ai_cmd;

		sys_cmd = "tiv -w 100 -h 100 ./src/main_npc.png";
		system(sys_cmd.c_str());
		
		cout<< "[--------------------DAY " << day << "--------------------]" << endl;
		cout << plot << endl << endl;
		note = "참고로 그녀의 성격은 ";
		for(int i = 0; i<3; i++){
			note += main_npc.get_mbti(i);
			note += ", ";
		}
		note += "이다. ";
		
		ai_cmd = plot+note;
		ai_cmd += "그녀가 나에게 할 것 같은 말을 세 문장으로 대신 해주는데, 마지막 문장은 질문으로 해줘. ";

		cout << ai_cmd << endl;
   		input = R"(
    		{
        		"model": "gpt-3.5-turbo",
        		"messages":[{"role":"user"}],
        		"temperature": 1
    		}
    		)"_json;


		input["messages"][0]["content"] = ai_cmd;
		
		auto chat = openai::chat().create(input);
		
		cout << main_npc.get_name() << "> ";
    		std::cout <<  chat["choices"][0]["message"]["content"] << endl;

		
		ai_cmd = note;
		ai_cmd += "그녀에게 내가 할 수 있는 대답을 대신 해줘. ";

   		input = R"(
    		{
        		"model": "gpt-3.5-turbo",
        		"messages":[{"role":"user"}],
        		"temperature": 1
			"n" : 3
    		}
    		)"_json;


		input["messages"][0]["content"] = ai_cmd;
		
		auto chat = openai::chat().create(input);
		cout << "그녀에게 어떻게 대답할까? " << endl;
		for(int i = 0; i<3; i++){
			
			cout << i << ". ";
    			std::cout <<  chat["choices"][0]["message"]["content"] << endl;
		}
		
		


		
	}


};

int main(){
	system("export OPENAI_API_KEY=\'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\'");
	openai::start();

	class program program;
	program.init();
	program.run();


	return 0;

}
