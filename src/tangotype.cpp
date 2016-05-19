//タイピング練習しつつ単語覚えよう/*{{{*/
//2016.03.24 ver. 1.0 writen by Noriaki Machinaka/*}}}*/
#include <ros/ros.h>/*{{{*/
#include <iostream>
#include <string>
#include <cstdlib>  //乱数のために必要
#include <ctime>    //乱数のために必要
#include <fstream> //ファイル操作に必要
#include <vector>

using namespace std;/*}}}*/

namespace tango_type{

	class Duo/*{{{*/
	{
		vector<string> eigo;
		vector<string> nihongo;
		int torokusu;
		int typesu;//入力文字数
		int pinpon;//正答数
		public:
		Duo():typesu(0),pinpon(0){};
		int get_tourokusu() const{ return torokusu; }
		int input();
		void shuffle();
		void cntdwn();
		int show(int,int&);
		int cntnue(int);
		void spin();
	};/*}}}*/

	int Duo::input()/*{{{*/
	{
		int tourokusu=0;
		string tmp[10000][2];

		ifstream f("/home/amsl/ros_catkin_ws/src/asobi/src/duotan.txt");
		if (f.fail()){
			cout<<"err"<<endl;
		}
		while(getline(f,tmp[tourokusu][0],','))
		{
			getline(f,tmp[tourokusu][1]);
			tourokusu++;
		}
		for(int i=0;i<tourokusu;i++){
			eigo.push_back(tmp[i][0]);
			nihongo.push_back(tmp[i][1]);
		}
		torokusu = tourokusu;
		return tourokusu;
	}/*}}}*/

	void Duo::shuffle()/*{{{*/
	{
		srand((unsigned int)time(NULL));
		rand();rand();
		for(int i=0;i<torokusu;i++){
			int j = rand()%torokusu;
			string t[2];
			t[0] = eigo[i];
			t[1] = nihongo[i];
			eigo[i] = eigo[j];
			nihongo[i] = nihongo[j];
			eigo[j] = t[0];
			nihongo[j] = t[1];
		}
	}/*}}}*/

	void Duo::cntdwn()/*{{{*/
	{
		cout<<"準備はいいですか(Etnerキーを押してください)";
		getchar();
		//	cin.ignore();

		for(int i=0;i<3;i++){
			cout<<"=============================="<<endl<<endl;
			cout<<"              "<<3-i<<endl<<endl;
			cout<<"=============================="<<endl<<endl;
			sleep(1);
			cout<<"\033[A";
			cout<<"\033[A";
			cout<<"\033[A";
			cout<<"\033[A";
			cout<<"\033[A";
			cout<<"\033[A";
		}
		for(int i=0;i<6;i++){
			cout<<"                                      "<<endl;
		}
		cout<<"\033[A";
		cout<<"\033[A";
		cout<<"\033[A";
		cout<<"\033[A";
		cout<<"\033[A";
		cout<<"\033[A";

		sleep(0.5);
	}/*}}}*/

	int Duo::show(int i, int &j)/*{{{*/
	{
		char bs=8;
		//	cout <<"111"<<bs<<bs<<bs<<"000";
		string kaito;
		int gosuu=0;
		int gscnt=1;
		while(gscnt){
			gscnt--;
			gscnt=(int)eigo[i].find(" ",gscnt+1);
			gscnt++;
			gosuu++;
		}
		if(j>=eigo[i].length()){
			cout<<nihongo[i]<<":";
			cout<<"【"<<eigo[i]<<"】"<<endl;
		}else{
			cout<<nihongo[i];
			if(gosuu>1){
				cout<<"["<<gosuu<<"語"<<"]";
			}
			cout<<":";
			for(int k=0;k<j;k++){
				cout<<eigo[i][k];
			}
			cout<<"~ "<<endl;
		}	
		cout<<"                              ";
		cout<<"\033[2K";
		getline(cin,kaito);
		if(kaito.length()<1){
			cout<<"\033[A";
			cout<<"\033[A";
			cout<<"\033[2K";
			return 1;
		}
		if(kaito==eigo[i]){
			typesu+=kaito.length();
			if(j==1){
				pinpon++;
			}
			return 0;
		}else{
			cout<<"\033[A";
			cout<<"\033[A";
			cout<<"\033[2K";
			if(j>2){
				cout<<"【"<<eigo[i]<<"】"<<endl;
			}
			return 1;
		}
		return 1;
	}/*}}}*/

	int Duo::cntnue(int p)/*{{{*/
	{
		char key;
		cout<<"続けますか？[y/n]"<<endl;
		cin>>key;
		if(key!='y'){
			return 0;
		}else{
			cout<<"すべての単語から   [a]"<<endl;
			if(p){
				cout<<"間違えた単語のみ   [m]"<<endl;
			}else{
				cout<<"同じ範囲から       [m]"<<endl;
			}
			cout<<"今回の範囲以外から [n]"<<endl;
			cin>>key;
			if(key=='n'){
				return 3;
			}
			if(key=='m'){
				return 2;
			}else{
				return 1;
			}
		}
	}/*}}}*/

	void Duo::spin()/*{{{*/
	{
		int cntinum=1;
		input();
		int ik=0;//開始位置
		int dasukazu=0;//出題数
		double jikan=0.001;//総入力時間
		time_t start,end;
		int dasukazu2=dasukazu;
		vector<int> gotou(dasukazu,1);
		int misscnt=0;//MISS:
		int dashita=0;

		while(1){
			if(cntinum==1){//HAJIME:
				shuffle();
				ik=0;//開始位置
			}

			if(cntinum==1||cntinum==3){//NEXT:
				dasukazu=0;//出題数
				jikan=0.001;//総入力時間
				cout<<"出題数を入力(1~"<<torokusu-ik<<") : ";
				cin>>dasukazu;
				if(dasukazu>torokusu-ik){
					dasukazu=torokusu-ik;
				}
			}

			if(cntinum!=2){//FUKUSHU:
				dasukazu2=dasukazu;
				gotou.assign(dasukazu,1);
			}

			misscnt=0;//MISS:
			dasukazu2=dasukazu2-pinpon;//[m]出題数
			pinpon=0;
			typesu=0;

			cin.ignore();
			cntdwn();
			time(&start);
			dashita=0;
			for(int i=ik;i<dasukazu+ik;i++){
				int j=1;
				if(gotou[i-ik]){
					misscnt++;
					if(i>=dasukazu+ik){
						break;
					}
					cout<<"("<<misscnt<<"/"<<dasukazu2<<")"<<endl;
AGAIN:
					if(show(i,j)){
						j++;
						gotou[i-ik]=gotou[i-ik]+1;
						goto AGAIN;
					}else{
						gotou[i-ik]=gotou[i-ik]-1;
					}
				}
				dashita++;
			}
			time(&end);
			jikan=difftime(end,start);

			cout<<endl<<"お疲れ～"<<endl;
			cout<<typesu/jikan<<"[文字/sec]   ";
			cout<<"正答数 "<<pinpon<<"/"<<dasukazu2<<endl<<endl;;

			if(dasukazu2-pinpon){
				for(int i=ik;i<dasukazu+ik;i++){
					if(gotou[i-ik]){
						gotou[i-ik]=1;
					}
				}
				misscnt=0;
				for(int i=ik;i<dasukazu+ik;i++){
					if(gotou[i-ik]){
						misscnt+=eigo[i].length();
					}
				}
				misscnt += (dasukazu2-pinpon-1)*3+2 ;
				if(misscnt<14){
					misscnt=14;
				}
				int kaigyo=66;
				if(misscnt>kaigyo){
					misscnt=kaigyo;
				}

				cout<<"  ";
				for(int i=0;i<(misscnt-12)/2;i++){
					cout<<"=";
				}
				cout<<"間違えた単語";
				for(int i=0;i<(misscnt-12)/2;i++){
					cout<<"=";
				}
				cout<<endl;
				cout<<"   ";
				int i1=0;
				misscnt=0;
				for(i1=ik;i1<dasukazu+ik;i1++){
					if(gotou[i1-ik]){
						cout<<eigo[i1];
						misscnt+=eigo[i1].length();
						i1++;
						break;
					}
				}
				for(int i=i1;i<dasukazu+ik;i++){
					if(gotou[i-ik]){
						misscnt += eigo[i].length()+3;
						cout<<" , ";
						if(misscnt>kaigyo){
							cout<<endl<<"   ";
							kaigyo+=66;
						}
						cout<<eigo[i];
					}
				}
				misscnt += 2 ;
				if(misscnt<14){
					misscnt=14;
				}
				kaigyo=66;
				if(misscnt>kaigyo){
					misscnt=kaigyo;
				}
				cout<<"   "<<endl;
				cout<<"  ";
				for(int i=0;i<misscnt;i++){
					cout<<"=";
				}
				cout<<endl;
			}else{
				cout<<"===PERFECT==="<<endl<<endl;;
			}
			cntinum=cntnue(dasukazu2-pinpon);
			if(cntinum==0){//やめる
				break;
			}
			if(cntinum==1){//全範囲から
				ik=0;
				pinpon=0;
			}
			if(cntinum==2){//ミスのみ
				if(dasukazu2-pinpon){
				}else{
					pinpon=0;
					cntinum=4;
				}
			}
			if(cntinum==3){//今回の以外
				pinpon=0;
				ik+=dashita;
				if(ik==torokusu){
					cout<<"全範囲終了です！！お疲れ様！！"<<endl;
					cntinum=1;
				}
			}
		}
	}/*}}}*/

	void chose()/*{{{*/
	{
	}/*}}}*/

}

int main(int argc, char **argv)/*{{{*/
{
	ros::init(argc, argv, "tangotype");
	tango_type::Duo a;
	a.spin();
	return 0;
}/*}}}*/


