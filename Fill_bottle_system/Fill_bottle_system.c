#include "main.h"



void fill_reset_speed (s_fill_bottle_module * p_fill_bottle_module)
{
	uint16_t i = 0;
	p_fill_bottle_module->fill_bottle_speed_ctr = 0;
	p_fill_bottle_module->speed = 0;
	p_fill_bottle_module->fill_bottle_speed_ctr_index = 0;
	p_fill_bottle_module->fill_bottle_speed_ctr_length = 0;
	p_fill_bottle_module->fill_bottle_speed_ctr_avg = 0;
	for (i = 0; i < FILL_BOTTLE_SPEED_BUF_LENGHT; i++){
		p_fill_bottle_module->fill_bottle_speed_ctr_buf[i] = 0;
	}
}//
void fill_update_speed_time (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->fill_bottle_speed_ctr_index %= FILL_BOTTLE_SPEED_BUF_LENGHT;
	if (p_fill_bottle_module->fill_bottle_speed_ctr_length < FILL_BOTTLE_SPEED_BUF_LENGHT){
		p_fill_bottle_module->fill_bottle_speed_ctr_length++;
	}else{
		p_fill_bottle_module->fill_bottle_speed_ctr_avg -= p_fill_bottle_module->fill_bottle_speed_ctr_buf[p_fill_bottle_module->fill_bottle_speed_ctr_index];
	}
	p_fill_bottle_module->fill_bottle_speed_ctr_avg += p_fill_bottle_module->fill_bottle_speed_ctr;
	p_fill_bottle_module->fill_bottle_speed_ctr_buf[p_fill_bottle_module->fill_bottle_speed_ctr_index] = p_fill_bottle_module->fill_bottle_speed_ctr;
	p_fill_bottle_module->fill_bottle_speed_ctr_index++;
	
	//p_fill_bottle_module->fill_bottle_speed_time = p_fill_bottle_module->fill_bottle_speed_ctr;
	p_fill_bottle_module->fill_bottle_speed_time = p_fill_bottle_module->fill_bottle_speed_ctr_avg / p_fill_bottle_module->fill_bottle_speed_ctr_length;
	
	p_fill_bottle_module->fill_bottle_speed_ctr = 1;
}
//
void fill_calc_speed (s_fill_bottle_module * p_fill_bottle_module)
{	
	if ( p_fill_bottle_module->fill_bottle_speed_ctr > 0){
		p_fill_bottle_module->fill_bottle_speed_ctr++;
	}
	if (p_fill_bottle_module->fill_bottle_speed_time > 0){
		p_fill_bottle_module->speed = (60000 / p_fill_bottle_module->fill_bottle_speed_time);
	}
}
//
void fill_wait_piece_fall (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->fill_bottle_flag = 1;
	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_WAIT_PIECE_FALL_TIME;
	p_fill_bottle_module->big_gate_delay = p_fill_bottle_module->fill_bottle_op_delay;
}
//
void fill_set_fill_bottle_signal_on (s_fill_bottle_module * p_fill_bottle_module)
{
	SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_EXT_FILL_B);
}
//
void fill_set_fill_bottle_signal_off (s_fill_bottle_module * p_fill_bottle_module)
{
	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_EXT_FILL_B);
}
//
void fill_insert_to_rej_q (s_fill_bottle_module * p_fill_bottle_module)
{
#ifndef USE_SCREW_SHIFT_BOTTLE
	if (GET_BIT(p_fill_bottle_module->input_buf, OUTPUT_BUF_REJECT) == 0){
		p_fill_bottle_module->reject_q_index %= REJECT_Q_16;
		if (p_fill_bottle_module->enable_big_gate == MY_ENABLE){//使用总料门
			p_fill_bottle_module->reject_q[p_fill_bottle_module->reject_q_index] = SET_FILL_REJECT_START_DELAY +  SET_FILL_UP_NOZZLE_DELAY + SET_FILL_WAIT_PIECE_FALL_TIME;
		}else{
			p_fill_bottle_module->reject_q[p_fill_bottle_module->reject_q_index] = SET_FILL_REJECT_START_DELAY + SET_FILL_UP_NOZZLE_DELAY;
		}
		p_fill_bottle_module->reject_q_index++;
		p_fill_bottle_module->total_reject++;
		p_fill_bottle_module->continue_reject_ctr++;
	}else{
		p_fill_bottle_module->continue_reject_ctr = 0;
		p_fill_bottle_module->total_good++;
		fill_set_fill_bottle_signal_on (p_fill_bottle_module);
	}
	
#else
	if (GET_BIT(p_fill_bottle_module->input_buf, OUTPUT_BUF_REJECT) == 0){
		p_fill_bottle_module->continue_reject_ctr++;
	}else{
		p_fill_bottle_module->continue_reject_ctr = 0;
	}
	p_fill_bottle_module->total_good++;
#endif
	p_fill_bottle_module->total_count_sum++;
}
//
void fill_up_nozzle_no_delay (s_fill_bottle_module * p_fill_bottle_module)
{
//	SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_NOZZLE);
	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_NOZZLE);
}
void fill_down_nozzle_no_delay (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->nozzle_down_delay = 0;
//	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_NOZZLE);
	SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_NOZZLE);
}
//
void fill_up_nozzle (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_UP_NOZZLE_DELAY;
	p_fill_bottle_module->nozzle_up_delay = p_fill_bottle_module->fill_bottle_op_delay;
	p_fill_bottle_module->nozzle_delay = p_fill_bottle_module->nozzle_up_delay;
	fill_up_nozzle_no_delay (p_fill_bottle_module);
}
//
void fill_down_nozzle (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_DOWN_NOZZLE_DELAY;
	p_fill_bottle_module->nozzle_down_delay = p_fill_bottle_module->fill_bottle_op_delay;
	p_fill_bottle_module->nozzle_delay = p_fill_bottle_module->nozzle_down_delay;
	fill_down_nozzle_no_delay (p_fill_bottle_module);
}
//
void fill_start_nozzle_vib (s_fill_bottle_module * p_fill_bottle_module) 
{
	if (SET_FILL_ENABLE_NOZZLE_VIB == MY_ENABLE){
		if (p_fill_bottle_module->nozzle_vib_status == MY_DISABLE){
			p_fill_bottle_module->nozzle_vib_status = MY_ENABLE;//料嘴开始振动
			p_fill_bottle_module->nozzle_vib_up_time = SET_FILL_NOZZLE_VIB_UP_TIME;
			p_fill_bottle_module->nozzle_vib_down_time = 0;
		}
	}
}
//
void fill_stop_nozzle_vib (s_fill_bottle_module * p_fill_bottle_module) 
{
	p_fill_bottle_module->nozzle_vib_status = MY_DISABLE;//料嘴停止振动
	p_fill_bottle_module->nozzle_vib_up_time = 0;
	p_fill_bottle_module->nozzle_vib_down_time = 0;
	fill_down_nozzle_no_delay (p_fill_bottle_module);
}
//
void fill_open_big_gate_door_no_delay (s_fill_bottle_module * p_fill_bottle_module)
{
	fill_start_nozzle_vib (p_fill_bottle_module);
	SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_BIG_GATE_DOOR);
}
//
void fill_close_big_gate_door_no_delay (s_fill_bottle_module * p_fill_bottle_module)
{
	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_BIG_GATE_DOOR);
}
//
void fill_open_big_gate_door (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_OPEN_BIG_GATE_DOOR_DELAY;
	p_fill_bottle_module->big_gate_delay = p_fill_bottle_module->fill_bottle_op_delay;
	fill_open_big_gate_door_no_delay (p_fill_bottle_module);
}
void fill_close_big_gate_door (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_CLOSE_BIG_GATE_DOOR_DELAY;
	p_fill_bottle_module->big_gate_delay = p_fill_bottle_module->fill_bottle_op_delay;
//	inc_Modbus_test_value ();
	fill_close_big_gate_door_no_delay (p_fill_bottle_module);
}
//
void fill_open_shift_bottle (s_fill_bottle_module * p_fill_bottle_module)
{
#ifndef USE_SCREW_SHIFT_BOTTLE
	if (SET_FILL_DOWN_NOZZLE_DELAY > SET_FILL_OPEN_SHIFT_BOTTLE_DELAY){
		p_fill_bottle_module->nozzle_down_delay = SET_FILL_OPEN_SHIFT_BOTTLE_DELAY;
	}else{
		p_fill_bottle_module->nozzle_down_delay = SET_FILL_DOWN_NOZZLE_DELAY;
	}
	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_OPEN_SHIFT_BOTTLE_DELAY;
	p_fill_bottle_module->shift_bottle_delay = p_fill_bottle_module->fill_bottle_op_delay;
#else
#endif
	SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_BOTTLE_SHIFT);
}
void fill_close_shift_bottle_no_delay (s_fill_bottle_module * p_fill_bottle_module)
{
	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_BOTTLE_SHIFT);
}
void fill_close_shift_bottle (s_fill_bottle_module * p_fill_bottle_module)
{
	fill_update_speed_time (p_fill_bottle_module);
	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_CLOSE_SHIFT_BOTTLE_DELAY;
	p_fill_bottle_module->shift_bottle_delay = p_fill_bottle_module->fill_bottle_op_delay;
	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_BOTTLE_SHIFT);
}
//
void fill_set_fill_bottle_complete (s_fill_bottle_module * p_fill_bottle_module)
{
//	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_BOTTLE_COMPLETE_DELAY;
//	p_fill_bottle_module->fill_complete_delay = p_fill_bottle_module->fill_bottle_op_delay;
	SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_COMPLETE);
}
void fill_reset_fill_bottle_complete (s_fill_bottle_module * p_fill_bottle_module)
{
//	p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_RESET_FILL_BOTTLE_COMPLETE_DELAY;
//	p_fill_bottle_module->fill_complete_delay = p_fill_bottle_module->fill_bottle_op_delay;
//	p_fill_bottle_module->pre_fill_complete_time = 0;
	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_COMPLETE);
}
//
void fill_start_reject (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->reject_keep_delay = SET_FILL_REJECT_KEEP_DELAY;
	SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_REJECT);
}
//
void fill_stop_reject (s_fill_bottle_module * p_fill_bottle_module)
{
	SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_REJECT);
}
//
void fill_ready_to_shift_bottle (s_fill_bottle_module * p_fill_bottle_module)
{
	fill_insert_to_rej_q (p_fill_bottle_module);
}
//
void fill_bottle_finished (s_fill_bottle_module * p_fill_bottle_module)
{
//	if ((GET_BIT (p_fill_bottle_module->input_buf, SCREW_IS_RUN) == 1)){
//	}
	p_fill_bottle_module->fill_bottle_state = STANDBY;
	fill_set_fill_bottle_signal_off (p_fill_bottle_module);
}
//
void fill_nozzle_vib_process (s_fill_bottle_module * p_fill_bottle_module)
{
	if (counter_env.system_signal == SYSTER_STOP){
		return;
	}
	if (p_fill_bottle_module->nozzle_vib_status == MY_ENABLE){
		if (p_fill_bottle_module->nozzle_vib_up_time > 0){
			p_fill_bottle_module->nozzle_vib_up_time--;
			if (p_fill_bottle_module->nozzle_vib_up_time == 0){
				p_fill_bottle_module->nozzle_vib_down_time = SET_FILL_NOZZLE_VIB_DOWN_TIME;
				fill_up_nozzle_no_delay (p_fill_bottle_module);
			}
		}else if (p_fill_bottle_module->nozzle_vib_down_time > 0){
			p_fill_bottle_module->nozzle_vib_down_time--;
			if (p_fill_bottle_module->nozzle_vib_down_time == 0){
				p_fill_bottle_module->nozzle_vib_up_time = SET_FILL_NOZZLE_VIB_UP_TIME;
				fill_down_nozzle_no_delay (p_fill_bottle_module);
			}
		}
	}
}
void fill_check_reject_q_out_delay (s_fill_bottle_module * p_fill_bottle_module)
{
	uint16_t i;
	for (i = 0; i < REJECT_Q_16; i++){
		if (p_fill_bottle_module->reject_q[i] > 0){
			p_fill_bottle_module->fill_bottle_idle_time = 0;
			p_fill_bottle_module->reject_q[i]--;
			if (p_fill_bottle_module->reject_q[i] == 0){
				fill_start_reject(p_fill_bottle_module);
			}
		}
	}
}
void fill_check_reject_keep_delay (s_fill_bottle_module * p_fill_bottle_module)
{
	if (p_fill_bottle_module->reject_keep_delay > 0){
		p_fill_bottle_module->fill_bottle_idle_time = 0;
		p_fill_bottle_module->reject_keep_delay--;
		if (p_fill_bottle_module->reject_keep_delay == 0){
			fill_stop_reject(p_fill_bottle_module);
		}
	}
}
//
void fill_common_op (s_fill_bottle_module * p_fill_bottle_module)
{
	fill_nozzle_vib_process (p_fill_bottle_module);
	if (p_fill_bottle_module->big_gate_delay > 0){
		p_fill_bottle_module->big_gate_delay--;
	}
	if (p_fill_bottle_module->nozzle_up_delay > 0){
		p_fill_bottle_module->nozzle_up_delay--;
	}
	if (p_fill_bottle_module->nozzle_down_delay > 0){
		p_fill_bottle_module->nozzle_down_delay--;
		if (p_fill_bottle_module->nozzle_down_delay == 0){
			fill_down_nozzle_no_delay (p_fill_bottle_module);
		}
	}
	if (p_fill_bottle_module->nozzle_delay > 0){
		p_fill_bottle_module->nozzle_delay--;
	}
	if (p_fill_bottle_module->shift_bottle_delay > 0){
		p_fill_bottle_module->shift_bottle_delay--;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//料门提前打开功能
	if (p_fill_bottle_module->pre_fill_complete_time > 0){
		p_fill_bottle_module->pre_fill_complete_time--;
		if (p_fill_bottle_module->pre_fill_complete_time == 0){
			if (p_fill_bottle_module->enable_big_gate == MY_ENABLE){//使用总料门
				fill_open_big_gate_door_no_delay (p_fill_bottle_module);
				p_fill_bottle_module->pre_open_big_gate_flag = PRE_BIG_GATE_OPEN_ING;
			}else{
				fill_reset_fill_bottle_complete (p_fill_bottle_module);//延时耗尽，不管装瓶是否完成都给装瓶完成信号。
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////
	if (p_fill_bottle_module->pre_open_big_gate_flag == PRE_BIG_GATE_OPEN_ING){
		if (p_fill_bottle_module->pre_open_big_gate_ctr <= SET_FILL_OPEN_BIG_GATE_DOOR_DELAY){
			p_fill_bottle_module->pre_open_big_gate_ctr++;
		}
		if (p_fill_bottle_module->pre_open_big_gate_ctr == SET_FILL_OPEN_BIG_GATE_DOOR_DELAY){
			p_fill_bottle_module->pre_open_big_gate_flag = PRE_BIG_GATE_OPEN_ED;
		}
	}else{
		p_fill_bottle_module->pre_open_big_gate_ctr = 0;
	}
	//////////////////////////////////////////////
//	if (p_fill_bottle_module->pre_open_big_gate_flag == PRE_BIG_GATE_OPEN_ED){
//		if (GET_BIT (p_fill_bottle_module->input_buf, OUTPUT_BUF_COMPLETE) == 0){//数粒完成
//			if (p_fill_bottle_module->pre_count_complete_ctr == SET_PIECE_FALL_TIME){
//				fill_close_big_gate_door_no_delay (p_fill_bottle_module);
//				p_fill_bottle_module->pre_close_big_gate_flag = PRE_BIG_GATE_CLOSE_ING;
//				p_fill_bottle_module->pre_open_big_gate_flag = PRE_BIG_GATE_OPEN_ED1;
//			}
//		}
//	}
//	//////////////////////////////////////////////
//	if (p_fill_bottle_module->pre_close_big_gate_flag == PRE_BIG_GATE_CLOSE_ING){		
//		if (p_fill_bottle_module->pre_close_big_gate_ctr <= SET_FILL_CLOSE_BIG_GATE_DOOR_DELAY){
//			p_fill_bottle_module->pre_close_big_gate_ctr++;
//		}
//		if (p_fill_bottle_module->pre_close_big_gate_ctr == SET_FILL_CLOSE_BIG_GATE_DOOR_DELAY){
//			p_fill_bottle_module->pre_close_big_gate_flag = PRE_BIG_GATE_CLOSE_ED;
//		}
//	}else{
//		p_fill_bottle_module->pre_close_big_gate_ctr = 0;
//	}
	if (p_fill_bottle_module->fill_bottle_flag == 1){
		if (GET_BIT (p_fill_bottle_module->input_buf, OUTPUT_BUF_COMPLETE) == 0){//数粒完成
			if (p_fill_bottle_module->pre_count_complete_ctr < SET_PIECE_FALL_TIME){
				p_fill_bottle_module->pre_count_complete_ctr++;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////
	fill_calc_speed (p_fill_bottle_module);
//	if ((get_sys_run_time() % ((COUNTER_SYSTEM_ONE_SECOND_CTR+1) * 20)) == 0){
//		p_fill_bottle_module->speed = (p_fill_bottle_module->total_count_sum - p_fill_bottle_module->total_good_tmp) * 3;
//		p_fill_bottle_module->total_good_tmp = p_fill_bottle_module->total_count_sum;
//		p_fill_bottle_module->speed = (60000 / p_fill_bottle_module->fill_bottle_speed_time);
//	}
	fill_check_reject_q_out_delay (p_fill_bottle_module);
	fill_check_reject_keep_delay (p_fill_bottle_module);
}
//
void fill_clear_op_delay (s_fill_bottle_module * p_fill_bottle_module)
{
	p_fill_bottle_module->fill_bottle_op_delay = 0;
}
//
void fill_sync_vibrator_signal (s_fill_bottle_module * p_fill_bottle_module)
{
	if (GET_BIT(p_fill_bottle_module->input_buf, OUTPUT_BUF_VIB) == 0){//振动器
		SET_OUTPUT_ON(&p_fill_bottle_module->output_buf, OUTPUT_BUF_VIB);
	}else{
		SET_OUTPUT_OFF(&p_fill_bottle_module->output_buf, OUTPUT_BUF_VIB);
	}
}
//
void fill_pre_open_door_time_set(s_fill_bottle_module * p_fill_bottle_module)
{	
	uint16_t temp_u16 = 0;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (p_fill_bottle_module->enable_big_gate == MY_ENABLE){//使用总料门
		temp_u16 = (SET_FILL_OPEN_SHIFT_BOTTLE_DELAY);
	}else{//不使用使用总料门
		temp_u16 = (SET_FILL_OPEN_SHIFT_BOTTLE_DELAY);
	}
	if (counter_env.set_pre_open_door_time > 0){//大于0才有提前打开这个功能
		if (temp_u16 > counter_env.set_pre_open_door_time){
			p_fill_bottle_module->pre_fill_complete_time = temp_u16- counter_env.set_pre_open_door_time;//小料门提前打开时间
			p_fill_bottle_module->pre_fill_complete_ctr++;
		}else{
			p_fill_bottle_module->pre_fill_complete_time = 0;
		}
	}else{
		p_fill_bottle_module->pre_fill_complete_time = 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
//
void fill_bottle_standby_common_op (s_fill_bottle_module * p_fill_bottle_module)
{		
	if (counter_env.system_signal == SYSTER_STOP){
		fill_reset_speed (p_fill_bottle_module);
		if (p_fill_bottle_module->fill_bottle_idle_time == 1000){
			fill_up_nozzle_no_delay (p_fill_bottle_module);//停机延时一段时间后后料嘴升上去
			p_fill_bottle_module->output_buf_backup = p_fill_bottle_module->output_buf;//备份一下当前输出状态
			p_fill_bottle_module->output_buf_manual = p_fill_bottle_module->output_buf_backup;
		}
		if (counter_env.counter_stop_idle_time == 1500){
			fill_up_nozzle_no_delay (p_fill_bottle_module);//停机延时一段时间后后料嘴升上去
		}
	}else if (counter_env.system_signal == SYSTER_RESET){
		p_fill_bottle_module->pre_fill_complete_ctr = 0;
		p_fill_bottle_module->fill_bottle_speed_ctr = 1;
		p_fill_bottle_module->fill_bottle_idle_time = 0;
		p_fill_bottle_module->pre_open_big_gate_flag = PRE_BIG_GATE_INIT;
		p_fill_bottle_module->pre_open_big_gate_ctr = 0;
		p_fill_bottle_module->pre_close_big_gate_flag = PRE_BIG_GATE_INIT;
		p_fill_bottle_module->pre_close_big_gate_ctr = 0;
		fill_down_nozzle_no_delay (p_fill_bottle_module);//开机先把料嘴放下去
	}else if (counter_env.system_signal == SYSTER_PRE_STOP){
		fill_stop_nozzle_vib (p_fill_bottle_module);
		p_fill_bottle_module->fill_bottle_idle_time = 0;
	}		
	if (GET_BIT (p_fill_bottle_module->input_buf, OUTPUT_BUF_COMPLETE) == 0){//数粒完成
		if (counter_env.system_signal == SYSTER_STOP){
			fill_down_nozzle_no_delay (p_fill_bottle_module);//先把料嘴放下去
			p_fill_bottle_module->output_buf = p_fill_bottle_module->output_buf_backup;//恢复之前的输出状态
		}
		fill_set_fill_bottle_complete (p_fill_bottle_module);
		p_fill_bottle_module->fill_bottle_idle_time = 0;
	}else{
		if (p_fill_bottle_module->fill_bottle_idle_time < 60000){
			p_fill_bottle_module->fill_bottle_idle_time++;
		}
	}
	p_fill_bottle_module->fill_bottle_flag = 0;
////////////////////////////////////////////////////////////////////		
}
//
uint16_t poll_fill_complete_status (s_fill_bottle_module * p_fill_bottle_module)
{
	#define SIGNAL_ON_TIME 10 //10ms
	uint16_t r_value = 0;
	if (counter_env.system_signal == SYSTER_RESET){
		p_fill_bottle_module->complete_signal_poll_status = 0;
	}
	switch (p_fill_bottle_module->complete_signal_poll_status)
	{
		case 0:
			if (GET_BIT(p_fill_bottle_module->input_buf, OUTPUT_BUF_SCREW_RD) == 0){//等待信号拉低
				p_fill_bottle_module->complete_signal_t++;
				if (p_fill_bottle_module->complete_signal_t == SIGNAL_ON_TIME){
					fill_close_shift_bottle_no_delay (p_fill_bottle_module);
					p_fill_bottle_module->complete_signal_poll_status++;
					p_fill_bottle_module->complete_signal_t = 0;
					fill_pre_open_door_time_set (p_fill_bottle_module);
				}
			}else{
				p_fill_bottle_module->complete_signal_t = 0;
			}
			break;
		case 1:
			if (GET_BIT(p_fill_bottle_module->input_buf, OUTPUT_BUF_SCREW_RD) == 1){//等待信号拉高
				p_fill_bottle_module->complete_signal_poll_status = 0;
				r_value = 1;
			}
			break;
		default:break;
	}
	if (counter_env.system_signal == SYSTER_STOP){
		fill_close_shift_bottle_no_delay (p_fill_bottle_module);
		r_value = 1;
	}
	if (counter_env.is_master){
		if ((counter_env.servo_motor_shift_bottle_time == 1) && (counter_env.servo_motor_pulse_num == 0)){
			r_value = 1;
		}
		if (counter_env.screw_running == 0){//螺杆放瓶启动后就可以撤销放瓶信号
			fill_close_shift_bottle_no_delay (p_fill_bottle_module);
		}
		if (counter_env.servo_motor_shift_bottle_time == SET_FILL_SERVO_MOTOR_SHIFT_BOTTLE_DELAY){
			fill_pre_open_door_time_set (p_fill_bottle_module);
		}
	}
	return r_value;
}
//
void fill_bottle_sys_unuse_big_gate_process (s_fill_bottle_module * p_fill_bottle_module, uint16_t input_map)//不使用总料门装瓶步骤
{
	////////////////////////////////////////////////////////////////////////////
	p_fill_bottle_module->input_buf = input_map;
	////////////////////////////////////////////////////////////////////////////
	fill_sync_vibrator_signal (p_fill_bottle_module);
	fill_common_op (p_fill_bottle_module);
	if (p_fill_bottle_module->fill_bottle_op_delay > 0){
		p_fill_bottle_module->fill_bottle_op_delay--;
		if (p_fill_bottle_module->fill_bottle_op_delay > 0){
			return;
		}
	}
	switch (p_fill_bottle_module->fill_bottle_state)
	{
		case STANDBY:
			fill_bottle_standby_common_op (p_fill_bottle_module);
			fill_open_big_gate_door_no_delay (p_fill_bottle_module);
		
			fill_start_nozzle_vib (p_fill_bottle_module);
			if (GET_BIT (p_fill_bottle_module->input_buf, OUTPUT_BUF_COMPLETE) == 0){
				p_fill_bottle_module->fill_bottle_state = WAIT_PIECE_FALL;
			}else{
				break;
			}
		case WAIT_PIECE_FALL://装料操作
			fill_wait_piece_fall (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = READY_TO_SHIFT_BOTTLE;
			break;
		case READY_TO_SHIFT_BOTTLE://准备放瓶正位
			fill_ready_to_shift_bottle (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = UP_NOZZLE;
			//break;
		case UP_NOZZLE:
			fill_stop_nozzle_vib (p_fill_bottle_module);
			fill_up_nozzle (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = OPEN_SHIFT_BOTTLE;
			break;
		case OPEN_SHIFT_BOTTLE:
			fill_open_shift_bottle (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = WAIT_SCREW_READY;
			break;
		case WAIT_SCREW_READY:
			if (poll_fill_complete_status (p_fill_bottle_module)){
				p_fill_bottle_module->fill_bottle_state = CLOSE_SHIFT_BOTTLE;
			}
			break;
		case CLOSE_SHIFT_BOTTLE:
			fill_close_shift_bottle (p_fill_bottle_module);
//			p_fill_bottle_module->fill_bottle_state = DOWN_NOZZLE;
//			break;
//		case DOWN_NOZZLE:
			fill_down_nozzle_no_delay (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = FILL_BOTTLE_FINISHED;
			break;
		case FILL_BOTTLE_FINISHED:
			if (counter_env.set_pre_open_door_time == 0){//如果禁用小料门提前打开
				fill_reset_fill_bottle_complete (p_fill_bottle_module);
			}
			fill_bottle_finished (p_fill_bottle_module);
			break;
		default:break;
	}
}
//
void fill_bottle_sys_use_big_gate_process (s_fill_bottle_module * p_fill_bottle_module, const uint16_t input_map)//不使用总料门装瓶步骤
{
	////////////////////////////////////////////////////////////////////////////
	p_fill_bottle_module->input_buf = input_map;
	fill_sync_vibrator_signal (p_fill_bottle_module);
	////////////////////////////////////////////////////////////////////////////
	fill_common_op (p_fill_bottle_module);
	if (p_fill_bottle_module->fill_bottle_op_delay > 0){
		p_fill_bottle_module->fill_bottle_op_delay--;
		if (p_fill_bottle_module->fill_bottle_op_delay > 0){
			return;
		}
	}
	switch (p_fill_bottle_module->fill_bottle_state)
	{
		case STANDBY:
//			if (GET_BIT (p_fill_bottle_module->output_buf, OUTPUT_BUF_BIG_GATE_DOOR) == 0){
//				inc_Modbus_test_value (100);
//			}
			fill_bottle_standby_common_op (p_fill_bottle_module);
			//fill_close_big_gate_door_no_delay (p_fill_bottle_module);
			if (GET_BIT (p_fill_bottle_module->input_buf, OUTPUT_BUF_COMPLETE) == 0){//数粒完成
				p_fill_bottle_module->fill_bottle_state = OPEN_BIG_GATE;
				p_fill_bottle_module->fill_bottle_op_delay = SET_PIECE_FALL_TIME - p_fill_bottle_module->pre_count_complete_ctr;//备料时间
				p_fill_bottle_module->pre_count_complete_ctr = 0;
			}else{
				break;
			}
		case OPEN_BIG_GATE:
			if (p_fill_bottle_module->pre_open_big_gate_flag == PRE_BIG_GATE_INIT){
				fill_open_big_gate_door (p_fill_bottle_module);
				p_fill_bottle_module->fill_bottle_state = CLOSE_BIG_GATE;
			}else if (p_fill_bottle_module->pre_open_big_gate_flag == PRE_BIG_GATE_OPEN_ING){
				p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_OPEN_BIG_GATE_DOOR_DELAY - p_fill_bottle_module->pre_open_big_gate_ctr;
				p_fill_bottle_module->fill_bottle_state = CLOSE_BIG_GATE;
			}else if (p_fill_bottle_module->pre_open_big_gate_flag == PRE_BIG_GATE_OPEN_ED){
				p_fill_bottle_module->fill_bottle_state = CLOSE_BIG_GATE;
			}else if (p_fill_bottle_module->pre_close_big_gate_flag == PRE_BIG_GATE_CLOSE_ING){
				p_fill_bottle_module->fill_bottle_op_delay = SET_FILL_CLOSE_BIG_GATE_DOOR_DELAY - p_fill_bottle_module->pre_close_big_gate_ctr;
				p_fill_bottle_module->fill_bottle_state = READY_TO_SHIFT_BOTTLE;
			}else if (p_fill_bottle_module->pre_close_big_gate_flag == PRE_BIG_GATE_CLOSE_ED){
				p_fill_bottle_module->fill_bottle_state = READY_TO_SHIFT_BOTTLE;
			}else{
				fill_open_big_gate_door (p_fill_bottle_module);
				p_fill_bottle_module->fill_bottle_state = CLOSE_BIG_GATE;
			}
			p_fill_bottle_module->pre_open_big_gate_flag = PRE_BIG_GATE_INIT;
			p_fill_bottle_module->pre_close_big_gate_flag = PRE_BIG_GATE_INIT;
			break;
		case CLOSE_BIG_GATE:
			fill_close_big_gate_door (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = READY_TO_SHIFT_BOTTLE;
			break;
		case READY_TO_SHIFT_BOTTLE://准备换下一个瓶子
//			if (GET_BIT (p_fill_bottle_module->output_buf, OUTPUT_BUF_BIG_GATE_DOOR) == 0){
//				inc_Modbus_test_value (1000);
//			}
//			fill_close_big_gate_door_no_delay (p_fill_bottle_module);
			fill_ready_to_shift_bottle (p_fill_bottle_module);
			fill_reset_fill_bottle_complete (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = WAIT_PIECE_FALL;
			break;
		case WAIT_PIECE_FALL://装料操作
			fill_wait_piece_fall (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = UP_NOZZLE;
			break;
		case UP_NOZZLE://升料嘴
			fill_stop_nozzle_vib (p_fill_bottle_module);
			fill_up_nozzle (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = OPEN_SHIFT_BOTTLE;
			break;
		case OPEN_SHIFT_BOTTLE:
			fill_open_shift_bottle (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = WAIT_SCREW_READY;
			break;
		case WAIT_SCREW_READY:
			if (poll_fill_complete_status (p_fill_bottle_module)){
				p_fill_bottle_module->fill_bottle_state = CLOSE_SHIFT_BOTTLE;
			}
			break;
		case CLOSE_SHIFT_BOTTLE:
			fill_close_shift_bottle (p_fill_bottle_module);
//			p_fill_bottle_module->fill_bottle_state = DOWN_NOZZLE;
//			break;
//		case DOWN_NOZZLE:
			fill_down_nozzle_no_delay (p_fill_bottle_module);
			p_fill_bottle_module->fill_bottle_state = FILL_BOTTLE_FINISHED;
			break;
		case FILL_BOTTLE_FINISHED:
			fill_bottle_finished (p_fill_bottle_module);
			break;
		default:break;
	}
}
//

void fill_bottle_system_module_poll (s_fill_bottle_module * p_fill_bottle_module, const uint16_t input_map)
{	
	if (p_fill_bottle_module->first_run == FIRST_RUN){//上电复位
		p_fill_bottle_module->first_run = !FIRST_RUN;
		fill_bottle_module_reset ();
		return;
	}
	////////////////////////////////////////////////////////////////////////////////
	if (my_env.is_registered != REGISTERED){
		return;
	}
	if (counter_env.is_master){
		if (p_fill_bottle_module->total_good >= counter_env.set_product_num){
			counter_env.procuct_num_reach = 1;
			if (SYSTEM_START_SWITCH == 0){//停机
				SYSTEM_START_SWITCH = 1;
			}
		}
	}
	if (p_fill_bottle_module->continue_reject_ctr >= SET_FILL_CONTINUE_REJECT_STOP_NUM){
		counter_env.continue_reject_flag = 1;
		if (SYSTEM_START_SWITCH == 0){//停机
			SYSTEM_START_SWITCH = 1;
		}
	}
	
	if (counter_env.system_signal == SYSTER_RESET){
		p_fill_bottle_module->continue_reject_ctr = 0;
		p_fill_bottle_module->enable_big_gate = p_fill_bottle_module->set_para.set_enable_big_gate;
		p_fill_bottle_module->fill_bottle_state = STANDBY;		
		if (p_fill_bottle_module->enable_big_gate == MY_ENABLE){
			fill_close_big_gate_door_no_delay (p_fill_bottle_module);
		}else{
			fill_open_big_gate_door_no_delay (p_fill_bottle_module);
		}
	}
	if (p_fill_bottle_module->enable_big_gate == MY_ENABLE){
		fill_bottle_sys_use_big_gate_process (p_fill_bottle_module, input_map);//使用总料门
	}else{
		fill_bottle_sys_unuse_big_gate_process (p_fill_bottle_module, input_map);//不使用使用总料门
	}
}


//

void counter_tech_lib_info (void)
{
	my_println ("Counter_Tech_Lib Version 1.0");
}










