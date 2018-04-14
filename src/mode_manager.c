void mode_manager(short mode)
{
	switch(mode)
	{
		case 0: tera_out_log_data(); break;
		case 1: run_test();     break;
		case 2: out_sens();		break;
		case 3: out_gyro();     break;
		case 4: out_accel();	break;	
		case 5:	coms_start(); break;	
		case 6: out_log_data2(); break;	
		case 7: out_map();		break;	
	}	
}

short select_mode()
{
	short mode = 0;

	while(LED_switch()){
		mode += wheel_vel_counter();
		LED_manager(mode);

		if(mode > 8){
			mode = 0;
		}
	}

	return mode;
}

