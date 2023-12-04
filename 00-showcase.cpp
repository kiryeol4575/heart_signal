#include "openai.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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



class npc{
protected:
	string name;
	vector<string> look;
	vector<string> mbti;
	vector<string> like;
	string image[3];
	string note;

	int look_len;
	int mbti_len;
	int like_len;
	int love;
	int day;
	int last_date;
	int seed;

	nlohmann::json ai_input = R"(
    	{
		"model": "dall-e-3",
		"n" : 1,
		"size" : "1024x1024"
    	}	
    	)"_json;
	nlohmann::json ai_output;

public:
	npc(){
		look_len=0;
		mbti_len=0;
		like_len = 0;
		love = 50;
		day = 0;
		last_date = 0;
		seed = 0;
	}

	void set_seed(int _seed){seed  = _seed;}
	int get_seed(){return seed;}

	void set_name(vector<string> name_list){
		name = name_list[rand()%name_list.size()];	
	}

	void set_look(vector<string> look_param_list, vector<string>* look_data_list){
		for(int i = 0; i<look_param_list.size(); i++){
			look.push_back( look_data_list[i][ rand()%look_data_list[i].size() ]);
		}
	}

	void set_mbti(vector<string> mbti_list ){
		for(int i = 0; i<2; i++){
			mbti.push_back( mbti_list[rand()%mbti_list.size()] );
		}
	}
	void set_like(vector<string> like_list){
		for(int i = 0; i<2; i++){
			like.push_back( like_list[rand()%like_list.size()] );
		}
	}
	string get_name(){
		return name;
	}
	string get_look(int i){
		return look[i];
	}
	string get_mbti(int i){
		return mbti[i];
	}
	string get_like(int i){
		return like[i];
	}

	void print_look(vector<string> look_param_list, string gender, string age){
		string sys_cmd;
		cout << "Printing NPC..." << endl;

		for(int i = 0; i<3; i++){

			string ai_cmd = "Shoulder Shot of Colored 2D Anime Character Look Front with ";

			ai_cmd += ("성별: " + gender + ", ");
			ai_cmd += ("나이: " + age + ", ");
			for(int j = 0; j< look_param_list.size(); j++){

				ai_cmd += (look_param_list[j] + ": " + look[j] + ", ");
			}
			if(i==1) ai_cmd += "with fall in love face";
			else if(i==2) ai_cmd += "with angry face";
				
			cout << ai_cmd << endl;

			cout << "Wait for Image Loading..." << endl;
			ai_input["prompt"] = ai_cmd;
			ai_output= openai::image().create(ai_input);
		
			sys_cmd = "wget -O ./src/" + name + to_string(i) + ".png ";
			sys_cmd += nlohmann::to_string(ai_output["data"][0]["url"]);

			system(sys_cmd.c_str());	


			cout << "Image Loding Done " << endl;

			sys_cmd = "tiv -w 100 -h 100 ./src/" + name + to_string(i) + ".png";
			system(sys_cmd.c_str());

			sleep(60);
		}

	}

	void add_day(){ day++; }
	int get_love(){
		return love;	
	}
	void add_love(int i){
		love += i;
	}
	void set_note(){
		note = "참고로 그녀의 성격은 ";
		for(int i = 0; i<2; i++){
			note += (mbti[i]+ ", ");
			note += ", ";
		}
		note += "이다. ";

		note += "그녀는  ";
		for(int i = 0; i<2; i++){
			note += (like[i] + ", ");
		}
		note += "한 남자를 좋아한다. ";
	}
	string get_note(){
		return note;
	
	}


};


class main_npc : public npc{

public:

	void set_name(vector<string> name_list){

		for(int i = 0; i<name_list.size(); i++){
			cout << name_list[i] << ", ";
		}
		cout << "를 제외한 " << endl;

		cout << "상대의 이름을 입력해 주세요: ";
		cin >> name;
	}

	void set_look(vector<string> look_param_list, vector<string>* look_data_list){

		cout << "당신의 이상형을 입력해 주세요. " << endl << endl;
		for(int i = 0; i<look_param_list.size(); i++){


			cout << look_param_list[i] << endl;
			for(int j = 0; j<look_data_list[i].size(); j++){
				cout << j+1 << ". " <<  look_data_list[i][j] << "\t\t\t";
			}
			cout << look_data_list[i].size()+1 << ". 직접 입력" << endl << endl;
			cout << "당신의 취향은?: ";

			int j;
			cin >> j;
			j--;
			if(j > look_data_list[i].size()){
				cout << "Invalid Choice, Set Default" << endl;
				look.push_back( look_data_list[i][0]);
			}
			else if(j == look_data_list[i].size() ){
				cout << "집적 입력: ";
				string direct_input;
				cin >> direct_input;	
				look.push_back( direct_input );

			}
			else{
				look.push_back( look_data_list[i][0]);
			}
			cout << endl;
		}
	}

	void set_mbti(vector<string> mbti_list ){
		cout << "상대의 성격 세가지를 선택해 주세요. " << endl << endl;
		for(int i = 0; i< mbti_list.size(); i++){
			cout << i+1 << ". " <<  mbti_list[i] << "\t\t";
		}
		cout << endl;
		cout << "숫자로 입력해 주세요: ";
		for(int i= 0; i<3; i++){
			int j;
			cin >> j;
			mbti.push_back(mbti_list[j]);
		}
		cout << endl;
	}
	void print_look(vector<string> look_param_list, string gender, string age){


		npc::print_look(look_param_list, gender, age);
		
		cout<< "당신의 이상형이 맞습니까? (Y/N): ";
		string usr_asr;
		cin >> usr_asr;

		if((usr_asr == "N")||(usr_asr == "n")) print_look(look_param_list, gender, age);
		return;

	
	}




};

class user{
private:
	string name;
	string her_gender;
	string age;
public:
	void set(){
		int i;
		cout << "당신의 이름을 입력해 주세요: ";
		cin >> name;

		cout << endl << "당신의 성별을 선택해 주세요: 1. 남성        2. 여성" << endl << "성별: ";

		cin >> i;
		if(i == 1) her_gender = "여성";
		else her_gender = "남성";


		cout << endl << "당신의 나이를 입력해 주세요: ";
		cin >> i;
		cout << endl;
		if(i < 20) age = "어림";
		else if(i>50) age = "늙음";
		else age = "성숙";

	}
	string get_name(){
		return name;
	}
	string get_gender(){
		return her_gender;
	}
	string get_age(){
		return age;
	}



};

class program{
private:
	vector<string> look_param_list;
	vector<string> look_data_list[10];

	vector<string> name_list;
	vector<string> mbti_list;
	vector<string> like_list;

	class main_npc npc_main;
	class npc npc_sub1, npc_sub2;
	class user user;

	//string plot;
	string place;
	string react[3];
	int cur_girl;
	int her_react;
	string prev_asr;
	string prev_quest;
	int day;

       //"model": "gpt-4-1106-preview",
	nlohmann::json ai_input = R"(
    	{
        	"model": "gpt-3.5-turbo",
        	"messages":[{"role":"user"}],
        	"temperature": 1.0
    	}	
    	)"_json;
	nlohmann::json ai_output;

public:

	void init_list(){
	
	        name_list = {"지수", "제니"};
	        mbti_list = {"상냥한", "장난스러운", "활발한", "수줍은", "차가운", "다혈질"};

		like_list = {"다정한", "재미있는", "솔직한", "엉뚱한", "지적인", "차가운"};

		look_param_list = {"몸매", "키", "피부색", "눈모양", "눈색깔", "머리", "머리색깔", "옷"};

		look_data_list[0] = {"표준", "마름", "통통", "글래머"};
		look_data_list[1] ={"표준", "큼", "작음"};
		look_data_list[2] ={"밝음", "어두움"};
		look_data_list[3] ={"둥금", "날카로움" };
		look_data_list[4] ={"검정", "파랑", "갈색", "빨강"};
		look_data_list[5] ={"긴생머리", "긴웨이브", "단발"};
		look_data_list[6] ={"검정", "갈색", "빨강", "금발"};
		look_data_list[7] ={"정장", "코트", "티셔츠", "드레스"};
	}
	program(){

		init_list();	
		
		cur_girl = 1;
		day = 1;
		her_react= -1;
	
	}
	void fix_str(string &str){
		
		string::size_type n;
		while( (n= str.find("\\n")) != string::npos){
			str.erase(n, 2);
		}
		while( (n= str.find('\"')) != string::npos){
			str.erase(n, 1);
		}
		while( (n= str.find('\\')) != string::npos){
			str.erase(n, 1);
		}
	
	
	
	
	}

	void div_str(string str1, string &str2, int num){
		
		string::size_type n;
		if(num == 1) n=str1.find("1.");
		else if(num == 2) n=str1.find("2.");
		else if(num == 3) n=str1.find("3.");
		str2 = str1.substr(n+2);

		if(num ==1){
			n=str2.find("2.");
			str2.erase(str2.begin()+n, str2.end());
		}

		else if(num ==2){
			n=str2.find("3.");
			str2.erase(str2.begin()+n, str2.end());
		}
	

	
	}
	void set_pick(string str, int* arr){
		
		string::size_type n1=str.find('1');
		string::size_type n2=str.find('2');
		string::size_type n3=str.find('3');
		if((n1 == string::npos)||(n2 == string::npos)||(n3==string::npos)){
			arr[0] = 1;
			arr[1] = 0;
			arr[2] = -1;
			return;
		}
		string::size_type best = n1;
		if(n2 < best) best = n2;
		if(n3 < best) best = n3;

		string::size_type worst = n3;
		if(n2 > worst) worst = n2;
		if(n1 > worst) worst = n1;

		string::size_type soso = n2;
		if((n1 != best)&&(n1 != worst)) soso = n1;
		if((n3 != best)&&(n3 != worst)) soso = n3;

		if(n1 == best) arr[0] = 1;
		else if(n1 == soso) arr[0] = 0;
		else if(n1 == worst) arr[0] = -1;

	
		if(n2 == best) arr[1] = 1;
		else if(n2 == soso) arr[1] = 0;
		else if(n2 == worst) arr[1] = -1;

		if(n3 == best) arr[2] = 1;
		else if(n3 == soso) arr[2] = 0;
		else if(n3 == worst) arr[2] = -1;
		return;
	
	}


	void set_npc(npc &npc){
		
		srand(npc.get_seed());
		npc.set_name(name_list);
		npc.set_look(look_param_list, look_data_list);
		npc.print_look(look_param_list, user.get_gender(), user.get_age());
		npc.set_mbti(mbti_list);
		npc.set_like(like_list);
		npc.set_note();
		//npc.set_plot();
	}


	void init(){

		cout << "[-----------------------------하트 시그널------------------------------]" << endl << endl;
		user.set();
		npc_main.set_seed(102);
		npc_sub1.set_seed(100);
		npc_sub2.set_seed(103);

		//set_npc(npc_main);
		//cout << "making sub npc...." << endl;
		//set_npc(npc_sub1);
		set_npc(npc_sub2);
		place = "해운대 바닷가의 어느 카페";
		react[0] = "그녀는 나의 대답이 매우 마음에 들었다. ";
		react[1] = "그녀는 나의 대답이 적당하다 생각했다. ";
		react[2] = "그녀는 나의 대답에 매우 실망했다. ";
		//set_stat();
	
		cout << "[------------------------------설정 완료-------------------------------]" << endl << endl;
	}
/*
	void set_first_plot(){
		plot = "오늘은 군대에서 5개월만에 휴가를 나와, 고등학교 때부터 알고 지내던 그녀와 부산 해운대의 어느 카페에서 만나기로 했다. ";
		place = "부산 해운대의 어느 카페";
	}
	}
*/


	int run_day(){
	
		npc &npc = cur_girl==1 ? npc_main : (cur_girl == 2 ? npc_sub1 : npc_sub2) ;	
	
		her_react=-1;

		for(int i= 0; i<3; i++){
			run_talk(npc);
		}
		end_talk(npc);
		set_girl();
		return 0;
	}

	void set_girl(){
		cout << "내일은 누구와 만나보도록 할까?" <<endl;
		cout << "1. " << npc_main.get_name() << ", 2. " << npc_sub1.get_name() << ", 3. " << npc_sub2.get_name() << endl;
		int i;
		cin >> i;
		cout << "어디서 보자고 연락할까?" << endl << endl;
		cout << "직접 입력: ";
		cin >> place;
	}

	int end_talk(npc &npc){

		string sys_cmd;
		string ai_cmd;
		string choice[3];
		int her_pick[3];
		int my_pick;
		string quest;
		string choices;
		string temp;
		int confess;

		if(her_react != -1) sys_cmd = "tiv -w 100 -h 100 ./src/" + npc.get_name() + to_string(her_react) + ".png";
		else sys_cmd = "tiv -w 100 -h 100 ./src/" + npc.get_name() + to_string(0) + ".png";
		system(sys_cmd.c_str());
		
		if(her_react != -1){
			if(her_react == 1){		
				cout << "그녀는 나의 대답을 좋아하는듯 수줍게 웃었다. " << endl << endl;
			}
			else if(her_react == 0){
				cout << "그녀의 표정에 변함이 없다. " << endl << endl;
			}
			else if(her_react ==2 ){
				cout << "그녀는 나의 대답에 실망하는듯 인상을 찌뿌렸다. " << endl << endl;
			}

			ai_cmd = "나는 " + prev_asr + "라고 답했고, " + react[her_react];

		}
		ai_cmd += npc.get_note();
		ai_cmd += "이제는 갈 시간이 된 것 같다. 그녀가 가기 전에 마지막으로 나에게 할 것 같은 말을 세 문장으로 그녀가 말하는 것처럼 반말로 써줘. 번호는 빼줘. ";

		ai_input["messages"][0]["content"] = ai_cmd;

		ai_output = openai::chat().create(ai_input);
		
		cout << npc.get_name() << "> ";
		
		temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
		
		fix_str(temp);
		cout << temp << endl << endl;


		cout << "이제는 갈 시간이 된 것 같다. 그녀와 헤어지는 길은 항상 아쉬운 것 같다. " << endl << endl;
		cout << npc.get_name() << "> ";
		cout << "혹시 뭐 할 말 있어? " <<endl << endl;

		cout << "오늘이야말로 그녀에게 고백을 할 절호의 기회일까? " << endl<< endl;
		cout << "1. 고백한다. " << endl<< endl;
		cout << "2. 안 한다. " << endl << endl;
		cout << "나의 선택은?: ";
		cin >> confess;

		if(confess == 1){
			ai_cmd = "짝사랑하는 그녀에게 사랑을 고백을 하려고 한다. 내가 그녀에게 사랑을 고백하는 것을 내가 말하는 것처럼 반말로 써줘. 대답의 선택지는 세 가지로 매우 상반되게 해줘. 대답마다 앞에 1. 2. 이런식으로 번호를 붙여줘. ";


			ai_input["messages"][0]["content"] = ai_cmd;

			ai_output = openai::chat().create(ai_input);
			choices = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
			fix_str(choices);
			for(int i=0; i<3; i++){
				div_str(choices, choice[i], i+1);
			}	


			cout << endl << "그녀에게 어떻게 고백할까? " << endl << endl;
			for(int i = 0; i<3; i++){
			
				cout << i+1 << ". ";
				cout << choice[i] << endl << endl;
			}

			ai_cmd = "그녀가 어떤 고백을 좋아할지 순서대로 알려줘. 참고로 나의 고백은 ";
			for(int i = 0; i<3; i++){
				ai_cmd += to_string(i+1) +  ". " + choice[i] + " ";
			}
			ai_cmd += "였어. ";
			ai_cmd += npc.get_note();
			ai_cmd += "그녀가 좋아할 고백의 순서를 321 이런식으로 숫자만 써줘. 숫자말고 다른건 쓰지마. ";
		
			ai_input["messages"][0]["content"] = ai_cmd;
			ai_output = openai::chat().create(ai_input);
			temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
			set_pick(temp, her_pick);
		
			cout <<temp << endl;
			cout << "나의 선택은?: ";
			cin >> my_pick;
		
			if(her_pick[my_pick-1] == 1){
				npc.add_love(30);
				her_react = 0;		
			}
			else if(her_pick[my_pick-1] == 0){
				her_react = 1;
		
			}
			else if(her_pick[my_pick-1] == -1){
				npc.add_love(-30);
				her_react = 2;
			}		
			prev_asr = choice[my_pick-1];


			if(npc.get_love() < 100){
				ai_cmd = "내가 고백을 했지만 그녀는 나를 좋아하진 않는 것 같다. 나의 고백 멘트는 ";
				ai_cmd += prev_asr + "였다. " + "그녀가 나의 고백을 거절할 말을 세 문장으로 그녀가 말하는 것처럼 반말로 써줘. 번호는 빼줘. ";
			
				ai_input["messages"][0]["content"] = ai_cmd;
				ai_output = openai::chat().create(ai_input);
				temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
				fix_str(temp);
				cout << npc.get_name() << "> ";


				cout <<temp << endl << endl;
				cout << "오랫동안 그녀에게 결국 차여버렸다. 더이상 살고싶지가 않다..." << endl;
				return -1;
			}

			else if(npc.get_love() >= 100){
				ai_cmd = "내가 고백을 했는데 그녀도 나를 좋아하는 것 같다. 나의 고백 멘트는 ";
				ai_cmd += prev_asr + "였다. ";
				ai_cmd += "그녀가 나의 고백을 받아주는 말을 세 문장으로 그녀가 말하는 것처럼 반말로 써줘. 번호는 빼줘. ";
			
				ai_input["messages"][0]["content"] = ai_cmd;
				ai_output = openai::chat().create(ai_input);
				temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
				fix_str(temp);
				cout << npc.get_name() << "> ";
				cout <<temp << endl << endl;
				cout << "오랫동안 그녀와 결국 사귀게 되었다. 꿈만 같다. " << endl;
				return 1;
			}
		}
		else{
			ai_cmd = "다음에 그녀와 만났을 때의 첫 상황을 한 문장으로 만들어줘.";

			
			ai_input["messages"][0]["content"] = ai_cmd;
			ai_output = openai::chat().create(ai_input);
			temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
			fix_str(temp);
		}
		


		her_react = -1;
		return 0;

	}


	void run_talk(npc& npc){
	
		string sys_cmd;
		string ai_cmd;
		string choice[3];
		int her_pick[3];
		int my_pick;
		string quest;
		string choices;
		string temp;

		if(her_react != -1) sys_cmd = "tiv -w 100 -h 100 ./src/" + npc.get_name() + to_string(her_react) + ".png";
		else sys_cmd = "tiv -w 100 -h 100 ./src/" + npc.get_name() + to_string(0) + ".png";

		system(sys_cmd.c_str());
		
		if(her_react != -1){
			if(her_react == 1){		
				cout << "그녀는 나의 대답을 좋아하는듯 수줍게 웃었다. " << endl << endl;
			}
			else if(her_react == 0){
				cout << "그녀의 표정에 변함이 없다. " << endl << endl;
			}
			else if(her_react == 2 ){
				cout << "그녀는 나의 대답에 실망하는듯 인상을 찌뿌렸다. " << endl << endl;
			}

			ai_cmd = "나는 " + prev_asr + "라고 답했고, " + react[her_react];

		}
		else{

			cout<< "[------------------------------DAY " << day << "------------------------------]" << endl;
			cout << "오늘은 " << npc.get_name() << "와 " << place << "에서 만나기로 했다. " << endl;
			ai_cmd= "오늘은 " + npc.get_name() + "와 " + place + "에서 만나기로 했다. ";
		}


		ai_cmd += npc.get_note();
		ai_cmd += "그녀가 나에게 할 것 같은 말을 세 문장으로 그녀가 말하는 것처럼 반말로 써줘. 세번 째 문장은 질문으로 해줘. 번호는 빼줘. ";


		ai_input["messages"][0]["content"] = ai_cmd;


		ai_output = openai::chat().create(ai_input);
		
		cout << npc.get_name() << "> ";
		
		quest = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
		
		fix_str(quest);

		cout << quest << endl << endl;

		ai_cmd = npc.get_note() + "그녀는 " + quest;
		ai_cmd += "라고 질문을 했어. 내가 할 대답을 내가 말하는 것처럼 반말로 써줘. 대답의 선택지는 세 가지로 매우 상반되게 해줘. 대답마다 앞에 1. 2. 이런식으로 번호를 붙여줘. ";


		ai_input["messages"][0]["content"] = ai_cmd;

		ai_output = openai::chat().create(ai_input);
		choices = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
		fix_str(choices);
		for(int i=0; i<3; i++){
			div_str(choices, choice[i], i+1);
		}


		cout << "그녀에게 어떻게 대답할까? " << endl << endl;
		for(int i = 0; i<3; i++){
			
			cout << i+1 << ". ";
			cout << choice[i] << endl << endl;
		}

		ai_cmd = "그녀가 어떤 대답을 좋아할지 순서대로 알려줘. 참고로 나의 대답은 ";
		for(int i = 0; i<3; i++){
			ai_cmd += choices;
		}
		ai_cmd += "였어. " + npc.get_note() + "그녀가 좋아할 대답의 순서를 321 이런식으로 숫자만 써줘. 숫자말고 다른건 쓰지마. ";
		//cout << ai_cmd;
		

		ai_input["messages"][0]["content"] = ai_cmd;
		ai_output = openai::chat().create(ai_input);
		temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
		cout <<temp << endl;

		set_pick(temp, her_pick);
		
		cout << "나의 대답은?: ";
		fflush(stdin);
		cin >> my_pick;
		if((my_pick < 1)&&(my_pick > 3)){
			cout << "Invalid Choice!, 1 Picked" << endl << endl;
		
		}
		my_pick=1;
		
		if(her_pick[my_pick-1] == 1){
			npc.add_love(10);
			her_react = 1;		
		}
		else if(her_pick[my_pick-1] == 0){
			her_react = 0;
		
		}
		else if(her_pick[my_pick-1] == -1){
			npc.add_love(-10);
			her_react = 2;
		
		}
		
		prev_asr = choice[my_pick-1];


	}


};

int main(){
	system("export OPENAI_API_KEY=\'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\'");
	openai::start();

	class program program;
	program.init();
	while(program.run_day() == 0);


	return 0;

}
