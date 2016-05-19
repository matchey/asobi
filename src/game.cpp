//game.cpp
#include <iostream>
#include <cstdlib>  //乱数のために必要
#include <ctime>    //乱数のために必要
using namespace std;

//ヒーローつまり英雄ですね
class Hero
{
    int power;                       //ヒーローのパワー
public:
    Hero() : power(100){}            //ヒーローのコンストラクタ、ヒーローのパワーをはじめ100とした
    int getPower(){ return power; }  //「powerを戻す」関数
    void kougeki_suru(int n);        //「ヒーローが攻撃する」関数
    void kougeki_sareru(int n);      //「ヒーローが攻撃される」関数
};

//「ヒーローが攻撃する」関数の定義、nは攻撃に使うパワー
void Hero::kougeki_suru(int n)
{
    cout << "悪党め。正義の攻撃を受けてみよ。" << endl;
    cout << "どか～ん！！！" << endl;
    power -= n;  //攻撃したのでパワーを減らします。
    //もちろん本当にパワーをどこかにやったのではなく、単に数値を減らすだけでした。

    //もしパワーがまだ0以上ならよし、もし、負になったら、使いすぎで負けとした。
    //以下のifとelseはそういう意味です。
    if(power >= 0){
        cout << "（現在のパワーは" << power << "になった。）" << endl;
    }
    else{
        cout << "しまった！パワーを使いすぎた。" << endl;
        cout << "もうおしまいだ！！！がくっ。" << endl;
    }
}

//「ヒーローが攻撃される」関数の定義、nは攻撃されて減らされるパワーの量
void Hero::kougeki_sareru(int n)
{
    cout << "くそっ。悪党の攻撃も当たることがあるのか。" << endl;
    power -= n;  //攻撃されてパワーが減る。
    //以下の仕組みはkougeki_suru()とほぼ同じ。
    if(power >= 0){
        cout << "（現在のパワーは" << power << "になった。）" << endl;
    }
    else{
        cout << "やられた。がくっ。" << endl;
    }
}

//大魔王。実はヒーローとほとんど同じ構造です
class Daimao
{
    int power;                       //大魔王のパワー
public:
    Daimao() : power(100){}          //大魔王のコンストラクタ、大魔王のはじめのパワーは100とした
    int getPower(){ return power; }  //「powerを戻す」関数
    void kougeki_suru(int n);        //「大魔王が攻撃する」関数
    void kougeki_sareru(int n);      //「大魔王が攻撃される」関数
};

//「大魔王が攻撃する」関数の定義、nは攻撃に使うパワー
void Daimao::kougeki_suru(int n)
{
    cout << "大魔王様の一撃をうけてみよ。" << endl;
    cout << "どか～ん。" << endl;
    power -= n;  //攻撃したのでパワーを減らします。
    //もちろん本当にパワーをどこかにやったのではなく、単に数値を減らすだけでした。

    //もしパワーがまだ０以上ならよし、もし、負になったら、使いすぎで負けとした。
    //しかし、大魔王のパワーは秘密なので書かない。
    if(power < 0){
        cout << "しまった！！！パワーを使いすぎた。" << endl;
        cout << "む、む、む。無念だ。がくっ。" << endl;
    }
}

//「大魔王が攻撃される」関数の定義。nは攻撃されて減らされるパワーの量
void Daimao::kougeki_sareru(int n)
{
    cout<< "くそっ。正義の味方の攻撃も当たることがあるのか。" <<endl;
    power -= n;  //攻撃されてパワーが減る。
    //以下の仕組みはkougeki_suru()とほぼ同じ。大魔王のパワーは秘密なので書かない。
    if(power < 0){
        cout << "やられた。がくっ。" << endl;
    }
}

//対決場所のクラス（ヒーローや大魔王が「もの」なら、対決場所も「もの」ですね。）
class Taiketu_basyo
{
    Daimao bu;         //対決場所にいる大魔王bu！
    Hero you;          //対決場所にいるヒーローyou！
    int bu_no_basho;   //大魔王のいる場所（1～5の数値）、これは後で決まる
    int you_no_basho;  //ヒーローのいる場所（1～5の数値）、これは後でユーザが決める
public:
    Taiketu_basyo();            //対決場所のコンストラクタ、定義はクラス宣言の外で
    void taiketu();             //「ヒーローと大魔王が1回対決する」関数
    void kurikaesi_taiketu();   //「繰り返し対決する」関数
};

//対決場所のコンストラクタの定義
Taiketu_basyo::Taiketu_basyo()
{
    srand( (unsigned)time( NULL ) ); //乱数の初期化
    bu_no_basho = rand() % 5 + 1;    //rand()% 5は0～4の中のでたらめな数（乱数）
    //したがってrand() % 5 + 1は1～5のうちのどれか（次回に説明します）、これで大魔王の場所が決まった
    cout << "あなたと大魔王ブーとの決戦です。\n" << endl;
    cout << "大魔王ブーは座標1～5のどこかに潜んでいます。" << endl;
    cout << "あなたもどこかに身を潜めてください。" << endl;
    cout << "身を潜める座標（1～5の数値）を入力してください。" << endl;
    cin >> you_no_basho;             //ヒーローの場所を入力
}

void Taiketu_basyo::taiketu()   //「ヒーローと大魔王が対決する」関数の定義
{
    int iti, kougeki;           //一時的に必要な「位置」と「攻撃量」の変数（いれもの）
    cout << "さあ、あなたの攻撃です。" << endl;
    cout << "攻撃の位置（1～5の数値）を入力してください。>> ";
    cin >> iti;                //攻撃する位置をitiに代入
    cout << "攻撃に使うパワー（" << you.getPower() << "以下の数値）を入力してください。>> ";
    cin >> kougeki;            //攻撃に使うパワーをkougekiに代入
    cout << endl;
    you.kougeki_suru(kougeki); //ヒーローyouの攻撃
                               //攻撃量はkougekiに代入された値、この値だけヒーローのパワーは減る
    //taiketu()の改造した場所
    if(you.getPower() < 0){ //攻撃のしすぎで、ヒーローのパワーが負になれば、、、
        return;             //returnはその関数の実行を終了させる
    }
    if(bu_no_basho == iti){    //もしbu_no_bashoとitiが一致したら（つまり、ねらったところに大魔王がいたら）
        bu.kougeki_sareru(kougeki * 2);  //攻撃を受けます。このとき大魔王は
                                         //ヒーローが使ったパワーの2倍を消耗します。
                                         //kougeki * 2とはkougekiの2倍という意味です。
        //taiketu()の改造した場所
        if(bu.getPower() < 0){  //攻撃されて、大魔王のパワーが負になれば、、、
            return;             //returnはその関数の実行を終了させる
        }
    }
    else{
        cout << "あなたの攻撃ははずれたようです。" << endl;  //はずれたら、何も起こらない
    }
    cout << endl;
    cout << "大魔王の攻撃です。" << endl;
    cout << "（エンターキーを押してください。）" << endl;
    cin.sync();       //cinをフラッシュ
    cin.get();        //1時ストップ
    iti = rand() % 5 + 1;       //大魔王の攻撃の位置は乱数（ここでは1～5までのうちのどれか）で決まる
    kougeki = rand() % 100 + 1; //大魔王の攻撃の量も乱数（1～100までのうちのどれか）で決まる
    bu.kougeki_suru(kougeki);   //大魔王buの攻撃（大魔王のパワーが減る）
    //taiketu()の改造した場所
    if(bu.getPower() < 0){  //攻撃のしすぎで大魔王のパワーが負になれば、、、
        return;             //returnはその関数の実行を終了させる
    }
    if(you_no_basho == iti){              //当たったら
        you.kougeki_sareru(kougeki * 2);  //攻撃される
    }
    else{                                 //はずれたら
        cout << "大魔王ブーの攻撃ははずれたようです。" << endl;
    }
}

void Taiketu_basyo::kurikaesi_taiketu()
{
    while(1){
        taiketu();
        if(you.getPower() < 0){
            cout << "\n大魔王の前に正義のヒーローは敗れました。"<<endl;
            break;
        }
        if(bu.getPower() < 0){
            cout << "\n正義のヒーローの前に大魔王は敗れました。" << endl;
            break;
        }
    }
}

int main()
{
    Taiketu_basyo dokoka;  //対決場所Dokokaの生成
                           //ここでコンストラクタが働き、ヒーローと大魔王の位置が決められる。

    dokoka.kurikaesi_taiketu();      //dokokaの対決
} 
