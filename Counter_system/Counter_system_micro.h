#ifndef _COUNTER_SYSTEM_MICRO_H_
#define _COUNTER_SYSTEM_MICRO_H_

//Rej flag
#define REJ_TOO_MORE 								0x0001 //多数
#define REJ_TOO_LONG 								0x0002 //超长
#define REJ_START 									0x0004 //开机剔除
#define REJ_TOO_BIG 								0x0008 //面积太大
#define REJ_TOO_SMALL 							0x0010 //面积太小
#define REJ_TOO_CLOSE 							0x0020 //关门间隔太小
#define REJ_DOOR_SWITCH_TOO_FAST 		0x0040 //开关间隔太小

#define OUTPUT_BUF_VIB 						0x0001
#define OUTPUT_BUF_BIG_GATE_DOOR 	0x0002
#define OUTPUT_BUF_NOZZLE					0x0004
#define OUTPUT_BUF_BOTTLE_SHIFT 	0x0008
#define OUTPUT_BUF_REJECT					0x0010
#define OUTPUT_BUF_UP							0x0020
#define OUTPUT_BUF_DOWN						0x0040
#define OUTPUT_BUF_EXT_FILL_B			0x0080
#define OUTPUT_BUF_SCREW_RD			  0x0100
#define OUTPUT_BUF_COMPLETE				0x8000

#define DOOR0  0x0001
#define DOOR1  0x0002
#define DOOR2  0x0004
#define DOOR3  0x0008
#define DOOR4  0x0010
#define DOOR5  0x0020
#define DOOR6  0x0040
#define DOOR7  0x0080
#define DOOR8  0x0100
#define DOOR9  0x0200
#define DOOR10 0x0400
#define DOOR11 0x0800
#define DOOR12 0x1000
#define DOOR13 0x2000
#define DOOR14 0x4000
#define DOOR15 0x8000


#define ALM_BOTTLE_FALL_1_CH  			0x0001
#define ALM_BOTTLE_JAM_1_CH  				0x0002
#define ALM_VIB_1_OFF_CH  					0x0004
#define ALM_LIFTER_UP_LIMIT_CH			0x0008
#define ALM_LIFTER_DOWN_LIMIT_CH		0x0010
#define ALM_BOTTLE_FALL_2_CH  			0x0020
#define ALM_BOTTLE_JAM_2_CH  				0x0040
#define ALM_VIB_2_OFF_CH  					0x0080
#define RES8  0x0100
#define RES9  0x0200
#define RES10 0x0400
#define RES11 0x0800
#define RES12 0x1000
#define RES13 0x2000
#define RES14 0x4000 
#define ALM_FRONT_BOTTLE_JAM_CH 		0x8000
 
#define ALM_EMERGENCY_STOP_CH  		0x0001
#define ALM_POWER_OFF_CH  				0x0002
#define ALM_PRESSURE_LOW_CH  			0x0004
#define ALM_CONVEYOR_STOP_CH  		0x0008
#define ALM_VIB_UNLOCK_CH					0x0010
#define ALM_DETECT_ERROR_CH				0x0020
#define ALM_PRODUCT_NUM_REACH_CH	0x0040
#define ALM_CONTINUE_REJ_STOP_CH	0x0080

#define ALM_NORMAL_STATUS					0xFFFF

#define MY_RAND_MAX 10
#define MY_ENABLE 	0
#define MY_DISABLE 1
#define FIRST_RUN 0

#define DETECT_ON 0
#define DETECT_OFF 1

#define SHIFT_A_BOTTLE 4
#define SYSTER_PRE_STOP 3
#define SYSTER_RESET 2
#define SYSTER_RUNNING 1
#define SYSTER_STOP 0

#define COUNTER_MODULE_NUM 2
#define CHANEL_NUM 16
#define REJECT_Q_16 16

#define READY_SIGNAL_CTR 5



#define SCREW_IS_RUN 0x02
#define VIBRATOR_SWITCH_2 virtual_input[6]
#define SYSTEM_START_SWITCH  virtual_input[7]
#define VIB_LOCK_SWITCH virtual_input[8]
#define VIBRATOR_SWITCH_1 virtual_input[9]
#define CONVEYOR_START virtual_input[10]
#define ALL_DOOR_MANUAL virtual_input[11]

#define SCREW_MODULE_1_EN virtual_input[44- VIRTUAL_INPUT_OFFSET]
#define SCREW_MODULE_2_EN virtual_input[45- VIRTUAL_INPUT_OFFSET]
#define SCREW_MODULE_3_EN virtual_input[46- VIRTUAL_INPUT_OFFSET]

#define SIM_SCREW_READY_SIGNAL virtual_input[43- VIRTUAL_INPUT_OFFSET]

#define DETECT_ERROR counter_env.detect_error
#define PRODUCT_NUM_REACH counter_env.procuct_num_reach
#define CONTINUE_REJ_STOP counter_env.continue_reject_flag

#define SET_OUTPUT_ON(P_VALUE,MASK) reset_16io_bit(P_VALUE, MASK)
#define SET_OUTPUT_OFF(P_VALUE,MASK) set_16io_bit(P_VALUE, MASK)
#define OUTPUT_ALL_ON  (0x00000000)
#define OUTPUT_ALL_OFF (0xFFFF)
#define ALL_DOOR_MASK  (0xFFFF)

#define SET_COUNT counter_env.set_count
#define SET_PRE_COUNT counter_env.set_pre_count
#define SET_START_REJECT p_counter_module->set_para.set_start_rej
#define SET_MAX_LENGTH p_counter_module->set_para.set_max_length
#define SET_MIN_LENGTH p_counter_module->set_para.set_min_length
#define SET_DOOR_CLOSE_DELAY p_counter_module->set_para.set_door_close_delay
#define SET_DOOR_CLOSE_INTERVAL p_counter_module->set_para.set_door_close_interval
#define SET_DOOR_SWITCH_INTERVAL p_counter_module->set_para.set_door_switch_interval

#define SET_SIM_LENGTH_X p_counter_module->set_para.set_sim_length_X
#define SET_SIM_LENGTH_Y p_counter_module->set_para.set_sim_length_Y
#define SET_SIM_INTERVAL_X p_counter_module->set_para.set_sim_interval_X
#define SET_SIM_INTERVAL_Y p_counter_module->set_para.set_sim_interval_Y


#define SET_SIM_SCAN_CTR p_counter_module->set_para.sim_scan_ctr

#define MAX_DISPLAY (1000 - 1)
#define COUNTER_SYSTEM_ONE_SECOND_CTR (((1000/TIM2_INT_CYCLE) * 1000)-1)

#define GET_BIT(V,N) (((V & N) == 0) ? 0 : 1)

#endif


