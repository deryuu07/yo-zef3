void main(void)
{
	//各モジュール初期化
	hardware_initialize();
	fld_init_fcu_ram();
	load_cui_data_set2();
	init_log();
	init_map();	
	
	//バッテリー確認
	if(!check_batt()){
		LED_manager(2);
		while(1);
	}

	//CMT0スタート
	timer_start(0);

	while(1){
		mode_manager(select_mode());
	}

}
