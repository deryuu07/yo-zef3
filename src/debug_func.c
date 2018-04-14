/*
 * debug_func.c
 *
 *  Created on: 2018/02/18
 *      Author: nomura
 */
short end_flag = 1;

void set_end_flag(short num)
{
    end_flag = num;
}

void calc_check()
{
}

void run_test()
{
    count_down();
    init_vel();
    init_enc();
    set_accel_def(6000.0);
    set_max_vel(300.0);
    set_max_gyro(550.0);
    set_acc_offset();
    reset_acc_vel();
    reset_acc_dis();
    reset_angle();
    set_end_flag(1);
    
    timer_start(1);
    timer_start(2);
    while(end_flag);
//    while(!timer_free_wait(3000));

    timer_stop(1);
    timer_stop(2);
    mot_drive(0, 0);
}

void out_sens()
{
    short ad[4];
    short ad_dis[4];
    timer_start(2);

    while(1){
        get_sens_ad(ad);
        get_sens_dis(ad_dis);

        print_num(ad[0], 4);
        print_str(" ");
        print_num(ad[1], 4);
        print_str(" ");
        print_num(ad[2], 4);
        print_str(" ");
        print_num(ad[3], 4);
        print_str(" ");

        print_str("lf: ");
        print_num(ad_dis[0], 4);
        print_str(" ls: ");
        print_num(ad_dis[1], 4);
        print_str(" rf: ");
        print_num(ad_dis[2], 4);
        print_str(" rs: ");
        print_num(ad_dis[3], 4);
        print_str("\r\n");
    }

}

void out_gyro()
{
    float gyro, ang;
    count_down();
    reset_angle();

    while(1){
        get_ang_vel(&gyro);
        get_angle(&ang);
        print_num((short)gyro, 4);
        print_str(" ");
        print_num((short)ang, 4);
        print_str("\r\n");
    }    
}

void out_accel()
{
    float curr_acc[3];
    float curr_acc_vel[3];
    count_down();
    set_acc_offset();
    reset_acc_vel();

    while(1){
        get_acc(curr_acc);
        get_acc_vel(curr_acc_vel);

        print_num((short)curr_acc[0], 6);
        print_str(" ");
        print_num((short)curr_acc[1], 6);
        print_str(" ");
        print_num((short)curr_acc[2], 6);
        print_str(" ");
        print_num((short)(curr_acc_vel[0]), 6);
        print_str(" ");
        print_num((short)(curr_acc_vel[1]), 6);
        print_str(" ");
        print_num((short)curr_acc_vel[2], 6);
        print_str("\r\n");
    }    
}
