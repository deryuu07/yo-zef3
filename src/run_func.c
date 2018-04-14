#include "utility.h"

#define PARA_SIZE 3
#define RUN_VEL 0
#define RUN_ANG_VEL 1
#define RUN_ANG_ACCEL 2
float slalom_para[PARA_SIZE] = {300.0, 270.0, 2500.0};
float straight_para[PARA_SIZE] = {300.0, 0.0, 0.0};

short start_step_flag = 0;
short straight_step_flag = 0;
short slalom_step_flag = 0;
short run_state = 0;
short run_state_flag = 0;
short finish_cnt = 0;

short run_Uturn()
{
    float curr_angle = 0.0;
    short result = 0;
    static short step_cnt = 0;

if(step_cnt == 0){
    set_Uturn_flag(1); //超信地旋回モード

    /***制御処理***/
    result = update_desire_gyro(U_TURN);
    
    /***終了処理***/
    get_angle(&curr_angle);
    if(curr_angle >= 179.0 && result == 1){
        LED_on(0);
        step_cnt = 1;
        reset_gyro_I();
        reset_angle();
//		set_end_flag(0);
//        return 1;
    }
}
if(step_cnt == 1){

    /***制御処理***/    
    /***終了処理***/
    if(timer_free_wait(2000)){  
        step_cnt = 2;
    }
    else{}
}
if(step_cnt == 2){

    /***制御処理***/
    mot_drive(-200, -200);
    
    /***終了処理***/
    if(timer_free_wait(1000)){
  
		set_end_flag(0);
        return 1;
    }
    else{}
}

    return 0;
}

short run_slalom()
{
    float curr_angle = 0.0;
    short result = 0;

    /***制御処理***/
    update_accel2(ACCEL_STEP);
	result = update_desire_gyro(run_state);

    /***終了処理***/
    get_angle(&curr_angle);
    if((curr_angle >= 89.0 || curr_angle <= -89.0) && result == 1){
//        run_state = map_manager();
        reset_dis();
        reset_gyro_I();
        reset_angle();
        return 1;
    }

    return 0;
}

short run_straight()
{
    float curr_dis = 0.0;

    /***制御処理***/
    set_run_state(STRAIGHT);
    update_accel2(ACCEL_STEP);

    /***終了処理***/
    get_dis(&curr_dis);
    if(curr_dis >= 90.0){
        finish_cnt++;
        run_state = map_manager();
        reset_dis();
        reset_gyro_I();
        reset_angle();
        return 1;
    }

    return 0;
}

short run_start_step()
{
    float curr_dis = 0.0;

    /***制御処理***/
    set_Uturn_flag(0);
    set_run_state(STRAIGHT);
    update_accel2(ACCEL_STEP);

    /***終了処理***/
    get_dis(&curr_dis);
    if(curr_dis >= 65.0){
        reset_dis();
        reset_gyro_I();
        reset_angle();

        run_state = map_manager();
        return 1;
    }

    return 0;
}

short run_stop()
{
    float curr_dis = 0.0;

    /***制御処理***/
    update_accel2(DECEL_STEP);

    /***終了処理***/
    get_dis(&curr_dis);
    if(curr_dis >= 65.0){
        reset_dis();
        return 1;
    }

    return 0;

}

void run_manager()
{
    if(!start_step_flag){
        start_step_flag = run_start_step();
    }
    else{
        run_stop();
//         if(!run_state_flag){
//             switch(run_state){
//                 case STRAIGHT:
//                     run_state_flag = run_straight();
//                     break;
//                 case LEFT_SLALOM:
//                 case RIGHT_SLALOM:
// //                    run_state_flag = run_slalom();
//                     run_state_flag = run_straight();
//                     break;
//             }   
//         }
//         else{
//             if(finish_cnt==2){
//                 run_stop();
//             }
//             else{
//                 run_state_flag = 0;
//             }
//         }
    }

//    run_Uturn();

}

