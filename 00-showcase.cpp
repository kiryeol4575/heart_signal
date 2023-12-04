#include "openai.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

using namespace std;


class npc{
protected:
	string name;
	vector<string> look;
	vector<string> mbti;
	vector<string> like;
	string note;
	string relation;

	int look_len;
	int mbti_len;
	int like_len;
	int love;
	int seed;
	int gone;


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
		seed = 0;
		gone = 0;
	}

	void set_seed(int _seed){seed  = _seed;}

	virtual void set_name(vector<string> name_list){
		name = name_list[rand()%name_list.size()];	
	}
	virtual void set_relation(vector<string> relation_list){
		relation = relation_list[rand()%relation_list.size()];	
	}
	virtual void set_look(vector<string> look_param_list, vector<string>* look_data_list){
	}

	virtual void set_mbti(vector<string> mbti_list ){
		for(int i = 0; i<2; i++){
			mbti.push_back( mbti_list[rand()%mbti_list.size()] );
		}
	}
	virtual void set_like(vector<string> like_list){
		for(int i = 0; i<2; i++){
			like.push_back( like_list[rand()%like_list.size()] );
		}
	}
	void add_love(int i){
		love += i;
	}
	void set_note(){
		note = "그녀는 내 " + relation + "이다. ";
		note += "참고로 그녀의 성격은 ";
		for(int i = 0; i<2; i++){
			note += (mbti[i]+ ", ");
			note += ", ";
		}
		note += "이다. ";

		if(love <= 0){
			note += "그녀는 나를 매우 싫어하는 것 같다. ";
		
		}
		else if(love < 25){
			note += "그녀는 나를 남자로 생각하지 않는다. ";
		
		}
		else if(love > 75){
			note += "그녀는 나에게 끌리는 것 같다. ";
		}
		else if(love >= 100){
			note += "그녀는 나를 매우 사랑하는 것 같다. ";
		}
	}
	void set_gone(){
		gone = 1;
	}

	int get_seed(){return seed;}
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

	string get_relation(){
		return relation;
	}

	int get_love(){
		return love;	
	}
	string get_note(){
		return note;
	
	}
	int get_gone(){
		return gone;	
	}

	virtual void print_look(vector<string> look_param_list, string gender, string age, int i){


	}
};


class main_npc : public npc{

public:

	virtual void set_name(vector<string> name_list){

		for(int i = 0; i<(int)name_list.size(); i++){
			cout << name_list[i] << ", ";
		}
		cout << "를 제외한 " << endl;

		cout << "상대의 이름을 입력해 주세요: ";
		cin >> name;
		cout << endl;
	}

	virtual void set_look(vector<string> look_param_list, vector<string>* look_data_list){

		cout << "당신의 이상형을 입력해 주세요. " << endl << endl;
		for(int i = 0; i<(int)look_param_list.size(); i++){


			cout << look_param_list[i] << endl;
			for(int j = 0; j<(int)look_data_list[i].size(); j++){
				cout << j+1 << ". " <<  look_data_list[i][j] << "\t\t\t";
			}
			cout << look_data_list[i].size()+1 << ". 직접 입력" << endl << endl;
			cout << "당신의 취향은?: ";

			int j;
			cin >> j;
			j--;
			if(j > (int)look_data_list[i].size()){
				cout << "Invalid Choice, Set Default" << endl;
				look.push_back( look_data_list[i][0]);
			}
			else if(j == (int)look_data_list[i].size() ){
				cout << "집적 입력: ";
				string direct_input;

				cin.ignore();
				getline(cin, direct_input);	
				look.push_back( direct_input );

			}
			else{
				look.push_back( look_data_list[i][j]);
			}
			cout << endl;
		}
	}

	virtual void set_mbti(vector<string> mbti_list ){
		cout << endl << "상대의 성격 두가지를 선택해 주세요. " << endl << endl;
		for(int i = 0; i< (int)mbti_list.size(); i++){
			cout << i+1 << ". " <<  mbti_list[i] << "\t\t";
		}
		cout << endl;
		cout << "숫자로 입력해 주세요: ";
		for(int i= 0; i<2; i++){
			int j;
			cin >> j;
			j--;
			mbti.push_back(mbti_list[j]);
		}
		cout << endl;
	}
	virtual void set_relation(vector<string> name_list){

		cout << "상대방과의 관계를 입력해 주세요: ";
		cin >> relation;
	}
	virtual void print_look(vector<string> look_param_list, string gender, string age, int i){


		string sys_cmd;
		cout << "Printing NPC..." << endl;

		string ai_cmd = "정면을 향해 보고 있는, 상반신 사진, 2D 일본 애니메이션 캐릭터 작화, ";

		ai_cmd += ("성별: " + gender + ", ");
		ai_cmd += ("나이: " + age + ", ");
		for(int j = 0; j< (int)look_param_list.size(); j++){

			ai_cmd += (look_param_list[j] + ": " + look[j] + ", ");
		}
		if(i==1) ai_cmd += "눈웃음 짓는 표정";
		else if(i==2) ai_cmd += "화난 표정";
				

		cout << "Wait for Image Loading..." << endl;
		ai_input["prompt"] = ai_cmd;
		ai_output= openai::image().create(ai_input);
		
		sys_cmd = "wget -O ./heart_signal_src/" + name + to_string(i) + ".png ";
		sys_cmd += nlohmann::to_string(ai_output["data"][0]["url"]);

		system(sys_cmd.c_str());	


		cout << "Image Loading Done " << endl;

		sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + name + to_string(i) + ".png";
		system(sys_cmd.c_str());

		npc::print_look(look_param_list, gender, age, i);
		
		if(i == 0) cout<< "당신의 이상형이 맞습니까? (Y/N): ";
		if(i == 1) cout<< "당신의 이상형의 웃는 얼굴이 맞습니까? (Y/N): ";
		if(i == 2) cout<< "당신의 이상형의 화난 얼굴이 맞습니까? (Y/N): ";
		string usr_asr;
		cin >> usr_asr;
		cout << endl;
		if((usr_asr == "N")||(usr_asr == "n")) print_look(look_param_list, gender, age, i);
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
	bool saved;
	
	vector<string> look_param_list;
	vector<string> look_data_list[10];

	vector<string> name_list;
	vector<string> mbti_list;
	vector<string> like_list;
	vector<string> relation_list;

	class main_npc npc_main;
	class npc npc_sub1, npc_sub2;
	class user user;

	string place;
	string react[3];
	int cur_girl;
	int her_react;
	string prev_asr;
	int day;

        //"model": "gpt-3.5-turbo",
	nlohmann::json ai_input = R"(
    	{
        	"model": "gpt-4-1106-preview",
        	"messages":[{"role":"user"}],
        	"temperature": 1.0
    	}	
    	)"_json;
	nlohmann::json ai_output;

public:

	void init_list(){
	
	        name_list = {"지수", "제니"};
	        mbti_list = {"상냥한", "장난스러운", "활발한", "수줍은", "차가운", "다혈질", "진지한", "엉뚱한", "지적인", "솔직한"};
		like_list = mbti_list;
		relation_list = {"고등학교 동창", "소꿉친구", "소개팅에서 만난 사람", "전애인", "같은 동아리 사람", "대학교 선배", "첫사랑", "친구의 친구"};

		look_param_list = {"키", "몸매", "피부색", "눈모양", "눈색깔", "머리", "머리색깔", "옷"};

		look_data_list[0] ={"표준", "큼", "작음"};
		look_data_list[1] = {"표준", "마름", "통통", "글래머"};
		look_data_list[2] ={"밝음", "어두움"};
		look_data_list[3] ={"둥금", "날카로움" };
		look_data_list[4] ={"검정", "파랑", "갈색", "빨강"};
		look_data_list[5] ={"긴생머리", "긴웨이브", "단발"};
		look_data_list[6] ={"검정", "갈색", "빨강", "금발"};
		look_data_list[7] ={"정장", "코트", "티셔츠", "드레스"};
	}
	program(bool _saved){
		saved = _saved;

		init_list();	

		cur_girl = 1;
		day = 1;
		her_react= -1;

		place = "해운대 바닷가의 어느 카페";
		react[0] = "그녀는 나의 대답이 적당하다 생각했다. ";
		react[1] = "그녀는 나의 대답이 매우 마음에 들었다. ";
		react[2] = "그녀는 나의 대답에 매우 실망했다. ";
	
	}
	void fix_str(string &str){
		
		string::size_type n;
		while( (n= str.find("\\n")) != string::npos){
			str.erase(n, 2);
			str.insert(n, " ");
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
		if(!saved) npc.set_look(look_param_list, look_data_list);
		if(!saved) for(int i=0; i<3; i++) npc.print_look(look_param_list, user.get_gender(), user.get_age(), i);
		npc.set_mbti(mbti_list);
		npc.set_like(like_list);
		npc.set_relation(relation_list);
	}


	void init_npc(){

		cout << "[-----------------------------하트 시그널------------------------------]" << endl << endl;
		cout << "입력창이 뜨기 전까지 입력을 기다려 주십시오. AI가 답변을 만드는데 시간이 걸립니다. 모든 문자열 입력은 띄어쓰기 없이 입력해 주십시오." << endl << endl;
		user.set();
		npc_main.set_seed(time(NULL));
		npc_sub1.set_seed(100);
		npc_sub2.set_seed(103);

		set_npc(npc_main);
		cout << "Making Sub Npc...." << endl;

		set_npc(npc_sub1);
		set_npc(npc_sub2);
	
		cout << "[------------------------------설정 완료-------------------------------]" << endl << endl;
	}


	int run_day(){


	
		npc &npc = cur_girl==1 ? npc_main : (cur_girl == 2 ? npc_sub1 : npc_sub2) ;	
	
		her_react=-1;

		for(int i= 0; i<3; i++){
			npc.set_note();
			run_talk(npc);
		}
		npc.set_note();
		if(end_talk(npc) == 1){
			cout << "[------------------------------ENDING-------------------------------]" << endl << endl;
		       	return 1;
		}
		if(npc_main.get_gone() && npc_sub1.get_gone() && npc_sub2.get_gone()){
			cout << "이젠 아무도 나와 만나주지 않는다... 어디서부터 잘못된걸까?" <<endl << endl;
			cout << "[------------------------------GAME OVER-------------------------------]" << endl << endl;
			return -1;
		}
		cout<< "[------------------------------DAY END------------------------------]" << endl << endl;
		set_girl();
		day++;
		return 0;
	}

	void set_girl(){
		cout << "내일은 누구와 만나보도록 할까?" <<endl << endl;
		cout << "1. " << npc_main.get_name() << "        2. " << npc_sub1.get_name() << "        3. " << npc_sub2.get_name() << endl << endl;
		
		cout << "나의 선택은?: ";
		cin >> cur_girl;
		cout <<endl;

		npc &npc = cur_girl==1 ? npc_main : (cur_girl == 2 ? npc_sub1 : npc_sub2) ;	
		if(npc.get_gone()){
			cout << npc.get_name() << "와는 더이상 연락이 되지 않는다... 다른 사람을 찾아보자..." <<endl << endl;
			set_girl();
			return;
		}
			
		cout << "어디서 보자고 연락할까?" << endl << endl;
		cout << "직접 입력: ";
		cin.ignore();
		getline(cin, place);
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

		if(her_react != -1) sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(her_react) + ".png";
		else sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(0) + ".png";
		system(sys_cmd.c_str());
		
		if(her_react != -1){
			cout << user.get_name() << "> " << prev_asr << endl << endl;

			if(her_react == 1){		
				cout << "그녀는 나의 대답을 좋아하는듯 수줍게 웃었다. " << endl << endl;
			}
			else if(her_react == 0){
				cout << "그녀는 나의 대답이 썩 마음에 들었다. " << endl << endl;
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


		if(npc.get_love() < 0){
			ai_cmd = "그녀는 나에게 정이 다 떨어졌다. 그녀는 더이상 나를 만나지 않으려 한다. 그녀가 나에게 할 말을 그녀가 말하는 것처럼 반말로 써줘.";
			ai_cmd += npc.get_note();
			
			ai_input["messages"][0]["content"] = ai_cmd;
			ai_output = openai::chat().create(ai_input);
			temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
			fix_str(temp);

			sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(2) + ".png";
			system(sys_cmd.c_str());
			cout << npc.get_name() << "> ";
			cout << temp << endl << endl;
			npc.set_gone();
			cout << "그녀는 그렇게 차갑게 돌아섰다. 다신 그녀와 만날 수 없게 되었다." << endl << endl;
			return -1;
		}
		
		
		cout << endl << endl << "이제는 갈 시간이 된 것 같다. 그녀와 헤어지는 길은 항상 아쉬운 것 같다. " << endl << endl;
		cout << npc.get_name() << "> ";
		cout << "혹시 뭐 할 말 있어? " <<endl << endl;

		cout << "오늘이야말로 그녀에게 고백을 할 절호의 기회일까? " << endl<< endl;
		cout << "1. 고백한다. " << endl<< endl;
		cout << "2. 안 한다. " << endl << endl;
		cout << "나의 선택은?: ";
		cin >> confess;
		cout << endl;


		if(confess == 1){
			ai_cmd = "그녀에게 사랑 고백을 하려고 한다. 내가 그녀에게 사랑 고백하는 것을 내가 말하는 것처럼 반말로 써줘. 멘트의 선택지는 세 가지로 매우 상반되게 해줘. 멘트마다 앞에 1. 2. 이런식으로 번호를 붙여줘. ";

			ai_cmd += "그녀는 내 " + npc.get_relation() + "이다. ";

			ai_input["messages"][0]["content"] = ai_cmd;

			ai_output = openai::chat().create(ai_input);
			choices = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
			fix_str(choices);
			for(int i=0; i<3; i++){
				div_str(choices, choice[i], i+1);
			}	


			cout << "그녀에게 어떻게 고백할까? " << endl << endl;
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
			ai_cmd += "그녀는  ";
			for(int i = 0; i<2; i++){
				ai_cmd += (npc.get_like(i) + ", ");
			}
			ai_cmd += "한 남자를 좋아한다. ";


			ai_cmd += "그녀가 좋아할 고백의 순서를 321 이런식으로 숫자만 써줘. 숫자말고 다른건 쓰지마. ";
		
			ai_input["messages"][0]["content"] = ai_cmd;
			ai_output = openai::chat().create(ai_input);
			temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
			set_pick(temp, her_pick);
		
			cout << "나의 선택은?: ";
			cin >> my_pick;
			cout << endl;
			prev_asr = choice[my_pick-1];


			if(her_pick[my_pick-1] == 1){
				npc.add_love(30);
				her_react = 1;		
			}
			else if(her_pick[my_pick-1] == 0){
				npc.add_love(15);
				her_react = 0;
		
			}
			else if(her_pick[my_pick-1] == -1){
				npc.add_love(-30);
				her_react = 2;
			}		


			if(npc.get_love() < 100){
				sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(2) + ".png";
				system(sys_cmd.c_str());
				cout << user.get_name() << "> " << prev_asr << endl <<endl;
	
				ai_cmd = "내가 고백을 했지만 그녀는 나를 좋아하진 않는 것 같다. 나의 고백 멘트는 ";
				ai_cmd += prev_asr + "였다. " + "그녀가 나의 고백을 거절할 말을 세 문장으로 그녀가 말하는 것처럼 반말로 써줘. 번호는 빼줘. ";
				ai_cmd += npc.get_note();

				ai_input["messages"][0]["content"] = ai_cmd;
				ai_output = openai::chat().create(ai_input);
				temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
				fix_str(temp);
				cout << npc.get_name() << "> ";


				cout <<temp << endl << endl;
				cout << "그녀에게 결국 차여버렸다. 너무 슬픈 하루다..." << endl << endl;
				npc.set_gone();
				return -1;
			}

			else if(npc.get_love() >= 100){
				sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(1) + ".png";
				system(sys_cmd.c_str());
				cout << user.get_name() << "> " << prev_asr << endl <<endl;

				ai_cmd = "내가 고백을 했는데 그녀도 나를 좋아하는 것 같다. 나의 고백 멘트는 ";
				ai_cmd += prev_asr + "였다. ";
				ai_cmd += "그녀가 나의 고백을 받아주는 말을 세 문장으로 그녀가 말하는 것처럼 반말로 써줘. 번호는 빼줘. ";
				ai_cmd += npc.get_note();

				ai_input["messages"][0]["content"] = ai_cmd;
				ai_output = openai::chat().create(ai_input);
				temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
				fix_str(temp);
				cout << npc.get_name() << "> ";
				cout <<temp << endl << endl;
				cout << "오랫동안 좋아했던 그녀와 결국 사귀게 되었다. 꿈만 같다. " << endl << endl;

				return 1;
			}
		}
		else{
			if(npc.get_love() > 150){
				sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(2) + ".png";
				system(sys_cmd.c_str());

				ai_cmd = "그녀는 애만 태우고 고백을 하지 않는 나에게 지쳐버렸다. 그녀는 더이상 나를 만나지 않으려 한다. 그녀가 나에게 할 말을 그녀가 말하는 것처럼 반말로 써줘.";
				ai_cmd += npc.get_note();
			
				ai_input["messages"][0]["content"] = ai_cmd;
				ai_output = openai::chat().create(ai_input);
				temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);
				fix_str(temp);
				cout << npc.get_name() << "> ";
				cout << temp << endl << endl;
				npc.set_gone();
				cout << "그녀는 그렇게 차갑게 돌아섰다. 다신 그녀와 만날 수 없게 되었다." << endl << endl;
				return -1;
			}
		}
		


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

		if(her_react != -1) sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(her_react) + ".png";
		else sys_cmd = "tiv -w 100 -h 100 ./heart_signal_src/" + npc.get_name() + to_string(0) + ".png";

		system(sys_cmd.c_str());
		
		if(her_react != -1){
			cout << user.get_name() << "> " << prev_asr << endl << endl;

			if(her_react == 1){		
				cout << "그녀는 나의 대답을 좋아하는듯 수줍게 웃었다. " << endl << endl;
			}
			else if(her_react == 0){
				cout << "그녀는 나의 대답이 썩 마음에 들었다. " << endl << endl;
			}
			else if(her_react == 2 ){
				cout << "그녀는 나의 대답에 실망하는듯 인상을 찌뿌렸다. " << endl << endl;
			}

			ai_cmd = "나는 " + prev_asr + "라고 답했고, " + react[her_react];

		}
		else{

			cout<< "[------------------------------DAY " << day << "------------------------------]" << endl;
			cout << "오늘은 " << npc.get_name() << "와 " << place << "에서 만나기로 했다. " << endl;
			cout << "그녀는 내 " << npc.get_relation() << "이다. " << endl << endl;
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
		ai_cmd += "였어. " + npc.get_note();
		ai_cmd += "그녀는  ";
		for(int i = 0; i<2; i++){
			ai_cmd += (npc.get_like(i) + ", ");
		}
		ai_cmd += "한 남자를 좋아한다. ";
	
		ai_cmd += "그녀가 좋아할 대답의 순서를 321 이런식으로 숫자만 써줘. 숫자말고 다른건 쓰지마. ";
		

		ai_input["messages"][0]["content"] = ai_cmd;
		ai_output = openai::chat().create(ai_input);
		temp = nlohmann::to_string(ai_output["choices"][0]["message"]["content"]);

		set_pick(temp, her_pick);
		
		cout << "나의 대답은?: ";
		cin >> my_pick;
		if((my_pick < 1)&&(my_pick > 3)){
			cout << "Invalid Choice!, 1 Picked" << endl << endl;
			my_pick=1;
		
		}
		

		if(her_pick[my_pick-1] == 1){
			npc.add_love(10);
			her_react = 1;		
		}
		else if(her_pick[my_pick-1] == 0){
			npc.add_love(5);
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
	openai::start();
	
	bool saved;
	string asr;
	cout << "기존의 이미지 파일을 사용하시겠습니까? 소스 디렉토리에 메인 NPC와 서브 NPC의 이름에 해당하는 파일이 있어야 합니다.(Y/N): ";
	cin >> asr;
	if(!(asr == "Y" || asr == "y")){
		system("rm -rf ./heart_signal_src");
		system("git clone https://github.com/kiryeol4575/heart_signal_src.git");
		saved = false;
	}
	else saved = true;

	class program program(saved);
	program.init_npc();
	while(program.run_day() == 0);


	return 0;

}
