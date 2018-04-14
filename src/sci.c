#include "iodefine.h"
#include "SCI.h"

#define BACK_SPACE 0x08
#define ENTER 0x0D
#define ESCAPE 0x1B

#define SCI_PCLK		48 000 000	//覚書
#define SCI_BITRATE		38400		//覚書
#define SCI_BRR			38			//ビットレート調整用の定数。(SCI_PCLK)/(64*SCI_BITRATE*2^(2n-1))-1=BRRで求める(n=0)

void SCI_initialize(){				//SCI初期設定
	unsigned short dummy;			//ダミーとループカウンタの変数

	IEN(SCI1,TXI1) = 0;				//TXI割り込み禁止
	IEN(SCI1,RXI1) = 0;				//RXI割り込み禁止
	MSTP_SMCI1 = 0;					//SCI1モジュールスタンバイ解除
	SCI1.SCR.BYTE = 0x00;			//送信,受信及び割り込み要求を禁止
	MPC.PWPR.BIT.B0WI = 0;			//PFSWEビット書き込み許可
	MPC.PWPR.BIT.PFSWE = 1;			//PFSレジスタ書き込み許可
	MPC.P26PFS.BIT.PSEL = 10;		//P26をTXD1として使用する
	MPC.P30PFS.BIT.PSEL = 10;		//P30をRXD1として使用する
	PORT2.PMR.BIT.B6 = 1;			//TXD1を周辺モジュール入出力に設定
	PORT3.PMR.BIT.B0 = 1;			//RXD1を周辺モジュール入出力に設定
	SCI1.SCR.BIT.CKE = 0;			//クロック設定
	SCI1.SMR.BYTE = 0x00;			//b0	0：
									//b1 クロック選択ビット	0：PCLKクロック(n=0)
									//b2 マルチプロセッサモードビット	0：マルチプロセッサ通信機能を禁止
									//b3 ストップビット長選択	0：1ストップビット
									//b4 パリティモードビット	0：偶数パリティで送受信
									//b5 パリティ許可ビット	0：パリティビットを付加
									//b6 キャラクタ長ビット	0：データ長8ビットで送受信
									//b7 コミュニケーションモードビット	0：調歩同期式モードで動作
	SCI1.BRR = SCI_BRR;				//ビットレート調整
	IPR(SCI1,TXI1) = 7;				//割り込み優先度設定
	IPR(SCI1,RXI1) = 7;				//割り込み優先度設定
	IR(SCI1,TXI1) = 0;				//割り込み要求フラグクリア
	IR(SCI1,RXI1) = 0;				//割り込み要求フラグクリア

	for(dummy = 0; dummy < 50000; dummy++); //ちょっと待つ

	SCI1.SCR.BIT.TIE = 1;			//TXI割り込み許可
	SCI1.SCR.BIT.RIE = 1;			//RXI割り込み許可
	SCI1.SCR.BIT.RE = 1;			//受信開始
	SCI1.SCR.BIT.TE = 1;			//送信開始
	dummy = SCI1.SSR.BYTE;			//ダミーリード
	SCI1.SSR.BYTE = 0xC0;			//フラグクリア
}

void sci_print(unsigned char data){	//unsigned char型の文字列を送る関数。下の関数で使っている
	while(!IR(SCI1,TXI1));
	SCI1.TDR = data;				//データを転送
	IR(SCI1,TXI1)=0;
}

char sci_data_get(void){
	char data;
	IR(SCI1,RXI1) = 0;
	while(!IR(SCI1,RXI1));
	data = SCI1.RDR;				//データを返す
	return data;
}

void print_str(unsigned char *str){	//文字列を送る関数。引数から文字列を持ってくる
	int i;
	for(i=0;str[i]!='\0';i++){
		sci_print(str[i]);
	}
}

void print_num(long data,short n){
	char min_flag=0;
	short i,j;
	unsigned char str[11]={0};
	long digit=1;

	if(data<0){
		min_flag=1;
		data*=-1;
	}

	/*桁数作成*/
	for(i=0;i<12;i++){
		if(data<digit)break;
		digit*=10;
	}
	digit/=10;
	if(data==0){
		i=1;
	}

	/*表示桁数と比較*/
	if(n<=i){
		n=i;						//最大桁nを更新
		if(min_flag==1){
			sci_print('-');
		}
		if(n<11)
		str[n+1]='\0';				//文字列終端
	}
	else{
		/*iは桁数nは最大文字列*/
		for(j=0;j<n;j++){		//空白作成 0~n-1
			str[j]=' ';
		}
		if(min_flag==1){
			str[n-i-1]='-';
		}

		if(n<11)
		str[n+1]='\0';
	}
	for(j=(n-i);j<n;j++){
		str[j]='0'+(data/digit);
		data%=digit;
		digit/=10;
	}
	print_str(str);
}

void print_clear(void){
	sci_print(0x1B);
	print_str("[2J");
	sci_print(0x1B);
	print_str("[0;0H");
}

void back_space(char n){
	char i;
	for(i = 0;i < n; i++){
		sci_print(0x1B);
		print_str("[1D");
		sci_print(' ');
		sci_print(0x1B);
		print_str("[1D");
	}
}

void pointer_move(unsigned char line,unsigned char column){
	sci_print(0x1B);
	sci_print('[');
	print_num(line,0);
	sci_print(';');
	print_num(column,0);
	sci_print('H');
}

void clear_pointer_to_eol(void){
	sci_print(0x1B);
	print_str("[0K");
}

void clear_sol_to_pointer(void){
	sci_print(0x1B);
	print_str("[1K");
}

float scan_fnum(void){
	char data[20]={0};
	char point=0;
	char period_flag=0;
	char minus_flag=0;

	do{
		data[point]=sci_data_get();
		if((data[point]==ENTER)&&(point>0)){
			break;
		}
		if(('0'<=data[point])&&(data[point]<='9')){
			sci_print(data[point]);
			point++;
		}
		else if((data[point]=='-')&&(minus_flag==0)){
			sci_print(data[point]);
			point++;
			minus_flag = 1;
		}
		else if((data[point]=='.')&&(period_flag==0)){
			sci_print(data[point]);
			point++;
			period_flag = 1;
		}
		else if((data[point]==BACK_SPACE)&&(point>0)){
			back_space(1);
			point--;
			data[point]=0;
		}
	}
	while(1);
	data[point]=0;
	return my_atof(data);
}

short scan_num(void){
	char data[20]={0};
	char point=0;
	do{
		data[point]=sci_data_get();
		if((data[point]==ENTER)&&(point>0)){
			break;
		}
		if((('0'<=data[point])&&(data[point]<='9')) || (data[point]=='-')){
			sci_print(data[point]);
			point++;
		}
		else if((data[point]==BACK_SPACE)&&(point>0)){
			back_space(1);
			point--;
			data[point]=0;
		}
	}
	while(1);
	data[point]=0;
	return my_atos(data);
}

char scan_vector(void){
	char state=0;
	char data;
	while(1){
		data=sci_data_get();
		switch(state){
			case 0:
				if(data==ESCAPE)
					state = 1;
				else if(data==ENTER)
					return 4;
				break;
			case 1:
				if(data=='[')
					state = 2;
				else
					state = 0;
				break;
			case 2:
				if(data=='A')
					return 0;
				else if(data=='B')
					return 2;
				else if(data=='C')
					return 1;
				else if(data=='D')
					return 3;
				else
					state = 0;
				break;

		}
	}
}

void scan_enter(void){
	while(sci_data_get()!=ENTER);
}

short my_atos(char s[]){
	short i, n, sign;

	for(i = 0; my_isspace( s[i] ); i++);	//先頭の空白を読み飛ばす
	sign = (s[i] == '-') ? -1 : 1;			//符号を保存する
	if(s[i] == '-' || s[i] == '+')				//符号を飛ばす
		i++;
	for( n = 0; my_isdigit(s[i]); i++)	//s[i]が数字のあいだ、nへ
		n = 10 * n + s[i] - '0';
	return sign * n;								//符号を反映
}

float my_atof(char s[]){
	float value = 0.0, power = 1.0;
	short i, sign;

	for (i = 0; my_isspace(s[i]); i++);
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (; my_isdigit(s[i]); i++){
		value = value * 10 + s[i] - '0';
	}
	if (s[i] == '.')
		i++;
	for (; my_isdigit(s[i]); i++) {
		value = value * 10 + s[i] - '0';
		power *= 10.0;
    }
	return sign * value / power;
}

char my_isspace(char c){
	return (c == ' ' || c == '\t' || c == '\n') ;
}

char my_isdigit(char c){
	return ('0' <= c && c <= '9');
}

void out_float(float data)
{
	short s_data1;
	float tmp_data = data;

	if(data > 1.0){
		s_data1 = (short)data;
		print_num(s_data1, 5);
		print_str(".");
		tmp_data-=(float)s_data1;
		tmp_data*=1000.0;
		print_num((short)tmp_data, 5);
	}
	else{
		print_num(0, 5);
		print_str(".");
		tmp_data*=1000.0;
		print_num((short)tmp_data, 5);
	}
}
