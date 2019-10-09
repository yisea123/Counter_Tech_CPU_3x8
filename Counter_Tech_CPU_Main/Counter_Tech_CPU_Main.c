#include "main.h"



s_counter_module counter_module[COUNTER_MODULE_NUM];
s_fill_bottle_module fill_bottle_module[FILL_BOTTLE_MODULE_NUM];
s_counter_env counter_env;
uint16_t counter_input_signal_1 = 0xFFFF;
uint16_t counter_input_signal_2 = 0xFFFF;



//s_counter_env counter_env;

void counter_init (void)
{
	uint32_t i, j;
	S8 *p;
	for (j = 0; j < 2; j++){
		p = (S8 *) &counter_module[j];
		for (i = 0; i < sizeof(s_counter_module); i++){
			*(p++) = 0;
		}
	}
	sys_run_time.data_hl = 0;
	counter_env.screw_running = 1;
}

uint16_t get_rand(uint16_t X, uint16_t Y)
{
	srand (get_tim5_ticks());
	//set_Modbus_test_value();
	return (rand()%(Y-X+1)+X);
}
void input_sim_ch (const uint16_t input_map, uint16_t ch)
{
	#define VIB_DELAY_STOP_X 8
	#define VIB_DELAY_STOP_Y 32
	#define VIB_DELAY_START 500
	#define LENGTH_X 		counter_module[0].set_para.set_sim_length_X
	#define LENGTH_Y 		counter_module[0].set_para.set_sim_length_Y
	#define INTERVAL_X 	counter_module[0].set_para.set_sim_interval_X
	#define INTERVAL_Y 	counter_module[0].set_para.set_sim_interval_Y
	static uint16_t vib_delay_stop = 0;
	static uint16_t vib_delay_start = 0;
	static uint16_t delay_op[CHANEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static uint16_t status[CHANEL_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint16_t tmp;
	if (delay_op[ch] > 0){
		delay_op[ch]--;
		if (delay_op[ch] > 0){
			return;
		}
	}
	tmp = rand_tmp;
	switch (status[ch])
	{
		case 0:
			status[ch] = 1;
			delay_op[ch] = get_rand (INTERVAL_X, INTERVAL_Y);
			SET_OUTPUT_OFF (&tmp, (0x01 << ch));
			if (counter_env.sim_too_close_flag == 1){
				counter_env.sim_too_close_flag = 0;
				delay_op[ch] >>= 1;
			}
			break;
		case 1:
			if (vib_delay_start > 0){
				vib_delay_start--;
			}else{
				if ((GET_BIT (input_map, OUTPUT_BUF_VIB) == 0)){
					vib_delay_stop = get_rand (VIB_DELAY_STOP_X, VIB_DELAY_STOP_Y);;
					status[ch] = 0;
					delay_op[ch] = get_rand (LENGTH_X, LENGTH_Y);
					SET_OUTPUT_ON (&tmp, (0x01 << ch));
					if (counter_env.sim_too_long_flag == 1){
						counter_env.sim_too_long_flag = 0;
						delay_op[ch] *= 2;
					}
				}else{
					if (vib_delay_stop > 0){
						vib_delay_stop--;
						status[ch] = 0;
						delay_op[ch] = get_rand (LENGTH_X, LENGTH_Y);
						SET_OUTPUT_ON (&tmp, (0x01 << ch));
					}else{
						vib_delay_start = VIB_DELAY_START;
					}
				}
			}
			break;
		default:
			break;
	}
	rand_tmp = tmp;
}
void input_sim (const uint16_t input_map)
{
	uint16_t i;
	for (i = 0; i < CHANEL_NUM; i++){
		input_sim_ch(input_map, i);
	}
}
void input_output_test (void)
{
	#define MAP_INPUT_TO_OUTPUT(CH){ \
		EXT_OUT##CH = !EXT_IN##CH; \
	}
	MAP_INPUT_TO_OUTPUT(0);
	MAP_INPUT_TO_OUTPUT(1);
	MAP_INPUT_TO_OUTPUT(2);
	MAP_INPUT_TO_OUTPUT(3);
	MAP_INPUT_TO_OUTPUT(4);
	MAP_INPUT_TO_OUTPUT(5);
	MAP_INPUT_TO_OUTPUT(6);
	MAP_INPUT_TO_OUTPUT(7);
	MAP_INPUT_TO_OUTPUT(8);
	MAP_INPUT_TO_OUTPUT(9);
	MAP_INPUT_TO_OUTPUT(10);
	MAP_INPUT_TO_OUTPUT(11);
	MAP_INPUT_TO_OUTPUT(12);
	MAP_INPUT_TO_OUTPUT(13);
	MAP_INPUT_TO_OUTPUT(14);
	MAP_INPUT_TO_OUTPUT(15);
	MAP_INPUT_TO_OUTPUT(16);
	MAP_INPUT_TO_OUTPUT(17);
	MAP_INPUT_TO_OUTPUT(18);
	MAP_INPUT_TO_OUTPUT(19);
	MAP_INPUT_TO_OUTPUT(20);
	MAP_INPUT_TO_OUTPUT(21);
	MAP_INPUT_TO_OUTPUT(22);
	MAP_INPUT_TO_OUTPUT(23);
	MAP_INPUT_TO_OUTPUT(24);
	MAP_INPUT_TO_OUTPUT(25);
	MAP_INPUT_TO_OUTPUT(26);
	MAP_INPUT_TO_OUTPUT(27);
	MAP_INPUT_TO_OUTPUT(28);
	MAP_INPUT_TO_OUTPUT(29);
	MAP_INPUT_TO_OUTPUT(30);
	MAP_INPUT_TO_OUTPUT(31);
}
//
void copy_bit_to_bit (uint16_t *value, uint16_t dst_mask_bit, uint16_t src_mask_bit)
{
	if ((*value) & src_mask_bit){
		(*value) |= dst_mask_bit;
	}else{
		(*value) &= (~dst_mask_bit);
	}
}
//
UINT16_INPUT_FILTER_FUNC (EMERGENCY_STOP_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (CONVEYOR_STOP_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (POWER_OFF_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (PRESSURE_LOW_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (VIB_UNLOCK_CH, 10, 0);//
UINT16_INPUT_FILTER_FUNC (DETECT_ERROR_CH, 10, 1);
UINT16_INPUT_FILTER_FUNC (PRODUCT_NUM_REACH_CH, 10, 1);
UINT16_INPUT_FILTER_FUNC (CONTINUE_REJ_STOP_CH, 10, 1);
UINT16_INPUT_FILTER_FUNC (BOTTLE_FALL_1_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (BOTTLE_FALL_2_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (VIB_1_OFF_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (VIB_2_OFF_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (BOTTLE_JAM_1_CH, 500, 0);
UINT16_INPUT_FILTER_FUNC (BOTTLE_JAM_2_CH, 500, 0);
UINT16_INPUT_FILTER_FUNC (LIFTER_UP_LIMIT_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (LIFTER_DOWN_LIMIT_CH, 500, 1);
UINT16_INPUT_FILTER_FUNC (FRONT_BOTTLE_JAM_CH, 1000, 0);

void update_system_alarm_status (void)
{
	UPDATE_ALARM_STATUS (EMERGENCY_STOP_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (CONVEYOR_STOP_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (POWER_OFF_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (PRESSURE_LOW_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (VIB_UNLOCK_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (DETECT_ERROR_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (PRODUCT_NUM_REACH_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (CONTINUE_REJ_STOP_CH, &counter_env.system_alarm_high);
	UPDATE_ALARM_STATUS (VIB_1_OFF_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (VIB_2_OFF_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (BOTTLE_FALL_1_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (BOTTLE_FALL_2_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (BOTTLE_JAM_1_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (BOTTLE_JAM_2_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (LIFTER_UP_LIMIT_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (LIFTER_DOWN_LIMIT_CH, &counter_env.system_alarm_low);
	UPDATE_ALARM_STATUS (FRONT_BOTTLE_JAM_CH, &counter_env.system_alarm_low);
	///////////////////////////////////////////////////////////////////////
	copy_bit_to_bit (&counter_env.system_alarm_low_enable, ALM_BOTTLE_FALL_2_CH, ALM_BOTTLE_FALL_1_CH);
	copy_bit_to_bit (&counter_env.system_alarm_low_enable, ALM_BOTTLE_JAM_2_CH, ALM_BOTTLE_JAM_1_CH);
	counter_env.system_alarm_low |= counter_env.system_alarm_low_enable;
	counter_env.system_alarm_high |= counter_env.system_alarm_high_enable;
	///////////////////////////////////////////////////////////////////////
	if (counter_env.system_alarm_low != ALM_NORMAL_STATUS || 
			counter_env.system_alarm_high != ALM_NORMAL_STATUS){//黄灯闪烁
		counter_env.led_yellow_flash_ctr++;
		if (counter_env.led_yellow_flash_ctr == 500){
			counter_env.led_yellow_flash_ctr = 0;
			counter_env.led_yellow = !counter_env.led_yellow;
		}
	}else{
		counter_env.led_yellow_flash_ctr = 0;
		counter_env.led_yellow = 1;
	}
	///////////////////////////////////////////////////////////////////////
	if (counter_env.system_alarm_low != ALM_NORMAL_STATUS || 
			counter_env.system_alarm_high != ALM_NORMAL_STATUS){//蜂鸣器闪烁
		if (counter_env.beep_flash_ctr < 10000){
			counter_env.beep_flash_ctr++;
			if ((counter_env.beep_flash_ctr % 500) == 0){
				counter_env.beep = !counter_env.beep;
			}
		}
	}else{
		counter_env.beep_flash_ctr = 0;
		counter_env.beep = 1;
	}
	///////////////////////////////////////////////////////////////////////
}
uint16_t get_stop_machine_alarm_status (void)
{
	uint16_t r_code = 0;
	if (counter_env.system_alarm_high == ALM_NORMAL_STATUS){
		r_code = 0;
	}else{
		r_code = 1;
	}
	return r_code;
}
//
#define MAP_INPUT_POS(IN,DST,SRC) IN##SRC << (DST < 16 ? DST: SRC)
void map_input_signal (void)
{
	counter_env.sys_config_info = MAP_INPUT_POS (CONFIG_INPUT, 0, 0) |
																MAP_INPUT_POS (CONFIG_INPUT, 1, 1) |
																MAP_INPUT_POS (CONFIG_INPUT, 2, 2) |
																MAP_INPUT_POS (CONFIG_INPUT, 3, 3);
//	counter_input_signal_1 = MAP_INPUT_POS(COUNTER_INPUT, 0, 2) | 
//												 MAP_INPUT_POS(COUNTER_INPUT, 1, 2) |
//												 MAP_INPUT_POS(COUNTER_INPUT, 2, 2);
	if ((CONFIG_INPUT2 == 1) && (CONFIG_INPUT3 == 0)){//兼容珐玛珈计数电眼
		counter_input_signal_1 = MAP_INPUT_POS(EXT_IN, 0, 0) |
													 MAP_INPUT_POS(EXT_IN, 1, 1) |
													 MAP_INPUT_POS(EXT_IN, 2, 2) |
													 MAP_INPUT_POS(EXT_IN, 3, 3) |
													 MAP_INPUT_POS(EXT_IN, 4, 4) |
													 MAP_INPUT_POS(EXT_IN, 5, 5) |
													 MAP_INPUT_POS(EXT_IN, 6, 6) |
													 MAP_INPUT_POS(EXT_IN, 7, 7);
		counter_input_signal_2 = MAP_INPUT_POS(EXT_IN, 0, 8) |
													 MAP_INPUT_POS(EXT_IN, 1, 9) |
													 MAP_INPUT_POS(EXT_IN, 2, 10) |
													 MAP_INPUT_POS(EXT_IN, 3, 11) |
													 MAP_INPUT_POS(EXT_IN, 4, 12) |
													 MAP_INPUT_POS(EXT_IN, 5, 13) |
													 MAP_INPUT_POS(EXT_IN, 6, 14) |
													 MAP_INPUT_POS(EXT_IN, 7, 15);
	}else if ((CONFIG_INPUT2 == 0) && (CONFIG_INPUT3 == 1)){
		counter_input_signal_1 = 0xFF00 | rand_tmp;//调试模拟数据
		counter_input_signal_2 = (0x00FF | rand_tmp) >> 8;//调试模拟数据
	}else{//使用定制电眼
		counter_input_signal_1 = MAP_INPUT_POS(COUNTER_INPUT, 16, 0) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 1) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 2) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 3) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 4) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 5) | 
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 6) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 7) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 8) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 9) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 10) |
													 MAP_INPUT_POS(COUNTER_INPUT, 16, 11);
		counter_input_signal_2 = 0xFFFF;
	}
	counter_input_signal_1 |= 0xFF00;//8轨;
	counter_input_signal_2 |= 0xFF00;//8轨;
}
//
void clear_alarm (void)
{
	uint16_t i;
	for (i = 0; i < FILL_BOTTLE_MODULE_NUM; i++){
		fill_bottle_module[i].continue_reject_ctr = 0;
	}
	counter_env.beep_flash_ctr = 0;
	counter_env.detect_error = 0;
	counter_env.continue_reject_flag = 0;
	counter_env.procuct_num_reach = 0;
	counter_env.clear_alarm = 0;
}
//
void ext_fill_bottle_counter (uint16_t * pdata)
{	
	if (EXT_FILL_BOTTLE_SIGNAL == 0){//外部装瓶计数脉冲
		if (counter_env.ext_fill_b_t < 15){
			counter_env.ext_fill_b_t++;
		}
		if (counter_env.ext_fill_b_t == 10){	
			//外部装瓶合格计数脉冲	
			counter_env.ext_total_good++;
			counter_env.ext_fill_b_signal_on = 0;
		}else{
		}
	}else{
		counter_env.ext_fill_b_signal_on = 1;
		counter_env.ext_fill_b_t = 0;
	}
}
//
void screw_motor_pwm_process (void)
{
	static uint16_t bit_pwm = 0;
	static uint16_t pwm_ctr = 0;
	if (counter_env.set_servo_motor_speed < 1){
		counter_env.set_servo_motor_speed = 1;
	}else if (counter_env.set_servo_motor_speed > 100){
		counter_env.set_servo_motor_speed = 100;
	}
	if (pwm_ctr == (100 - counter_env.set_servo_motor_speed)){
		pwm_ctr = 0;
		if (counter_env.servo_motor_pulse_num > 0){
			if (bit_pwm == 0){
				bit_pwm = 1;
			}else{
				counter_env.servo_motor_pulse_num--;
				bit_pwm = 0;
			}
			//MAP_SIG_TO_EXT_OUTPUT (31, bit_pwm);//
			MAP_SIG_TO_EXT_OUTPUT (32, bit_pwm);//
		}
	}else{
		pwm_ctr++;
	}
}
//
void screw_shift_bottle (uint16_t input_low)
{
	static uint16_t speed_tmp = 0;
	static uint16_t speed_time_ctr = 0;
		speed_time_ctr++;
	if (speed_time_ctr == 20000){
		speed_time_ctr = 0;
		counter_env.speed = speed_tmp * 3;
		speed_tmp = 0;
	}
	if (counter_env.system_signal == SYSTER_RESET){
		counter_env.screw_shift_bottle_status = 0;
		counter_env.screw_running = 1;
	}
	if ((counter_env.servo_motor_shift_bottle_time == 1) && (counter_env.servo_motor_pulse_num == 0)){
		counter_env.servo_motor_shift_bottle_time = 0;
	}
	if(counter_env.is_master){
		if (counter_env.servo_motor_shift_bottle_time > 1){
			counter_env.servo_motor_shift_bottle_time--;
			counter_env.screw_running = 0;
		}else if (counter_env.servo_motor_pulse_num == 0){
			counter_env.screw_running = 1;
		}
		switch (counter_env.screw_shift_bottle_status)
		{
			case 0:
				if (input_low == 0){
					counter_env.servo_motor_shift_bottle_time = SET_FILL_SERVO_MOTOR_SHIFT_BOTTLE_DELAY+1;
					counter_env.servo_motor_pulse_num = SET_FILL_SERVO_MOTOR_PULSE_NUM;
					counter_env.screw_shift_bottle_status = 1;
					speed_tmp += counter_env.set_servo_motor_shift_bottle_num;
				}
				break;
			case 1:
				if ((counter_env.servo_motor_shift_bottle_time == 0) && (counter_env.servo_motor_pulse_num == 0)){
					counter_env.screw_shift_bottle_status = 2;
				}
				break;
			case 2:
				if (input_low == (SCREW_MODULE_1_EN) | (SCREW_MODULE_2_EN << 1) | (SCREW_MODULE_3_EN << 2)){
					counter_env.screw_shift_bottle_status = 0;
				}
				break;
			default:break;
		}
	}
}
//
void sync_parameter (void)
{	
	memcpy (&counter_module[1].set_para, &counter_module[0].set_para, sizeof (s_counter_set_para));
	memcpy (&fill_bottle_module[1].set_para, &fill_bottle_module[0].set_para, sizeof (s_fill_bottle_set_para));
}
//
void counter_task_poll (void)
{
	static uint16_t tick_old;
	uint16_t period;
	uint16_t tick_new;
	uint16_t input_high;
	uint16_t input_low;
	
	////////////////////////////////////////////////////////////////////////////////
	period = get_tim5_ticks();
	if (period > tick_old){
		counter_env.scan_period = period - tick_old;
		if (counter_env.min_scan_period > counter_env.scan_period){
			counter_env.min_scan_period = counter_env.scan_period;
		}
		if (counter_env.max_scan_period < counter_env.scan_period){
			counter_env.max_scan_period = counter_env.scan_period;
		}
	}
	tick_old = period;
	////////////////////////////////////////////////////////////////////////////////
	if (counter_env.auto_scan == 1){
		if (counter_env.mannual_scan_time == 0){
			return;
		}else{
			if (counter_env.mannual_scan_time > 0){
				counter_env.mannual_scan_time--;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	sys_run_time.data_hl++;
	////////////////////////////////////////////////////////////////////////////////
	map_input_signal ();
	////////////////////////////////////////////////////////////////////////////////
	if ((CONFIG_INPUT2 == 0) && (CONFIG_INPUT3 == 0)){
		input_output_test ();
		return;
	}
	////////////////////////////////////////////////////////////////////////////////
	sync_parameter ();//同步参数
	////////////////////////////////////////////////////////////////////////////////
	update_system_alarm_status ();//更新报警状态信息，如倒瓶、堵瓶等
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	if (counter_env.clear_monitor_data == 1){
		counter_data_clear ();
	}
	if (counter_env.clear_product_data == 1){
		counter_product_data_clear ();
	}
	if (counter_env.clear_alarm == 1){
		clear_alarm ();
	}
	////////////////////////////////////////////////////////////////////////////////
	if (counter_env.is_master){
	}else{
		SYSTEM_START_SWITCH = EXT_RUNNING_SIGNAL;
	}
	if (get_stop_machine_alarm_status() != 0){
		if (SYSTEM_START_SWITCH == 0){
			SYSTEM_START_SWITCH = 1;
			clear_alarm ();
		}
	}
	if (GET_BIT(counter_env.system_alarm_high, ALM_EMERGENCY_STOP_CH) == 0){//拍急停
		if (CONVEYOR_START == 0){//停输送带
			CONVEYOR_START = 1;
			counter_env.beep_flash_ctr = 0;
		}
		if (SYSTEM_START_SWITCH == 0){//停机
			SYSTEM_START_SWITCH = 1;
		}
	}
	/////////////////////////////////////////////////////////////////////////
	switch (counter_env.system_signal)
	{
		case SYSTER_STOP:
			if (SYSTEM_START_SWITCH == 0){
				counter_env.system_signal = SYSTER_RESET;
				counter_pre_start ();//预运行，做一些准备工作
				counter_env.servo_motor_shift_bottle_time = SET_FILL_SERVO_MOTOR_SHIFT_BOTTLE_DELAY+1;
				counter_env.servo_motor_pulse_num = SET_FILL_SERVO_MOTOR_PULSE_NUM;
			}else{
				if (counter_env.counter_stop_idle_time < 60000){
					counter_env.counter_stop_idle_time++;
				}
			}
			counter_env.counter_start_delay = 0;
			break;
		case SHIFT_A_BOTTLE:
			if ((counter_env.servo_motor_shift_bottle_time == 1) && (counter_env.servo_motor_pulse_num == 0)){
				counter_env.servo_motor_shift_bottle_time = 0;
				counter_env.system_signal = SYSTER_RESET;
			}
			break;
		case SYSTER_RESET:
			counter_env.counter_start_delay++;
			counter_env.counter_stop_idle_time = 0;
			if (counter_env.counter_start_delay > 1000){
			}
			counter_env.system_signal = SYSTER_RUNNING;
			break;
		case SYSTER_RUNNING:
			if (SYSTEM_START_SWITCH == 1){
				counter_env.system_signal = SYSTER_PRE_STOP;
			}
			break;
		case SYSTER_PRE_STOP://预停机
			counter_stop ();
			counter_env.system_signal = SYSTER_STOP;
			break;
		default:break;
	}
	////////////////////////////////////////////////////////////////////////////////
	//开始数粒逻辑处理
	////////////////////////////////////////////////////////////////////////////////
	ext_fill_bottle_counter (&input_low);
	////////////////////////////////////////////////////////////////////////////////
	if (counter_module[0].first_run == FIRST_RUN){//上电复位
		counter_module[0].first_run = !FIRST_RUN;
		counter_reset ();
	}
	////////////////////////////////////////////////////////////////////////////////
	//数粒装瓶模块1
	if (counter_env.auto_scan == 0){
		input_low = counter_input_signal_1;
	}else{
		input_low = counter_module[0].input_buf_low;
	}
	input_high = fill_bottle_module[0].output_buf;
	counter_module_poll_chanel (&counter_module[0], input_high, input_low);
	input_low = (counter_module[0].output_buf_high);
	if (VIBRATOR_SWITCH_1 == 1 ||
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_FALL_1_CH) == 0 || 
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_JAM_1_CH) == 0){
			SET_OUTPUT_OFF (&input_low, OUTPUT_BUF_VIB);
	}
	if ((SCREW_READY_SIGNAL == 0) || (SIM_SCREW_READY_SIGNAL == 0)){
		SET_OUTPUT_ON (&input_low, OUTPUT_BUF_SCREW_RD);
	}else{
		SET_OUTPUT_OFF (&input_low, OUTPUT_BUF_SCREW_RD);
	}
	fill_bottle_system_module_poll (&fill_bottle_module[0], input_low);
	////////////////////////////////////////////////////////////////////////////////
	//模拟振动器
	if (counter_env.system_signal == SYSTER_RUNNING){
		if (counter_env.auto_scan == 0){
			input_sim (input_low);
		}
	}else{
		rand_tmp = 0xFFFF;
	}
	////////////////////////////////////////////////////////////////////////////////	
	//数粒装瓶模块2
	if (counter_env.auto_scan == 0){
		input_low = counter_input_signal_2;
	}else{
		input_low = counter_module[1].input_buf_low;
	}
	input_high = fill_bottle_module[1].output_buf;
	counter_module_poll_chanel (&counter_module[1], input_high, input_low);
	input_low = (counter_module[1].output_buf_high);
	if (VIBRATOR_SWITCH_2 == 1 ||
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_FALL_2_CH) == 0 || 
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_JAM_2_CH) == 0){
			SET_OUTPUT_OFF (&input_low, OUTPUT_BUF_VIB);
	}
	if ((SCREW_READY_SIGNAL == 0) || (SIM_SCREW_READY_SIGNAL == 0)){
		SET_OUTPUT_ON (&input_low, OUTPUT_BUF_SCREW_RD);
	}else{
		SET_OUTPUT_OFF (&input_low, OUTPUT_BUF_SCREW_RD);
	}
	fill_bottle_system_module_poll (&fill_bottle_module[1], input_low);
	////////////////////////////////////////////////////////////////////////////////
	//螺杆放瓶信号处理
	input_low = 0;
	if (GET_BIT(fill_bottle_module[0].output_buf, OUTPUT_BUF_BOTTLE_SHIFT) == 0){
		SET_OUTPUT_ON (&input_low, 0x01);
	}else{
		SET_OUTPUT_OFF (&input_low, 0x01);
	}
	if (GET_BIT(fill_bottle_module[1].output_buf, OUTPUT_BUF_BOTTLE_SHIFT) == 0){
		SET_OUTPUT_ON (&input_low, 0x02);
	}else{
		SET_OUTPUT_OFF (&input_low, 0x02);
	}
	if (counter_env.ext_fill_b_signal_on == 0){
		SET_OUTPUT_ON (&input_low, 0x04);
	}else{
		SET_OUTPUT_OFF (&input_low, 0x04);
	}
	if ((SCREW_MODULE_1_EN) | (SCREW_MODULE_2_EN) | (SCREW_MODULE_3_EN)){//至少使能1个模块
		input_low &= ((SCREW_MODULE_1_EN) | (SCREW_MODULE_2_EN << 1) | (SCREW_MODULE_3_EN << 2));
		screw_shift_bottle (input_low);
	}
	////////////////////////////////////////////////////////////////////////////////
	map_output_signal ();
	////////////////////////////////////////////////////////////////////////////////
	tick_new = get_tim5_ticks();
	if (tick_new > tick_old){
		counter_env.scan_process_time = tick_new - tick_old + 2;
	}
	if (counter_env.min_scan_process_time > counter_env.scan_process_time){
		counter_env.min_scan_process_time = counter_env.scan_process_time;
	}
	if (counter_env.max_scan_process_time < counter_env.scan_process_time){
		counter_env.max_scan_process_time = counter_env.scan_process_time;
	}
	////////////////////////////////////////////////////////////////////////////////
}
//
void counter_reset (void)
{
	uint16_t i, j;
	for (i = 0; i < COUNTER_MODULE_NUM; i++){
		for (j = 0; j < CHANEL_NUM; j++){
			counter_module[i].chanel[j].input_status_new = 1;
			counter_module[i].chanel[j].input_status_old = 1;
		}
		switch_to_normal_count (&counter_module[i]);
		reset_count_complete (&counter_module[i]);

		counter_module[i].input_buf_high = 0xFFFF;
		counter_module[i].input_buf_low = 0xFFFF;
		counter_module[i].output_buf_high = 0xFFFF;
		counter_module[i].output_buf_low = 0xFFFF;
		counter_module[i].max_length = 0;
		counter_module[i].min_length = 999;
		counter_module[i].min_piece_interval = 999;
		counter_module[i].min_close_door_interval = 999;
		counter_module[i].min_door_switch_interval = 999;

		//counter_module[i].alarm_low = ALM_NORMAL_STATUS;
		counter_module[i].output_buf_low_manual = 0xFFFF;
		//start_vibrator (&counter_module[i]);	
		/////////////////////////////设置报警屏蔽开关////////////////////////////////////		
		if ((CONFIG_INPUT2 == 0) && (CONFIG_INPUT3 == 1)){//调试模式
		}else{
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	my_env.modbus_rtu_addr_offset = ((~counter_env.sys_config_info) & 0x3);
	my_env.modbus_rtu_addr = MODBUS_RTU_ADDR + my_env.modbus_rtu_addr_offset;
	counter_env.is_master = ((my_env.modbus_rtu_addr_offset == 0) ? 1 : 0);
	/////////////////////////////设置报警屏蔽开关////////////////////////////////////		
	if ((CONFIG_INPUT2 == 0) && (CONFIG_INPUT3 == 1)){//调试模式
		counter_env.system_alarm_high_enable = 0x000F;//调试模式
		counter_env.system_alarm_low_enable = 0x0001 | 0x0010 | 0x0008;
	}else{
		counter_env.system_alarm_low_enable = 0x0000 | 0x0010 | 0x0008;
		if (counter_env.is_master){
			counter_env.system_alarm_high_enable = 0x000E;//
		}else{
			counter_env.system_alarm_high_enable = 0x000E;//调试模式
		}
	}
	counter_env.system_alarm_high = ALM_NORMAL_STATUS;
	counter_env.system_alarm_low = ALM_NORMAL_STATUS;
	counter_env.auto_scan = 0;
	counter_env.system_signal = SYSTER_STOP;
	VIBRATOR_SWITCH_1 = 0;
	VIBRATOR_SWITCH_2 = 0;
}
//
void counter_pre_start (void)
{
	uint16_t i;
	counter_env.counter_start_delay = 2000;//2s
	for (i = 0; i < COUNTER_MODULE_NUM; i++){
		counter_module[i].output_buf_high = 0xFFFF;
		counter_module[i].output_buf_low = 0xFFFF;
		counter_module[i].normal_count = SET_COUNT;//开机先放走一瓶
//		counter_module[i].normal_count = 0;//开机先放走一瓶
//		counter_module[i].pre_count = 0;
	}
//	for (i = 0; i < FILL_BOTTLE_MODULE_NUM; i++){
//		fill_down_nozzle (&fill_bottle_module[i]);//开机先把料嘴放下去
//	}
}
//
void counter_stop (void)
{
	uint16_t i;
	for (i = 0; i < COUNTER_MODULE_NUM; i++){
		counter_module[i].output_buf_low_manual = counter_module[i].output_buf_low;
		counter_module[i].start_rej = 0;;
	}
//	for (i = 0; i < FILL_BOTTLE_MODULE_NUM; i++){
//		fill_up_nozzle (&fill_bottle_module[i]);//停机后料嘴升上去
//	}
}
//
void counter_product_data_clear (void)
{
	uint16_t i;
	for (i = 0; i < FILL_BOTTLE_MODULE_NUM; i++){
		fill_bottle_module[i].total_good = 0;
		fill_bottle_module[i].total_reject= 0;
	}
	counter_env.ext_total_good = 0;
	counter_env.clear_product_data = 0;
	counter_env.procuct_num_reach = 0;
}
//
void counter_data_clear (void)
{
	uint16_t i, j;
	counter_env.clear_monitor_data = 0;
	for (i = 0; i < COUNTER_MODULE_NUM; i++){
		for (j = 0; j < CHANEL_NUM; j++){
			counter_module[i].max_length = 0;
			counter_module[i].min_length = 999;
			counter_module[i].min_piece_interval = 999;
			counter_module[i].min_close_door_interval = 999;
			counter_module[i].min_door_switch_interval = 999;
			counter_module[i].chanel[j].length = 0;
		}
	}
}
//
void test_counter_module (void)
{
	s_counter_module * p_counter_module;
	p_counter_module = &counter_module[0];
	
	p_counter_module->output_buf_high = OUTPUT_ALL_OFF;
	p_counter_module->output_buf_low = OUTPUT_ALL_OFF;
	start_vibrator (p_counter_module);
	stop_vibrator (p_counter_module);
}
//

void fill_bottle_module_reset (void)
{
	uint16_t i;
	for (i = 0; i < FILL_BOTTLE_MODULE_NUM; i++){
		fill_bottle_module[i].enable_big_gate = fill_bottle_module[i].set_para.set_enable_big_gate;
		fill_bottle_module[i].fill_bottle_op_delay = 0;
		fill_bottle_module[i].input_buf = 0xFFFF;
		fill_bottle_module[i].output_buf = 0xFFFF;
		fill_bottle_module[i].nozzle_vib_status = MY_DISABLE;
		fill_bottle_module[i].reject_keep_delay = 0;
		fill_bottle_module[i].big_gate_delay = 0;
		fill_bottle_module[i].nozzle_up_delay = 0;
		fill_bottle_module[i].shift_bottle_delay = 0;
		/////////////////////////////////////////////////////////////////////
		//初始化个气缸状态
		fill_reset_fill_bottle_complete (&fill_bottle_module[i]);
		fill_close_shift_bottle (&fill_bottle_module[i]);
		fill_up_nozzle (&fill_bottle_module[i]);//开机料嘴默认上升状态
		fill_stop_reject (&fill_bottle_module[i]);
		if (fill_bottle_module[i].enable_big_gate == MY_ENABLE){
			fill_close_big_gate_door_no_delay (&fill_bottle_module[i]);
		}else{
			fill_open_big_gate_door_no_delay (&fill_bottle_module[i]);
		}
		/////////////////////////////////////////////////////////////////////
		fill_bottle_module[i].output_buf_backup = fill_bottle_module[i].output_buf;
		fill_bottle_module[i].output_buf_manual = fill_bottle_module[i].output_buf_backup;
	}
}
//
void map_output_signal ()
{
	uint16_t i = 0;
	//////////////////////////////////////////////////////////////////////////////////////
	if (counter_env.system_signal == SYSTER_RUNNING ||
			GET_BIT(counter_env.system_alarm_high, ALM_EMERGENCY_STOP_CH) == 0){
		counter_module[0].output_buf_low_manual = 0xFFFF;
		counter_module[1].output_buf_low_manual = 0xFFFF;
		ALL_DOOR_MANUAL = 1;
	}
	for (i = 0; i < COUNTER_MODULE_NUM; i++){
		counter_module[i].input_buf_high_map = counter_module[i].input_buf_high;
		counter_module[i].input_buf_low_map = counter_module[i].input_buf_low;
		counter_module[i].output_buf_high_map = counter_module[i].output_buf_high;
		counter_module[i].output_buf_low_map = counter_module[i].output_buf_low & counter_module[i].output_buf_low_manual;
	}
	
	

	if (GET_BIT(counter_env.system_alarm_high, ALM_EMERGENCY_STOP_CH) != 0){
		MAP_DOOR_TO_EXT_OUTPUT (0, 0, 0);
		MAP_DOOR_TO_EXT_OUTPUT (0, 1, 1);
		MAP_DOOR_TO_EXT_OUTPUT (0, 2, 2);
		MAP_DOOR_TO_EXT_OUTPUT (0, 3, 3);
		MAP_DOOR_TO_EXT_OUTPUT (0, 4, 4);
		MAP_DOOR_TO_EXT_OUTPUT (0, 5, 5);
		MAP_DOOR_TO_EXT_OUTPUT (0, 6, 6);
		MAP_DOOR_TO_EXT_OUTPUT (0, 7, 7);
		MAP_DOOR_TO_EXT_OUTPUT (1, 8, 0);
		MAP_DOOR_TO_EXT_OUTPUT (1, 9, 1);
		MAP_DOOR_TO_EXT_OUTPUT (1, 10, 2);
		MAP_DOOR_TO_EXT_OUTPUT (1, 11, 3);
		MAP_DOOR_TO_EXT_OUTPUT (1, 12, 4);
		MAP_DOOR_TO_EXT_OUTPUT (1, 13, 5);
		MAP_DOOR_TO_EXT_OUTPUT (1, 14, 6);
		MAP_DOOR_TO_EXT_OUTPUT (1, 15, 7);
	}
	//////////////////////////////////////////////////////////////////////////////////////
	
	fill_bottle_module[0].output_buf_manual_real = fill_bottle_module[0].output_buf_manual;
	fill_bottle_module[1].output_buf_manual_real = fill_bottle_module[1].output_buf_manual;
	//////////////////////////////////////////////////////////////////////////////////////
	if (counter_env.system_signal == SYSTER_RUNNING ||
			GET_BIT(counter_env.system_alarm_high, ALM_EMERGENCY_STOP_CH) == 0){
		fill_bottle_module[0].output_buf_manual_real = 0xFFFF;
		fill_bottle_module[1].output_buf_manual_real = 0xFFFF;
	}
	if (GET_BIT(counter_env.system_alarm_low, ALM_LIFTER_UP_LIMIT_CH) == 0){
		SET_OUTPUT_OFF (&fill_bottle_module[0].output_buf_manual_real, OUTPUT_BUF_UP);
	}
	if (GET_BIT(counter_env.system_alarm_low, ALM_LIFTER_DOWN_LIMIT_CH) == 0){
		SET_OUTPUT_OFF (&fill_bottle_module[0].output_buf_manual_real, OUTPUT_BUF_DOWN);
	}
	if (VIBRATOR_SWITCH_1 == 1 ||
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_FALL_1_CH) == 0 || 
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_JAM_1_CH) == 0){
		fill_bottle_module[0].speed = 0;
	}
	if (VIBRATOR_SWITCH_2 == 1 ||
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_FALL_2_CH) == 0 || 
			GET_BIT(counter_env.system_alarm_low, ALM_BOTTLE_JAM_2_CH) == 0){
		fill_bottle_module[1].speed = 0;
	}
	fill_bottle_module[0].input_buf_map = fill_bottle_module[0].input_buf;
	fill_bottle_module[0].output_buf_map = fill_bottle_module[0].output_buf;
//	LED1 = GET_BIT (fill_bottle_module[0].output_buf, OUTPUT_BUF_COMPLETE);
//	LED3 = GET_BIT (fill_bottle_module[0].output_buf, OUTPUT_BUF_NOZZLE);
	
	if (GET_BIT(counter_env.system_alarm_high, ALM_EMERGENCY_STOP_CH) != 0){//没有拍急停
		MAP_FILL_SIG_TO_EXT_OUTPUT (0, 17, OUTPUT_BUF_BIG_GATE_DOOR);
		MAP_FILL_SIG_TO_EXT_OUTPUT (0, 18, OUTPUT_BUF_NOZZLE);
		MAP_FILL_SIG_TO_EXT_OUTPUT (1, 20, OUTPUT_BUF_BIG_GATE_DOOR);
		MAP_FILL_SIG_TO_EXT_OUTPUT (1, 21, OUTPUT_BUF_NOZZLE);

		MAP_SIG_TO_EXT_OUTPUT (26, VIB_LOCK_SWITCH);
	}
	
	if (CONVEYOR_START == 0){//夹瓶带控制
		if (GET_BIT(counter_env.system_alarm_low, ALM_FRONT_BOTTLE_JAM_CH) == 0){
			BOTTLE_BELT_OUTPUT = 1;
		}else{
			BOTTLE_BELT_OUTPUT = 0;
		}
	}else{
		BOTTLE_BELT_OUTPUT = 1;
	}
	
	MAP_FILL_SIG_TO_EXT_OUTPUT (0, 16, OUTPUT_BUF_VIB); //振动器
	MAP_FILL_SIG_TO_EXT_OUTPUT (1, 19, OUTPUT_BUF_VIB); //振动器
	
	if (counter_env.is_master){//小料门输出损坏备用代码
		MAP_DOOR_TO_EXT_OUTPUT (0, 22, 5);
	}else{
	#ifdef USE_SCREW_SHIFT_BOTTLE
		MAP_FILL_SIG_TO_EXT_OUTPUT (0, 22, OUTPUT_BUF_BOTTLE_SHIFT); 
	#else
		MAP_FILL_SIG_TO_EXT_OUTPUT (0, 22, OUTPUT_BUF_EXT_FILL_B); 
	#endif
	}
	
	MAP_SIG_TO_EXT_OUTPUT (23, CONVEYOR_START);
	MAP_SIG_TO_EXT_OUTPUT (25, counter_env.screw_running);//螺杆运转信号
	MAP_SIG_TO_EXT_OUTPUT (27, (counter_env.system_signal == SYSTER_RUNNING) ? 0 : 1);//绿灯
	MAP_SIG_TO_EXT_OUTPUT (28, (counter_env.system_signal == SYSTER_RUNNING) ? 1 : 0);//红灯
	MAP_SIG_TO_EXT_OUTPUT (29, counter_env.led_yellow);//黄灯
	MAP_SIG_TO_EXT_OUTPUT (30, counter_env.beep);//蜂鸣器
	MAP_FILL_SIG_TO_EXT_OUTPUT (0, 31, OUTPUT_BUF_COMPLETE);
	//////////////////////////////////////////////////////////////////////
	LED1 = (counter_env.system_signal == SYSTER_RUNNING) ? 1 : 0;
	LED3 = counter_env.led_yellow;
	//counter_env.speed = fill_bottle_module[0].speed + fill_bottle_module[1].speed;
	counter_env.total_good = fill_bottle_module[0].total_good + fill_bottle_module[1].total_good + counter_env.ext_total_good;
	counter_env.total_reject = fill_bottle_module[0].total_reject + fill_bottle_module[1].total_reject;
	////////////////////////////////////////////////////////////////////////////////
}
//









