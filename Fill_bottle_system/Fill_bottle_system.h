#ifndef __FILL_BOTTLE_SYSTEM_
#define __FILL_BOTTLE_SYSTEM_

#include "Fill_bottle_system_micro.h"


#define MAP_FILL_SIG_TO_EXT_OUTPUT(F_CH, EX_CH, SIGNAL){ \
	if (counter_env.system_signal == SYSTER_STOP){ \
		if (fill_bottle_module[F_CH].fill_bottle_idle_time > 2000){ \
			EXT_OUT##EX_CH = GET_BIT (fill_bottle_module[F_CH].output_buf_manual_real, SIGNAL); \
		}else{ \
			EXT_OUT##EX_CH = GET_BIT (fill_bottle_module[F_CH].output_buf, SIGNAL); \
		} \
	}else{ \
		EXT_OUT##EX_CH = GET_BIT (fill_bottle_module[F_CH].output_buf, SIGNAL); \
	} \
}
#define MAP_SIG_TO_EXT_OUTPUT(EX_CH, SIGNAL){ \
	EXT_OUT##EX_CH = SIGNAL; \
}


typedef enum{
	PRE_BIG_GATE_INIT= 0x00,
	PRE_BIG_GATE_OPEN_ING,
	PRE_BIG_GATE_OPEN_ED,
	PRE_BIG_GATE_OPEN_ED1,
	PRE_BIG_GATE_CLOSE_ING,
	PRE_BIG_GATE_CLOSE_ED,
}e_pre_big_gate_op;


typedef struct{
	uint16_t set_fill_bottle_time;
	uint16_t set_shift_bottle_time;
	
	uint16_t set_reject_delay;
	uint16_t set_reject_keep;
	
	uint16_t set_enable_big_gate;
	uint16_t set_enable_nozzle_vib;
	uint16_t set_nozzle_vib_up_time;
	uint16_t set_nozzle_vib_down_time;
	uint16_t set_gate_open_time;
	uint16_t set_gate_close_time;
	uint16_t set_nozzle_up_time;
	uint16_t set_nozzle_down_time;
	uint16_t set_fill_bottle_finished_delay;
}s_fill_bottle_set_para;
	
typedef struct{
	uint16_t total_count_sum;
	uint16_t speed;
	uint16_t total_reject;
	uint16_t total_good;
	uint16_t total_good_tmp;
	uint16_t continue_reject_ctr;
	
	
	
	uint16_t enable_big_gate;
	
	uint16_t reject_q[REJECT_Q_16];
	uint16_t reject_q_index;
	uint16_t reject_keep_delay;
	uint16_t big_gate_delay;
	uint16_t nozzle_up_delay;
	uint16_t nozzle_down_delay;
	uint16_t nozzle_delay;
	uint16_t shift_bottle_delay;
	uint16_t fill_complete_delay;
	e_fill_bottle_status fill_bottle_state;//装瓶状态机
	
	uint16_t fill_bottle_speed_time;
	uint16_t fill_bottle_speed_ctr;
	uint16_t fill_bottle_speed_ctr_avg;
	uint16_t fill_bottle_speed_ctr_buf[FILL_BOTTLE_SPEED_BUF_LENGHT];
	uint16_t fill_bottle_speed_ctr_index;
	uint16_t fill_bottle_speed_ctr_length;
	
	
	uint16_t fill_bottle_flag;
	
	uint16_t pre_fill_complete_time;//提前给装瓶完成信号延时
	uint16_t pre_fill_complete_ctr;//
	e_pre_big_gate_op pre_open_big_gate_flag;
	uint16_t pre_open_big_gate_ctr;
	e_pre_big_gate_op pre_close_big_gate_flag;
	uint16_t pre_close_big_gate_ctr;
	uint16_t pre_count_complete_ctr;
	
	uint16_t nozzle_vib_status;
	uint16_t nozzle_vib_up_time;
	uint16_t nozzle_vib_down_time;
	
	uint16_t input_buf;
	uint16_t output_buf;
	uint16_t output_buf_backup;
	uint16_t output_buf_manual;
	uint16_t output_buf_manual_real;
	uint16_t input_buf_map;
	uint16_t output_buf_map;
	uint16_t fill_bottle_idle_time;//装瓶系统空闲时间
	
	uint16_t complete_signal_poll_status;
	uint16_t complete_signal_t;
	
	s_fill_bottle_set_para set_para;
	uint16_t first_run;
	uint32_t fill_bottle_op_delay;
}s_fill_bottle_module;









void fill_bottle_system_module_poll (s_fill_bottle_module * p_fill_bottle_module, const uint16_t input_map);
void map_output_signal (void);
void fill_bottle_module_reset (void);
void fill_up_nozzle (s_fill_bottle_module * p_fill_bottle_module);
void fill_down_nozzle (s_fill_bottle_module * p_fill_bottle_module);

void fill_reset_fill_bottle_complete (s_fill_bottle_module * p_fill_bottle_module);
void fill_close_shift_bottle (s_fill_bottle_module * p_fill_bottle_module);
void fill_stop_reject (s_fill_bottle_module * p_fill_bottle_module);
void fill_open_big_gate_door_no_delay (s_fill_bottle_module * p_fill_bottle_module);
void fill_close_big_gate_door_no_delay (s_fill_bottle_module * p_fill_bottle_module);





#endif


