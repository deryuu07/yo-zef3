#include "iodefine.h"
#include "utility.h"

#define MAZE_SIZE 32
#define NORTH 0
#define EAST  1
#define SOUTH 2
#define WEST  3

unsigned char wmap[MAZE_SIZE][MAZE_SIZE];
unsigned char smap[MAZE_SIZE][MAZE_SIZE];
unsigned char sub_map[MAZE_SIZE][MAZE_SIZE];
short mx = 0;
short my = 0;
short gx = 1;
short gy = 3;
short head_direction = NORTH;
short current_run_state = STRAIGHT;

short get_head_direction()
{
	return head_direction;
}

void get_position(short *px, short *py)
{
	*px = mx;
	*py = my;
}

void out_map()
{
	short x,y;
	for(y=MAZE_SIZE-1; y>=0; y--){
		for(x=0; x<MAZE_SIZE; x++){
			print_num(wmap[y][x], 2);
			print_str(" ");
		}
		print_str("\r\n");
	}
	print_str("\r\n");
	for(y=MAZE_SIZE-1; y>=0; y--){
		for(x=0; x<MAZE_SIZE; x++){
			print_num(smap[y][x], 2);
			print_str(" ");
		}
		print_str("\r\n");
	}
	print_str("\r\n");
	for(y=MAZE_SIZE-1; y>=0; y--){
		for(x=0; x<MAZE_SIZE; x++){
			print_num(sub_map[y][x], 2);
			print_str(" ");
		}
		print_str("\r\n");
	}
}

void init_position()
{
	mx = 0;
	my = 0;
}

void init_map()
{
	short x,y;
	// すべてのマップデータを未探索状態にする
	for(y=0;y<MAZE_SIZE;y++){
		for(x=0;x<MAZE_SIZE;x++){
			sub_map[y][x] = 0;
			wmap[y][x] = 0x00;
			if ((x == 0)&&(y ==	0)){ wmap[y][x] = 0x0e;}
			if ((x == 1)&&(y == 0)){ wmap[y][x] = 0x0c;}
			if (y == MAZE_SIZE-1){	wmap[y][x] |= 0x01;}
			if (x == MAZE_SIZE-1){	wmap[y][x] |= 0x02;}
			if (y == 0){		wmap[y][x] |= 0x04;}
			if (x == 0){		wmap[y][x] |= 0x08;}
		}
	}

	for(y=0;y<MAZE_SIZE;y++){
		for(x=0;x<MAZE_SIZE;x++){
			sub_map[y][x] = 0;
		}
	}
	sub_map[0][0] = 1;
}

void init_smap(char gy, char gx)
{
	short x,y;

	for (y=0;y<MAZE_SIZE;y++){
		for (x=0;x<MAZE_SIZE;x++){
			smap[y][x]=255;
		}
	}
	smap[gy][gx] = 0;
}

//歩数マップ、壁マップ
void make_smap()
{
	unsigned short	 pt0,pt1,ct;
	short	 x,y;
	unsigned char	 wdata;

	pt0 = 0;
	do{
		ct = 0;
		pt1 = pt0+1;
		for (y=0;y<MAZE_SIZE;y++){
			for (x=0;x<MAZE_SIZE;x++){
				if (smap[y][x]==pt0){
					wdata=wmap[y][x];
					if (((wdata & 0x01)==0x00)&&(y!=MAZE_SIZE-1)){
						if(smap[y+1][x]==255){smap[y+1][x]=pt1;ct++;}}
					if (((wdata & 0x02)==0x00)&&(x!=MAZE_SIZE-1)){
						if(smap[y][x+1]==255){smap[y][x+1]=pt1;ct++;}}
					if (((wdata & 0x04)==0x00)&&(y!=0)){
						if(smap[y-1][x]==255){smap[y-1][x]=pt1;ct++;}}
					if (((wdata & 0x08)==0x00)&&(x!=0)){
						if(smap[y][x-1]==255){smap[y][x-1]=pt1;ct++;}}
				}
			}
		}
		pt0 = pt0+1;
	}while(ct!=0);
}

//壁情報、壁マップ、自己位置ｘ、自己位置ｙ、機体の向き
// void update_map(char wall)
// {
// 	switch(head_direction)
// 	{
// 		case NORTH:
// 			if((wall&0x01) == 0x01){
// 				wmap[my][mx]|=0x01;
// 				if(my!=MAZE_SIZE-1){
// 					wmap[my+1][mx]|=0x04;
// 				}
// 			}
// 			if((wall&0x02) == 0x02){
// 				wmap[my][mx]|=0x02;
// 				if(mx!=MAZE_SIZE-1){
// 					wmap[my][mx+1]|=0x08;
// 				}
// 			}
// 			if((wall&0x08) == 0x08){
// 				wmap[my][mx]|=0x08;
// 				if(mx!=0){
// 					wmap[my][mx-1]|=0x02;
// 				}
// 			}
// 			break;

// 		case EAST:
// 			if((wall&0x01) == 0x01){
// 				wmap[my][mx]|=0x02;
// 				if(mx!=MAZE_SIZE-1){
// 					wmap[my][mx+1]|=0x08;
// 				}
// 			}
// 			if((wall&0x02) == 0x02){
// 				wmap[my][mx]|=0x04;
// 				if(my!=0){
// 					wmap[my-1][mx]|=0x01;
// 				}
// 			}
// 			if((wall&0x08) == 0x08){
// 				wmap[my][mx]|=0x01;
// 				if(my!=MAZE_SIZE-1){
// 					wmap[my+1][mx]|=0x04;
// 				}
// 			}
// 			break;

// 		case SOUTH:
// 			if((wall&0x01) == 0x01){
// 				wmap[my][mx]|=0x04;
// 				if(my!=0){
// 					wmap[my-1][mx]|=0x01;
// 				}
// 			}
// 			if((wall&0x02) == 0x02){
// 				wmap[my][mx]|=0x08;
// 				if(mx!=0){
// 					wmap[my][mx-1]|=0x02;
// 				}
// 			}
// 			if((wall&0x08) == 0x08){
// 				wmap[my][mx]|=0x02;
// 				if(mx!=MAZE_SIZE-1){
// 					wmap[my][mx+1]|=0x08;
// 				}
// 			}
// 			break;

// 		case WEST:
// 			if((wall&0x01) == 0x01){
// 				wmap[my][mx]|=0x08;
// 				if(mx!=0){
// 					wmap[my][mx-1]|=0x02;
// 				}
// 			}
// 			if((wall&0x02) == 0x02){
// 				wmap[my][mx]|=0x01;
// 				if(my!=MAZE_SIZE-1){
// 					wmap[my+1][mx]|=0x04;
// 				}
// 			}
// 			if((wall&0x08) == 0x08){
// 				wmap[my][mx]|=0x04;
// 				if(my!=0){
// 					wmap[my-1][mx]|=0x01;
// 				}
// 			}
// 			break;
// 	}
// 	sub_map[my][mx] = 1;
// }

void update_head(short move_type)
{
	//sraight
	if (move_type == 0)
	{
		switch (head_direction)
		{
			case NORTH: head_direction = NORTH; return;
			case EAST: head_direction = EAST; return;
			case SOUTH: head_direction = SOUTH; return;
			case WEST: head_direction = WEST; return;
		}
	}

	//right slalom
	if (move_type == 1)
	{
		switch (head_direction)
		{
			case NORTH: head_direction = EAST; return;
			case EAST: head_direction = SOUTH; return;
			case SOUTH: head_direction = WEST; return;
			case WEST: head_direction = NORTH; return;
		}
	}

	//left slalom
	if (move_type == 2)
	{
		switch (head_direction)
		{
			case NORTH: head_direction = WEST; return;
			case EAST: head_direction = NORTH; return;
			case SOUTH: head_direction = EAST; return;
			case WEST: head_direction = SOUTH; return;
		}
	}
}

void update_my(short update_type)
{
	if(update_type == 0){
		my--;
	}
	else{
		my++;
	}
}

void update_mx(short update_type)
{
	if(update_type == 0){
		mx--;
	}
	else{
		mx++;
	}
}

void set_position(short px, short py)
{
	mx = px;
	my = py;
}

void set_run_state(short curr_state)
{
	current_run_state = curr_state;
}

void update_map(char g_wall)
{
	wmap[my][mx] |= g_wall;
}

char update_wall()
{
	char l_wall = 0x00;
	char g_wall = 0x00;
	short ad_io[4];
	get_sens_io(ad_io);

	if(ad_io[0] == 1 || ad_io[2] == 1) 
		l_wall |= 0x01;
	if(ad_io[1] == 1) 
		l_wall |= 0x08;
	if(ad_io[3] == 1) 
		l_wall |= 0x02;

	switch(head_direction){
		case NORTH: 
			if((l_wall&0x01)==0x01) g_wall |= 0x01;
			if((l_wall&0x02)==0x02) g_wall |= 0x02;
			if((l_wall&0x08)==0x08) g_wall |= 0x08;
			break;

		case EAST:  
			if((l_wall&0x01)==0x01) g_wall |= 0x02;
			if((l_wall&0x02)==0x02) g_wall |= 0x04;
			if((l_wall&0x08)==0x08) g_wall |= 0x01;
			break;

		case SOUTH: 
			if((l_wall&0x01)==0x01) g_wall |= 0x04;
			if((l_wall&0x02)==0x02) g_wall |= 0x08;
			if((l_wall&0x08)==0x08) g_wall |= 0x02;
			break;

		case WEST:  
			if((l_wall&0x01)==0x01) g_wall |= 0x08;
			if((l_wall&0x02)==0x02) g_wall |= 0x01;
			if((l_wall&0x08)==0x08) g_wall |= 0x04;
			break;
	}

	return g_wall;
}

void update_position()
{
	short delta_lx = 0;
	short delta_ly = 0;
	short delta_lx2 = 0;
	short delta_ly2 = 0;
	
	switch(current_run_state){
		case STRAIGHT: 	   delta_ly += 1; break;
		case LEFT_SLALOM:  delta_lx -= 1; break;
		case RIGHT_SLALOM: delta_lx += 1; break;
	}

	switch(head_direction){
		case NORTH: delta_lx2 = delta_lx; 
					delta_ly2 = delta_ly;
					break;
		case EAST:  delta_lx2 = delta_ly;
					delta_ly2 = -delta_lx;
					break;
		case SOUTH: delta_lx2 = delta_lx; 
					delta_ly2 = -delta_ly;
					break;
		case WEST:  delta_lx2 = -delta_ly;
					delta_ly2 = delta_lx;
					break;
	}

	mx += delta_lx2;
	my += delta_ly2;
}

void update_head_direction()
{
	short head_incr = 0;

	switch(current_run_state){
		case STRAIGHT:	   head_incr = 0; break;
		case LEFT_SLALOM:  head_incr = 1; break;
		case RIGHT_SLALOM: head_incr = -1; break;
	}
	head_direction += head_incr;
	if(head_direction < 0)
		head_direction = WEST;
	if(head_direction > 3)
		head_direction = NORTH;
		
}

short calc_goal_dis(short tmp_x, short tmp_y)
{
	short tmp_dis = 0;

	tmp_dis = abs(gx + gy - tmp_x - tmp_y);

	return tmp_dis;
}

short determ_move()
{
	short move_state = STRAIGHT;
	short tmp_state = STRAIGHT;
	short min_dis = 256;
	short tmp_dis = 0;

	tmp_dis = calc_goal_dis(mx+1, my);
	if(tmp_dis < min_dis){
		min_dis = tmp_dis;
		tmp_state = RIGHT_SLALOM;
	} 

	tmp_dis = calc_goal_dis(mx-1, my);
	if(tmp_dis < min_dis){
		min_dis = tmp_dis;
		tmp_state = LEFT_SLALOM;
	} 

	tmp_dis = calc_goal_dis(mx, my+1);
	if(tmp_dis < min_dis){
		min_dis = tmp_dis;
		tmp_state = STRAIGHT;
	} 

	tmp_dis = calc_goal_dis(mx, my-1);
	if(tmp_dis < min_dis){
		min_dis = tmp_dis;
		tmp_state = U_TURN;
	} 

	switch(head_direction){
		case NORTH: tmp_state += 0; break;
		case EAST:  tmp_state += 0; break;
		case SOUTH: tmp_state += 0; break;
		case WEST:  tmp_state += 0; break;
	}
	move_state = tmp_state;

	return move_state;
}

short map_manager()
{
	short move_state = STRAIGHT;
	char g_wall = 0x00;

	static short test_cnt = 0;

	//向き更新
	update_head_direction();

	//位置更新
	update_position();

	//壁情報更新
	g_wall = update_wall();

	//壁情報をマップに反映
	update_map(g_wall);

	//ゴールまでの距離を算出
	move_state = determ_move();

	if(test_cnt == 0){
		move_state = RIGHT_SLALOM;
	}
	if(test_cnt == 1){
		move_state = LEFT_SLALOM;
	}
	if(test_cnt == 2){
		move_state = RIGHT_SLALOM;
	}
	test_cnt++;

	return move_state;
}

// char get_wall_info(short *ad_io)
// {
// 	char wall = 0x00;

// 	LED_reset();
// 	if(ad_io[SENS_LF] == 1 || ad_io[SENS_RF] == 1){
// 		wall |= 0x01;
// //		LED_on(0);
// 	}
// 	if(ad_io[SENS_LS] == 1){
// 		wall |= 0x08;
// //		LED_on(1);
// 	}
// 	if(ad_io[SENS_RS] == 1){
// 		wall |= 0x02;
// //		LED_on(3);
// 	}

// 	return wall;
// }

//short determ_move(char wall, short gx, short gy)
//{
//	short move_type = 0;
//	short dis[4];
//	short min_dis = 64;
//	short min_dis_num = 0;
//	memset(dis, 64, sizeof(dis));
//
//	switch(head_direction){
//		case NORTH:
//		if(wall == 11){
//			return 3;
//		}
//
//		if((wall&0x01) == 0x00){
//			dis[0] = abs((mx - gx) + (my+1 - gy));
//			if(min_dis > dis[0]){
//				min_dis = dis[0];
//				min_dis_num = 0;
//			}
//		}
//		if((wall&0x02) == 0x00){
//			dis[1] = abs((mx+1 - gx) + (my - gy));
//			if(min_dis > dis[1]){
//				min_dis = dis[1];
//				min_dis_num = 1;
//			}
//		}
//		if((wall&0x08) == 0x00){
//			dis[2] = abs((mx-1 - gx) + (my - gy));
//			if(min_dis > dis[2]){
//				min_dis = dis[2];
//				min_dis_num = 2;
//			}
//		}
//		return min_dis_num;
//
//		case EAST:
//		if(wall == 7){
//			return 3;
//		}
//
//		if((wall&0x01) == 0x00){
//			dis[0] = abs((mx+1 - gx) + (my - gy));
//			if(min_dis > dis[0]){
//				min_dis = dis[0];
//				min_dis_num = 0;
//			}
//		}
//		if((wall&0x02) == 0x00){
//			dis[1] = abs((mx - gx) + (my-1 - gy));
//			if(min_dis > dis[1]){
//				min_dis = dis[1];
//				min_dis_num = 1;
//			}
//		}
//		if((wall&0x08) == 0x00){
//			dis[2] = abs((mx - gx) + (my+1 - gy));
//			if(min_dis > dis[2]){
//				min_dis = dis[2];
//				min_dis_num = 2;
//			}
//		}
//		return min_dis_num;
//
//		case SOUTH:
//		if(wall == 14){
//			return 3;
//		}
//
//		if((wall&0x01) == 0x00){
//			dis[0] = abs((mx - gx) + (my-1 - gy));
//			if(min_dis > dis[0]){
//				min_dis = dis[0];
//				min_dis_num = 0;
//			}
//		}
//		if((wall&0x02) == 0x00){
//			dis[1] = abs((mx-1 - gx) + (my - gy));
//			if(min_dis > dis[1]){
//				min_dis = dis[1];
//				min_dis_num = 1;
//			}
//		}
//		if((wall&0x08) == 0x00){
//			dis[2] = abs((mx+1 - gx) + (my - gy));
//			if(min_dis > dis[2]){
//				min_dis = dis[2];
//				min_dis_num = 2;
//			}
//		}
//		return min_dis_num;
//
//		case WEST:
//		if(wall == 13){
//			return 3;
//		}
//
//		if((wall&0x01) == 0x00){
//			dis[0] = abs((mx-1 - gx) + (my - gy));
//			if(min_dis > dis[0]){
//				min_dis = dis[0];
//				min_dis_num = 0;
//			}
//		}
//		if((wall&0x02) == 0x00){
//			dis[1] = abs((mx - gx) + (my+1 - gy));
//			if(min_dis > dis[1]){
//				min_dis = dis[1];
//				min_dis_num = 1;
//			}
//		}
//		if((wall&0x08) == 0x00){
//			dis[2] = abs((mx - gx) + (my-1 - gy));
//			if(min_dis > dis[2]){
//				min_dis = dis[2];
//				min_dis_num = 2;
//			}
//		}
//		return min_dis_num;
//	}
//
//
//
//
//}
