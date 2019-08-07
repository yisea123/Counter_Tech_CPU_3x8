#ifndef __COUNTER_TECH_CPU_MAIN_
#define __COUNTER_TECH_CPU_MAIN_




extern s_fill_bottle_module fill_bottle_module[2];
extern s_counter_module counter_module[2];
extern s_counter_env counter_env;


extern uint16_t counter_input_signal_1;
extern uint16_t counter_input_signal_2;





void counter_output_map_func (void);
void counter_reset (void);
void counter_pre_start (void);
void counter_stop (void);
void counter_data_clear(void);
void counter_product_data_clear (void);
void screw_motor_pwm_process (void);




void counter_init (void);
void counter_task_poll (void);



#endif










