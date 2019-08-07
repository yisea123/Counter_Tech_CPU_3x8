#include "main.h"






//
void update_piece_info(s_counter_module * p_counter_module, s_chanel_info * p_chanel) {
	if (p_counter_module->min_piece_interval > p_chanel->piece_interval){
		p_counter_module->min_piece_interval = p_chanel->piece_interval;
	}
	if (p_counter_module->max_length < p_chanel->length){
		p_counter_module->max_length = p_chanel->length;
	}
	if (p_counter_module->min_length > p_chanel->length){
		p_counter_module->min_length = p_chanel->length;
	}
}
//
void check_normal_count_length(s_counter_module * p_counter_module, s_chanel_info * p_chanel) {
	if (p_chanel->length > p_counter_module->set_para.set_max_length){ /*超过设定长度*/
		p_counter_module->normal_count_rej_flag |= REJ_TOO_LONG;
	}
}
//
void check_pre_count_length(s_counter_module * p_counter_module, s_chanel_info * p_chanel) {
	if (p_chanel->length > p_counter_module->set_para.set_max_length){ /*超过设定长度*/
		p_counter_module->pre_count_rej_flag |= REJ_TOO_LONG;
	}
}
//
void check_pre_count_close_door_interval(s_counter_module * p_counter_module, s_chanel_info * p_chanel) {
	p_chanel->close_door_interval = p_chanel->piece_interval;
	if (p_counter_module->min_close_door_interval > p_chanel->close_door_interval){
		p_counter_module->min_close_door_interval = p_chanel->close_door_interval;
	}
	/*判断是否小料门关闭时药粒间隔太小*/
	if (p_chanel->close_door_interval < p_counter_module->set_para.set_door_close_interval){
		p_counter_module->normal_count_rej_flag |= REJ_TOO_CLOSE;
		p_counter_module->pre_count_rej_flag |= REJ_TOO_CLOSE;
	}
}
//
void check_pre_count_door_switch_interval (s_counter_module * p_counter_module, s_chanel_info * p_chanel) {
	if (p_counter_module->min_door_switch_interval > p_chanel->door_switch_interval){
		p_counter_module->min_door_switch_interval = p_chanel->door_switch_interval;
	}
	/*判断是否小料门开关间隔太小*/
	if ((p_chanel->door_switch_interval + p_chanel->ch_door_close_delay) < (p_counter_module->set_para.set_door_switch_interval)){
		/*开关频率太快，需额外加一段延时等待料门完全打开*/
		p_chanel->ch_door_close_delay = (p_counter_module->set_para.set_door_switch_interval) - (p_chanel->door_switch_interval + p_chanel->ch_door_close_delay);
		p_counter_module->normal_count_rej_flag |= REJ_DOOR_SWITCH_TOO_FAST;
		p_counter_module->pre_count_rej_flag |= REJ_DOOR_SWITCH_TOO_FAST;
	}
}
//
void set_all_chanel_status (s_counter_module * p_counter_module, uint16_t status) 
	{
	uint16_t i;
	for (i = 0; i < CHANEL_NUM; i++){
		p_counter_module->chanel[i].counter_state = status;
	}
}

//
void set_16io_bit (uint16_t *p_value, uint16_t mask)
{
	*p_value |= mask;
}
//
void reset_16io_bit (uint16_t *p_value, uint16_t mask)
{
	*p_value &= ~mask;
}
////////////////////////////////////////////////////////////////////////////////
void counter_module_pieces_process(s_counter_module * p_counter_module, uint16 ch)
{
	s_chanel_info *p_chanel;
	p_chanel = &p_counter_module->chanel[ch];
	p_chanel->input_status_new = ((p_counter_module->input_buf_low) & (0x1 << ch)) != 0? 1 : 0;
	//1 表示有效， 0表示无效
	///////////////////////////////////////////////////////////
	if ((p_chanel->input_status_new == DETECT_OFF) && (p_chanel->input_status_old == DETECT_OFF)){      //电眼没有药粒状态
		if (p_chanel->piece_interval < MAX_DISPLAY){
			p_chanel->piece_interval++;
		}else{
		}
	}else if ((p_chanel->input_status_new == DETECT_ON) && (p_chanel->input_status_old == DETECT_OFF)){//药粒刚进电眼状态
		p_chanel->length = 0;
	}else if ((p_chanel->input_status_new == DETECT_ON) && (p_chanel->input_status_old == DETECT_ON)){//药粒进入电眼状态
		if (p_chanel->length < MAX_DISPLAY){
			p_chanel->length++;
		}else{
			counter_env.detect_error = 1;
		}
	}else if ((p_chanel->input_status_new == DETECT_OFF) && (p_chanel->input_status_old == DETECT_ON)){//药粒刚出电眼状态
		////////////////////////////////////////////////////////////////////////////////////
		if (p_chanel->length >= p_counter_module->set_para.set_min_length){//长度大于设定值最小长度才能计数
			/*药粒出了检测区，更新药粒相关信息****************************************************************/
			update_piece_info (p_counter_module, p_chanel);
			///////////////////////////////////////////////////////////////////////////////////////////
			//计数/////////////////////////////////////////////////////////////////////////////////////
			p_chanel->cur_count++;
			///////////////////////////////////////////////////////////////////////////////////////////
			switch (p_chanel->counter_state)
			{
				case NORMAL_COUNT://通道正常数粒状态
					if (p_counter_module->normal_count >= SET_COUNT){//错误状态
						p_counter_module->system_status = COUNTER_ERROR;
					}
					check_normal_count_length (p_counter_module, p_chanel);//检查药粒长度
					p_counter_module->normal_count++;
					if (p_counter_module->normal_count == SET_COUNT){//当前这一瓶的最后一粒
						set_all_chanel_status(p_counter_module, SEPARATE_PRE_COUNT);//通知其他通道下一颗要进行分药动作
						p_counter_module->last_piece_chanel_id = ch;//记录发出通知的通道ID
					}
					break;
				case SEPARATE_PRE_COUNT://每个通道的下一瓶的第一颗药，需要小料门分药动作
					if (p_counter_module->set_para.set_door_close_delay > 0){
						p_chanel->ch_door_close_delay = p_counter_module->set_para.set_door_close_delay;/*小料门关闭延时*/
					}else{
						p_chanel->ch_door_close_delay = p_counter_module->set_para.set_door_close_delay + 1;/*小料门关闭延时*/
					}
					check_pre_count_length (p_counter_module, p_chanel);//检查药粒长度
					check_pre_count_close_door_interval (p_counter_module, p_chanel);//检查小料门关门间隔
					check_pre_count_door_switch_interval (p_counter_module, p_chanel);//检查小料门开关间隔
					p_chanel->cur_count = 1;//每个通道的下一瓶的第一颗药
					p_counter_module->pre_count++;
					p_chanel->counter_state = PRE_COUNT;
					break;
				case PRE_COUNT://通道预数粒状态
					check_pre_count_length (p_counter_module, p_chanel);
					p_counter_module->pre_count++;
					break;
				default://错误状态
					p_counter_module->system_status = STATUS_ERROR;
					break;
				}
		}else{//长度小于设定值最小长度不计数
		}
		////////////////////////////////////////////////////////////////////////////////
		p_chanel->piece_interval = 0;
		////////////////////////////////////////////////////////////////////////////////
		if (p_counter_module->pre_count > SET_COUNT){//预数超过设定数
			p_counter_module->pre_count_rej_flag |= REJ_TOO_MORE;//更新剔除原因
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	p_chanel->input_status_old = p_chanel->input_status_new;
	//////////////////////////////////////////////////////////////////////////////////
}
//
void set_reject_flag (s_counter_module * p_counter_module)
{
	p_counter_module->reject_flag = p_counter_module->normal_count_rej_flag;
	if (p_counter_module->reject_flag != 0){
		p_counter_module->last_reject_flag = p_counter_module->reject_flag;//保存最后一次提出原因
		SET_OUTPUT_ON(&p_counter_module->output_buf_high, OUTPUT_BUF_REJECT);
	}
}
//
void start_vibrator (s_counter_module * p_counter_module)
{
	SET_OUTPUT_ON(&p_counter_module->output_buf_high, OUTPUT_BUF_VIB);
}
//
void stop_vibrator (s_counter_module * p_counter_module)
{
	SET_OUTPUT_OFF(&p_counter_module->output_buf_high, OUTPUT_BUF_VIB);
}
//
void close_specify_door (s_counter_module * p_counter_module, uint16 door_id)
{
	if (door_id < CHANEL_NUM){
		p_counter_module->chanel[door_id].door_is_close = 1;
		SET_OUTPUT_ON (&p_counter_module->output_buf_low, (0x01 << door_id));
	}
}
void set_count_complete (s_counter_module * p_counter_module)
{
	SET_OUTPUT_ON(&p_counter_module->output_buf_high, OUTPUT_BUF_COMPLETE);
	p_counter_module->counter_module_state = FILL_BOTTLE;
}
void reset_count_complete (s_counter_module * p_counter_module)
{
	p_counter_module->counter_module_state = COUNTER_IDLE;
	SET_OUTPUT_OFF(&p_counter_module->output_buf_high, OUTPUT_BUF_COMPLETE);
	SET_OUTPUT_OFF(&p_counter_module->output_buf_high, OUTPUT_BUF_REJECT);
}
void open_all_door (s_counter_module * p_counter_module)
{
	uint16_t i;
	for (i = 0; i < CHANEL_NUM; i++){
		p_counter_module->chanel[i].ch_door_close_delay = 0;
		if (p_counter_module->chanel[i].door_is_close){
			p_counter_module->chanel[i].door_is_close = 0;
			p_counter_module->chanel[i].door_switch_interval = 0;
		}
	}
	SET_OUTPUT_OFF (&p_counter_module->output_buf_low, ALL_DOOR_MASK);
}
//
void switch_to_normal_count (s_counter_module * p_counter_module)
{
	if (p_counter_module->pre_count < SET_COUNT){//如果预数粒还没数够
		set_all_chanel_status (p_counter_module, NORMAL_COUNT);
	}else{
		set_all_chanel_status (p_counter_module, SEPARATE_PRE_COUNT);
	}
	p_counter_module->normal_count = p_counter_module->pre_count;//
	p_counter_module->pre_count = 0;
	p_counter_module->normal_count_rej_flag = p_counter_module->pre_count_rej_flag;//
	p_counter_module->pre_count_rej_flag = 0;
	p_counter_module->counter_time_ctr = 0;
	reset_count_complete (p_counter_module);
	start_vibrator(p_counter_module);//启动振动器
	open_all_door (p_counter_module);//打开所有小料门
}
//
void check_door_delay (s_counter_module * p_counter_module)
{
	uint16_t i;
	for (i = 0; i < CHANEL_NUM; i++){
		if (p_counter_module->chanel[i].ch_door_close_delay > 0){
			p_counter_module->chanel[i].ch_door_close_delay--;
			if (p_counter_module->chanel[i].ch_door_close_delay == 0){
				close_specify_door (p_counter_module, i);
			}
		}
	}
}
//
void common_op (s_counter_module * p_counter_module)
{
	uint16_t i;
	if (counter_env.system_signal == SYSTER_RUNNING){
		p_counter_module->counter_time_ctr++;
		//start_vibrator(p_counter_module);
	}else{//停机状态
		stop_vibrator(p_counter_module);
	}
	if ((p_counter_module->pre_count >= SET_PRE_COUNT) || (p_counter_module->pre_count >= SET_COUNT)){//达到设定的预数
		if (p_counter_module->pre_count > 0){
			stop_vibrator(p_counter_module);
		}
	}
	for (i = 0; i < CHANEL_NUM; i++){
		if (p_counter_module->chanel[i].door_switch_interval < MAX_DISPLAY){
			if (p_counter_module->chanel[i].door_is_close == 0){
				p_counter_module->chanel[i].door_switch_interval++;
			}
		}
	}
	set_reject_flag (p_counter_module);
}
//
void poll_fill_bottle_status (s_counter_module * p_counter_module)
{
	if (counter_env.system_signal == SYSTER_RESET){
		p_counter_module->poll_status = 0;
	}
	switch (p_counter_module->poll_status)
	{
		case 0:
			if (p_counter_module->normal_count >= SET_COUNT){//等待数粒完成
				if (p_counter_module->start_rej < p_counter_module->set_para.set_start_rej){
					p_counter_module->start_rej++;
					p_counter_module->normal_count_rej_flag |= REJ_START;
				}
				p_counter_module->counter_time = p_counter_module->counter_time_ctr;
				set_count_complete (p_counter_module);
				p_counter_module->poll_status++;
			}
			break;
		case 1:
			if (GET_BIT(p_counter_module->input_buf_high, OUTPUT_BUF_COMPLETE) == 0){//等待信号拉低
				p_counter_module->poll_status++;
				reset_count_complete (p_counter_module);
			}
			break;
		case 2:
			if (GET_BIT(p_counter_module->input_buf_high, OUTPUT_BUF_COMPLETE) == 1){//等待信号拉高
				switch_to_normal_count(p_counter_module);
				p_counter_module->poll_status = 0;
			}
			break;
		default:break;
	}
}
void counter_module_poll_chanel (s_counter_module * p_counter_module, const uint16_t input_high, const uint16_t input_low)
{
	uint16_t i;
	////////////////////////////////////////////////////////////////////////////////
	p_counter_module->input_buf_high = input_high;		
	p_counter_module->input_buf_low = input_low;		
	////////////////////////////////////////////////////////////////////////////////
	common_op (p_counter_module);
	poll_fill_bottle_status (p_counter_module);
	for (i = 0; i <CHANEL_NUM; i++){
		counter_module_pieces_process (p_counter_module, i);
	}	
	check_door_delay(p_counter_module);//检查小料门延时，看是否需要关闭			
	/////////////////////////////////////////////////////////////////////////////	
}
//


















