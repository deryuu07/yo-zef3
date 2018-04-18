#define LOG_SIZE 1000
#define LOG_NUM  4
short slog[LOG_NUM][LOG_SIZE];
float flog[LOG_NUM][LOG_SIZE];
short data_seq1[LOG_NUM];
short data_seq2[LOG_NUM];
short data_cnt1[LOG_NUM];
short data_cnt2[LOG_NUM];
short cnt1[LOG_NUM];
short cnt2[LOG_NUM];
short max_data_seq = 0;
short log_cnt = 1;

void init_log()
{
	short i,j=0;
	float debug_para[4];

	for(i=0; i<LOG_SIZE; i++){
		for(j=0; j<LOG_NUM; j++){
			slog[j][i] = 0;
			flog[j][i] = 0.0;
		}
	}	

	for(i=0; i<LOG_NUM; i++){
		data_seq1[i] = 0;
		data_seq2[i] = 0;
		data_cnt1[i] = 0;
		data_cnt2[i] = 0;
		cnt1[i] = 0;
		cnt2[i] = 0;
	}

	get_debug_para(&debug_para);
	log_cnt = (short)debug_para[1];	
	if(log_cnt == 0)
		log_cnt = 1;
}

void set_sdata(short num, short data)
{
	cnt1[num]++;

	if(cnt1[num] == log_cnt){
		if(data_seq1[num] < LOG_SIZE){
			slog[num][data_seq1[num]++] = data;
			if(max_data_seq < data_seq1[num]){
				max_data_seq = data_seq1[num];
			}
		}
		cnt1[num] = 0;
	}
}

void set_fdata(short num, float data)
{
	cnt2[num]++;

	if(cnt2[num] == log_cnt){
		if(data_seq2[num] < LOG_SIZE){
			flog[num][data_seq2[num]++] = data;
			if(max_data_seq < data_seq2[num]){
				max_data_seq = data_seq2[num];
			}
		}
		cnt2[num] = 0;
	}
}

void out_log_data()
{
	short i, j;

	print_num(max_data_seq, 5);
	print_str("\r");
	timer_wait(100);	

	for(i = 0; i < max_data_seq; i++){
		for(j = 0; j < LOG_NUM; j++){
			print_num(slog[j][i], 5);
			print_str(" ");
		}
		for(j = 0; j < LOG_NUM; j++){
			print_num((short)flog[j][i], 5);
			print_str(" ");
		}
		print_str("\n");
	}
	print_str("\r");
}

void tera_out_log_data()
{
	short i, j;

	for(i = 0; i < max_data_seq; i++){
		for(j = 0; j < LOG_NUM; j++){
			print_num(slog[j][i], 5);
			print_str(" ");
		}
		for(j = 0; j < LOG_NUM; j++){
			print_num((short)flog[j][i], 5);
			print_str(" ");
		}
		print_num(log_cnt, 5);
		print_str(" ");
		print_num(max_data_seq, 5);
		print_str("\r\n");
	}
}

void auto_get_log()
{
	float curr_vel = 0.0;
	float curr_gyro = 0.0;
	float curr_desire_vel = 0.0;
	float curr_desire_gyro = 0.0;

	// get_vel(&curr_vel);
	// get_desire_vel(&curr_desire_vel);
	// get_ang_vel(&curr_gyro);
	// get_desire_gyro(&curr_desire_gyro);

	// get_fdata(0, curr_vel);
	// get_fdata(1, curr_desire_vel);
	// get_fdata(2, curr_gyro);
	// get_fdata(3, curr_desire_gyro);
}