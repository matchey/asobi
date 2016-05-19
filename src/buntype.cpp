//タイピング練習しつつ英文覚えよう/*{{{*/
//2016.04.09 ver. 1.1 writen by Noriaki Machinaka/*}}}*/
#include <ros/ros.h>/*{{{*/
#include <iostream>
#include <sstream>      // std::ostringstream
#include <string>
#include <cstdlib>  //乱数のために必要
#include <ctime>    //乱数のために必要
#include <fstream> //ファイル操作に必要
#include <vector>

using namespace std;/*}}}*/

namespace bun_type{

	string rmvkg(string eibun)/*{{{*/
	{
		string rmved = eibun;
		int kgichi=0;
		int kesu=1;

		while(1){
			switch(kesu){
				case 1:
					kgichi=(int)rmved.find(',');
					break;
				case 2:
					kgichi=(int)rmved.find('.');
					break;
				case 3:
					kgichi=(int)rmved.find('"');
					break;
				case 4:
					kgichi=(int)rmved.find('!');
					break;
				case 5:
					kgichi=(int)rmved.find('?');
					break;
				default :
					return rmved;
			};
			while(kgichi+1)
			{
				rmved.replace(kgichi,1," ");
				switch(kesu){
					case 1:
						kgichi=(int)rmved.find(',');
						break;
					case 2:
						kgichi=(int)rmved.find('.');
						break;
					case 3:
						kgichi=(int)rmved.find('"');
						break;
					case 4:
						kgichi=(int)rmved.find('!');
						break;
					case 5:
						kgichi=(int)rmved.find('?');
						break;
				};
			}
			kesu++;
		}
	}/*}}}*/

	class Duo/*{{{*/
	{
		string eibun;//例文
		vector<string> tango;//穴埋め単語
		string yaku;//例文日本語訳
		int tangosu;//穴埋め単語数
		vector<int> seikai;//穴埋め(一発正解=1)
		int typesu;//穴埋めタイプ文字数
		public:
		Duo(){}
		Duo(ifstream::pos_type&,string);
		int get_tngs() const{ return tangosu; }
		int get_typs() const{ return typesu; }
		void input(ifstream::pos_type,string);//ファイル読み込み(reibun,tangosu)
		void init();//seikai,typesu初期化
		void rst_seikai();//穴埋め正解リセット(seikai=0)
		void cursor2r(int);//カーソルを左からint番目に移動
		int showtoi(int,const vector<int>&);//問の表示
		int answer(int,vector<int>&);//回答入力
	};/*}}}*/

	Duo::Duo(ifstream::pos_type &pos, string fname)/*{{{*/
	{
		int tmp=0;
		input(pos,fname);
		vector<int> seikai(tangosu,0);
		for(int i=0;i<tangosu;i++){
			tmp+=tango[i].length();
		}
		typesu=tmp;
	}/*}}}*/

	void Duo::input(ifstream::pos_type pos, string fname)/*{{{*/
	{
		tangosu=0;
		string tmpbun;
		string tmptan;

		ifstream f(fname.c_str());
		if (f.fail()){
			cout<<"err"<<endl;
		}

		f.seekg(pos); // 保存した位置に移動

		getline(f,eibun);
		eibun = "  "+eibun;
		getline(f,tmpbun);//カンマ区切り単語一行分
		stringstream ss(tmpbun);
		while(getline(ss,tmptan,','))
		{
			tango.push_back(tmptan);
			tangosu++;
		}
		getline(f,yaku);

		//	pos = f.tellg();//位置を保存
	}/*}}}*/

	void Duo::init()/*{{{*/
	{
		int tmp=0;
		seikai.assign(tangosu,0);
		for(int i=0;i<tangosu;i++){
			tmp+=tango[i].length();
		}
		typesu=tmp;
	}/*}}}*/

	void Duo::rst_seikai()/*{{{*/
	{
		seikai.assign(tangosu,0);
	}/*}}}*/

	void Duo::cursor2r(int r)/*{{{*/
	{
		printf("\033[%dD" ,(int)eibun.length()+3); //カーソルを文の前へ
		if(r){	
			printf("\033[%dC" ,r); //カーソルを空欄へ移動
		}
	}/*}}}*/

	int Duo::showtoi(int rp, const vector<int> &gotou)/*{{{*/
	{
		int nokori=tangosu;//残り回答数
		int hyoji=0;//各空欄の表示文字数
		int kakko=0;// (の開始位置

		if(rp){	
			cout<<"\033[A";
			cout<<"\033[2K";
			cout<<"\033[A";
			cout<<"\033[A";
		}
		cout<<yaku<<endl;
		cout<<eibun;
		for(int i=0;i<tangosu;i++){
			if(seikai[i]){//既に正解してたら(seikai[i]=1)
				nokori--;
				continue;
			}
			if(gotou[i]){//既に正解してたら(gotou[i]=1)
				nokori--;
				continue;
			}
			hyoji = rp+1;
			kakko = (int)rmvkg(eibun).find(" "+tango[i]+" ",kakko);
			if( !(kakko+1) ){
				kakko = 0;
			}
			cursor2r(kakko);
			if(hyoji<(int)tango[i].length()){//単語の途中まで表示
				cout<<"(";
				printf("\033[%dC" ,hyoji);
				for(int j=hyoji+1;j<(int)tango[i].length();j++){
					cout<<" ";
				}
				cout<<")";
			}else{//すべての文字を表示(赤文字)
				printf("\033[%dC" ,1);//カーソルを右に１つ移動
				printf("\033[31m");//赤の文字に変更
				cout<<tango[i];
				printf("\033[39m");//文字の色を通常の色に戻す
			}
		}
		cout<<endl;
		if( !(nokori) ){
			cout<<endl;
		}
		return nokori;//全正解で0を返す
	}/*}}}*/

	int Duo::answer(int rp, vector<int> &gotou)/*{{{*/
	{
		string kt;//回答単語群
		int ktnum=0;//回答単語数
		vector<string> kaito;//回答各単語
		string tmptan;//回答単語一時格納
		int nokori=tangosu;

		getline(cin,kt);
		stringstream ss(kt);
		while(getline(ss,tmptan,' '))
		{
			kaito.push_back(tmptan);
			ktnum++;
		}
		for(int i=0;i<ktnum;i++){
			if(kaito[i]==":suspend"){
				return -10;
			}
		}
		for(int i=0;i<ktnum;i++){
			for(int j=0;j<tangosu;j++){
				if(gotou[j]||seikai[j]){continue;}
				if(kaito[i]==tango[j]){
					gotou[j]=1;
					seikai[j]= rp  ? 0 : 1;
					break;
				}
			}
		}
		for(int i=0;i<tangosu;i++){
			if(gotou[i]||seikai[i]){
				nokori--;
			}
		}
		if( !(rp||nokori) ) {	
			cout<<"\033[A";
			cout<<"\033[2K";
			cout<<"\033[A";
			cout<<"\033[A";
		}
		return nokori;//全正解で0を返す
	}/*}}}*/

	int cnttrks(vector<ifstream::pos_type> &pos, string fname)/*{{{例文登録数を返す*/
	{
		int torokusu=0;
		string tmp;
		ifstream f(fname.c_str());
		if (f.fail()){
			cout<<"err"<<endl;
		}
		while(1)
		{
			if(!(torokusu%3)){
				pos.push_back(f.tellg());
			}
			torokusu++;
			if( !(getline(f,tmp)) ){
				break;
			}
		}
		torokusu/=3;
		return torokusu;
	}/*}}}*/

	string chssction()/*{{{*/
	{
		string section;
		int ss;
		string fname = "/home/amsl/ros_catkin_ws/src/asobi/src/duobuntxt/section";
		cout<<"学習するセクションを入力(1~45):";
		cin>>section;
		ss = atoi(section.c_str());
		if(ss>0&&ss<46){
			fname += section+".txt";
		}else{
			fname = "/home/amsl/ros_catkin_ws/src/asobi/src/duobun.txt";
		}
		return fname;
	}/*}}}*/

	void cntdwn()/*{{{開始時カウントダウン*/
	{
		cout<<"準備はいいですか(Etnerキーを押してください)"<<endl;;
		getchar();
		//	cin.ignore();
		cout<<"\033[A";
		cout<<"\033[A";
		cout<<"\033[2K";
		cout<<"(プレイ中 \":suspend\" と入力することで中断できます)"<<endl;

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

	void psshffl(int torokusu,vector<ifstream::pos_type> &pos)/*{{{posシャッフル*/
	{
		srand((unsigned int)time(NULL));
		rand();rand();
		for(int i=0;i<torokusu;i++){
			int j = rand()%torokusu;
			ifstream::pos_type tmp;
			tmp = pos[i];
			pos[i] = pos[j];
			pos[j] = tmp;
		}
	}/*}}}*/

	void shuffle(int dasukazu, bun_type::Duo *a, vector<int> pinpon)/*{{{出題順シャッフル*/
	{
		srand((unsigned int)time(NULL));
		rand();rand();
		for(int i=0;i<dasukazu;i++){
			int j = rand()%dasukazu;
			bun_type::Duo tmp;
			int tmpin;
			tmp = a[i];
			tmpin =pinpon[i];
			a[i] = a[j];
			pinpon[i]=pinpon[j];
			a[j] = tmp;
			pinpon[j]=tmpin;
		}
	}/*}}}*/

	int cntnue(int p)/*{{{コンティニュ表示*/
	{
		char key;
		cout<<"続けますか？[y/n]"<<endl;
		cin>>key;
		cin.ignore();
		if(key!='y'){
			return 0;
		}else{
			cout<<"すべての文から     [a]"<<endl;
			if(p){
				cout<<"間違えた単語のみ   [m]"<<endl;
			}else{
				cout<<"同じ範囲から       [m]"<<endl;
			}
			cout<<"今回の範囲以外から [n]"<<endl;
			cin>>key;
			cin.ignore();
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

	void spin()/*{{{*/
	{
		/*{{{宣言*/
		int rp=0;
		int dasukazu=0;
		int dashita=0;
		double jikan=0.001;//総入力時間
		time_t start,end;
		vector<int> gotou(10,0);
		bun_type::Duo *a;
		string filename;
		int ik=0;//開始位置
		int cntinum=1;//continueメニュー
		int ans = 0; //answer()の返り値
		vector<int> pinpon;//正答=1
		int setosu=0;//正答数
		int typekei=0;//タイプ文字数合計
		vector<ifstream::pos_type> pos;//eibunの各開始位置ポインタ
		int torokusu=0;
		/*}}}*/


		filename = chssction();
		torokusu=cnttrks(pos,filename);
		cout<<"出題数(1~"<<torokusu<<")を入力:";/*{{{*/
		cin>>dasukazu;
		cin.ignore();
		if(dasukazu>torokusu){
			dasukazu=torokusu;
		}/*}}}*/

		psshffl(torokusu,pos);
		a = new bun_type::Duo[dasukazu];
		for(int i=0;i<dasukazu;i++){
			a[i].input(pos[i],filename);
		}
		pinpon.assign(dasukazu,0);
		for(int i=0;i<dasukazu;i++){
			a[i].init();
		}
		while(1){//やめるまで
			jikan = 1e-10;
			cntdwn();
			time(&start);
			for(int i=0;i<dasukazu;i++){
				if(pinpon[i]){
					continue;
				}
				typekei+=a[i].get_typs();
				gotou.assign(a[i].get_tngs(),0);
				dashita++;
				cout<<"("<<dashita<<"/"<<dasukazu-setosu<<")"<<endl;
				while(1){/*全正解まで*/
					if( (!a[i].showtoi(rp,gotou)) ){//全解答済
						break;
					}
					ans=a[i].answer(rp,gotou);
					if(ans==-10){
						time(&end);
						jikan+=difftime(end,start);
						cntdwn();
						time(&start);
					}else if(ans){//全正解でなければ
						rp++;
					}
				}
				pinpon[i] = rp ? 0 : 1 ;
				rp=0;
			}
			time(&end);
			jikan+=difftime(end,start);

			setosu=0;/*{{{カウント正答数*/
			for(int i=0;i<dasukazu;i++){
				if(pinpon[i]==1){
					setosu+=pinpon[i];
				}
			}/*}}}*/
			cout<<endl<<"お疲れ～"<<endl;
			cout<<typekei/jikan*60<<"[文字/min]   ";
			cout<<"正答数 "<<setosu-(dasukazu-dashita)<<"/"<<dashita<<endl<<endl;;

			cntinum=cntnue(dasukazu-setosu);/*{{{*/
			if(cntinum==0){//やめる
				break;
			}
			typekei=0;
			dashita=0;
			if(cntinum==2){//ミスのみ
				shuffle(dasukazu,a,pinpon);
				if(dasukazu-setosu){
					continue;
				}else{
					setosu=0;
					pinpon.assign(dasukazu,0);
					for(int i=0;i<dasukazu;i++){
						a[i].rst_seikai();
					}
					continue;
					//	cntinum=4;
				}
			}
			delete [] a;
			setosu=0;

			if(cntinum==3){//今回の以外
				ik+=dasukazu;
				if(ik==torokusu){
					cout<<"全範囲終了です！！お疲れ様！！"<<endl;
					ik=0;
					cntinum=1;
				}
			}
			if(cntinum==1){//全範囲から
				psshffl(torokusu,pos);
				ik=0;
			}
			pinpon.assign(dasukazu,0);

			cout<<"出題数(1~"<<torokusu-ik<<")を入力:";/*{{{*/
			cin>>dasukazu;
			cin.ignore();

			if(dasukazu>torokusu-ik){
				dasukazu=torokusu-ik;
			}/*}}}*/

			pinpon.assign(dasukazu,0);
			a = new bun_type::Duo[dasukazu];
			for(int i=0;i<dasukazu;i++){
				a[i].input(pos[ik+i],filename);
			}
			for(int i=0;i<dasukazu;i++){
				a[i].init();
			}
			/*}}}*/
		}
	}/*}}}*/

}//bun_type

int main(int argc, char **argv)/*{{{*/
{
	ros::init(argc, argv, "buntype");
	bun_type::spin();
	return 0;
}/*}}}*/

