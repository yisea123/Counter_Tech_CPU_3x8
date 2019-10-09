#ifndef _FILL_BOTTLE_SYSTEM_MICRO_H_
#define _FILL_BOTTLE_SYSTEM_MICRO_H_


#define FILL_BOTTLE_MODULE_NUM 2

#define FILL_BOTTLE_SPEED_BUF_LENGHT 16

#define USE_SCREW_SHIFT_BOTTLE

#define ONE_BOTTLE_SERVO_MOTOR_PULSE_NUM 3156




#define SET_FILL_BOTTLE_COMPLETE_DELAY 0
#define SET_FILL_RESET_FILL_BOTTLE_COMPLETE_DELAY 0
#define SET_FILL_WAIT_PIECE_FALL_TIME (p_fill_bottle_module->set_para.set_fill_bottle_time)
#define SET_FILL_OPEN_BIG_GATE_DOOR_DELAY (p_fill_bottle_module->set_para.set_gate_open_time)
#define SET_FILL_CLOSE_BIG_GATE_DOOR_DELAY (p_fill_bottle_module->set_para.set_gate_close_time)
#define SET_FILL_READY_TO_SHIFT_BOTTLE_DELAY 0
#define SET_FILL_OPEN_SHIFT_BOTTLE_DELAY (p_fill_bottle_module->set_para.set_shift_bottle_time)
#define SET_FILL_CLOSE_SHIFT_BOTTLE_DELAY p_fill_bottle_module->set_para.set_fill_bottle_finished_delay
#define SET_FILL_UP_NOZZLE_DELAY (p_fill_bottle_module->set_para.set_nozzle_up_time)
#define SET_FILL_DOWN_NOZZLE_DELAY (p_fill_bottle_module->set_para.set_nozzle_down_time)
#define SET_FILL_NOZZLE_VIB_DOWN_TIME p_fill_bottle_module->set_para.set_nozzle_vib_down_time
#define SET_FILL_NOZZLE_VIB_UP_TIME p_fill_bottle_module->set_para.set_nozzle_vib_up_time
#define SET_FILL_ENABLE_NOZZLE_VIB p_fill_bottle_module->set_para.set_enable_nozzle_vib
#define SET_FILL_REJECT_START_DELAY  (p_fill_bottle_module->set_para.set_reject_delay)
#define SET_FILL_REJECT_KEEP_DELAY  (p_fill_bottle_module->set_para.set_reject_keep)

#define SET_FILL_SERVO_MOTOR_PULSE_NUM (counter_env.set_servo_motor_shift_bottle_num * ONE_BOTTLE_SERVO_MOTOR_PULSE_NUM)
#define SET_FILL_SERVO_MOTOR_SHIFT_BOTTLE_DELAY fill_bottle_module[0].set_para.set_shift_bottle_time

#define SET_FILL_CONTINUE_REJECT_STOP_NUM (counter_env.set_continue_reject_stop_num)

#define SET_FILL_PRODUCT_NUM (counter_env.set_product_num)

#define SET_PIECE_FALL_TIME 450//备料时间















#endif
