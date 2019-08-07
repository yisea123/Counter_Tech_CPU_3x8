#include "main.h"


/*
* ������ :CRC16
* ���� : ����CRC16
* ���� : ptr---����,len---����
* ��� : У��ֵ
*/ 
uint16_t CRC16(uint8_t *ptr, uint16_t len) 
{
	uint8_t i;
	uint16_t crc = 0xFFFF; 
	if (len == 0) {
		len = 1;
	} 
	while (len--) {
		crc ^= *ptr;
		for (i = 0; i<8; i++) {
			if (crc & 1) {
				crc >>= 1; crc ^= 0xA001;
			}else{
				crc >>= 1;
			}
		} ptr++;
	} 
	return((crc >> 8) |(crc << 8)); 
}



///////////////////////////////////////////////////////////

u8 RS485_tx_buf[256];//���ͻ�����
static uint16_t modbus_ch = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Modbus�Ĵ����͵�Ƭ���Ĵ�����ӳ���ϵ
static vu16 *Modbus_InputIO[MODBUS_INPUT_NUM];//���뿪�����Ĵ���ָ��(����ʹ�õ���λ������)
vu16 *Modbus_OutputIO[MODBUS_OUTPUT_NUM];//����������Ĵ���ָ��(����ʹ�õ���λ������)
u16 *Modbus_HoldReg[MODBUS_REG_NUM];//���ּĴ���

vu16 virtual_input[MODBUS_VIRTUAL_INPUT_NUM];

#define MAP_MODBUS_INPUT(D,PORT,S) Modbus_InputIO[D]=(vu32*)&P##PORT##in(S)
#define MAP_MODBUS_OUTPUT(D,PORT,S) Modbus_OutputIO[D]=(vu32*)&P##PORT##out(S)
#define MAP_MODBUS_HOLDREG(D,V) { \
	if (D > 0){ \
		Modbus_HoldReg[D - 1] = &V; \
	} \
}

uint16_t Modbus_HoldReg_CPU_Usage = 0;
uint16_t Modbus_HoldReg_NULL = 999;
uint16_t Modbus_test_value = 0;
uint16_t Modbus_com_error = 0;
uint16_t data_change_flag = 0;

void inc_Modbus_test_value (uint16_t value)
{
	if (value > 0){
		Modbus_test_value += value;
	}else{
		Modbus_test_value++;
	}
}
void set_Modbus_test_value (uint16_t value)
{
	Modbus_test_value = value;
}

void Modbus_RegMap(void)
{
	int i;
	//���뿪�����Ĵ���ָ��ָ��
//	MAP_MODBUS_INPUT (0, C, 6);
//	MAP_MODBUS_INPUT (1, C, 7);
//	MAP_MODBUS_INPUT (2, A, 8);
//	MAP_MODBUS_INPUT (3, G, 11);
//	MAP_MODBUS_INPUT (4, G, 0);
//	MAP_MODBUS_INPUT (5, G, 1);
//	MAP_MODBUS_INPUT (6, G, 2);
//	MAP_MODBUS_INPUT (7, G, 3);
//	Modbus_InputIO[0]=(vu32*)&PCin(6);//KEY0

	//����������Ĵ���ָ��ָ��
//	MAP_MODBUS_OUTPUT (0, F, 0);
//	MAP_MODBUS_OUTPUT (1, F, 1);
//	MAP_MODBUS_OUTPUT (2, F, 2);
//	MAP_MODBUS_OUTPUT (3, F, 3);
//	MAP_MODBUS_OUTPUT (4, F, 4);
//	MAP_MODBUS_OUTPUT (5, F, 5);
//	MAP_MODBUS_OUTPUT (6, F, 6);
//	MAP_MODBUS_OUTPUT (7, F, 7);
//	MAP_MODBUS_OUTPUT (8, F, 8);
//	MAP_MODBUS_OUTPUT (9, F, 9);
//	MAP_MODBUS_OUTPUT (10, F, 10);
//	MAP_MODBUS_OUTPUT (11, F, 11);
//	MAP_MODBUS_OUTPUT (12, F, 12);
//	MAP_MODBUS_OUTPUT (13, F, 13);
//	MAP_MODBUS_OUTPUT (14, F, 14);
//	MAP_MODBUS_OUTPUT (15, F, 15);
//	MAP_MODBUS_OUTPUT (16, G, 3);
//	MAP_MODBUS_OUTPUT (17, G, 4);
//	MAP_MODBUS_OUTPUT (18, G, 5);
//	MAP_MODBUS_OUTPUT (19, G, 6);
//	Modbus_OutputIO[0]=(vu32*)&PFout(0);//LED0
	for (i = 0; i < MODBUS_VIRTUAL_INPUT_NUM; i++){
		virtual_input[i] = 1;
		Modbus_OutputIO[21 - 1 + i] = &virtual_input[i];//virtual_input begin from addr 21
	}

	//��ʼ�����ּĴ���ָ��ָ��///////////////////////////////////
	for (i = 0; i < MODBUS_REG_NUM; i++){
		Modbus_HoldReg[i] = &Modbus_HoldReg_NULL;
	}

	MAP_MODBUS_HOLDREG(1, counter_module[0].normal_count);
	MAP_MODBUS_HOLDREG(2, counter_module[0].pre_count);
	MAP_MODBUS_HOLDREG(3, counter_env.total_good);
	MAP_MODBUS_HOLDREG(4, counter_env.total_reject);
	MAP_MODBUS_HOLDREG(5, counter_env.speed);
	MAP_MODBUS_HOLDREG(6, counter_module[1].normal_count);
	MAP_MODBUS_HOLDREG(7, counter_module[1].pre_count);
	MAP_MODBUS_HOLDREG(8, counter_env.system_alarm_low_enable);
	MAP_MODBUS_HOLDREG(9, counter_env.system_alarm_high_enable);
	MAP_MODBUS_HOLDREG(10, counter_env.system_alarm_low);
	MAP_MODBUS_HOLDREG(11, counter_env.system_alarm_high);
	MAP_MODBUS_HOLDREG(12, counter_env.sys_config_info);
	MAP_MODBUS_HOLDREG(13, sys_run_time.data.h);
	MAP_MODBUS_HOLDREG(14, sys_run_time.data.l);
	MAP_MODBUS_HOLDREG(15, counter_env.scan_period);
	MAP_MODBUS_HOLDREG(16, counter_module[0].output_buf_low_manual);
	MAP_MODBUS_HOLDREG(17, fill_bottle_module[0].output_buf_manual);
	MAP_MODBUS_HOLDREG(18, counter_module[1].output_buf_low_manual);
	MAP_MODBUS_HOLDREG(19, fill_bottle_module[1].output_buf_manual);
	MAP_MODBUS_HOLDREG(28, counter_module[0].last_reject_flag);

	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 1, counter_env.set_count);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 2, counter_env.set_pre_count);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 3, counter_module[0].set_para.set_door_close_delay);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 4, counter_module[0].set_para.set_max_length);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 5, counter_module[0].set_para.set_min_length);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 6, counter_module[0].set_para.set_door_close_interval);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 7, counter_module[0].set_para.set_door_switch_interval);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 8, fill_bottle_module[0].set_para.set_reject_delay);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 9, fill_bottle_module[0].set_para.set_reject_keep);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 10, fill_bottle_module[0].set_para.set_enable_big_gate);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 11, fill_bottle_module[0].set_para.set_enable_nozzle_vib);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 12, counter_module[0].set_para.sim_scan_ctr);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 13, fill_bottle_module[0].set_para.set_fill_bottle_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 14, fill_bottle_module[0].set_para.set_shift_bottle_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 15, fill_bottle_module[0].set_para.set_nozzle_vib_up_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 16, fill_bottle_module[0].set_para.set_nozzle_vib_down_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 17, fill_bottle_module[0].set_para.set_gate_open_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 18, fill_bottle_module[0].set_para.set_gate_close_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 19, counter_module[0].set_para.set_sim_length_X);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 20, counter_module[0].set_para.set_sim_length_Y);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 21, counter_module[0].set_para.set_sim_interval_X);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 22, counter_module[0].set_para.set_sim_interval_Y);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 23, counter_module[0].set_para.set_start_rej);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 24, counter_env.counter_da_module[0]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 25, counter_env.counter_da_module[1]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 26, counter_env.counter_da_module[2]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 27, counter_env.counter_da_module[3]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 28, counter_env.counter_da_module[4]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 29, counter_env.counter_da_module[5]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 30, counter_env.counter_da_module[6]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 31, counter_env.counter_da_module[7]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 32, counter_env.counter_da_module[8]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 33, counter_env.counter_da_module[9]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 34, counter_env.counter_da_module_test[0]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 35, counter_env.counter_da_module_test[1]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 36, counter_env.counter_da_module_test[2]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 37, counter_env.counter_da_module_test[3]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 38, counter_env.counter_da_module_test[4]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 39, counter_env.counter_da_module_test[5]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 40, counter_env.counter_da_module_apply[0]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 41, counter_env.counter_da_module_apply[1]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 42, counter_env.counter_da_module_apply[2]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 43, counter_env.counter_da_module_apply[3]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 44, counter_env.counter_da_module_apply[4]);
//	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 45, counter_env.counter_da_module_apply[5]);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 46, counter_env.set_product_num);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 47, counter_env.set_continue_reject_stop_num);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 48, counter_env.set_pre_open_door_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 49, fill_bottle_module[0].set_para.set_nozzle_up_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 50, fill_bottle_module[0].set_para.set_nozzle_down_time);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 51, fill_bottle_module[0].set_para.set_fill_bottle_finished_delay);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 52, counter_env.set_servo_motor_pulse_num);
	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 53, counter_env.set_servo_motor_speed);
	
//END AT	MAP_MODBUS_HOLDREG(MODBUS_SAVE_DATA_START + 70, counter_module[0].set_para.set_start_rej);

	for (i = 0; i < 16; i++){
		MAP_MODBUS_HOLDREG(101 + i, counter_module[0].chanel[i].ch_door_close_delay);
	}
	for (i = 0; i < REJECT_Q_16; i++){
		MAP_MODBUS_HOLDREG(117 + i, fill_bottle_module[0].reject_q[i]);
	}
	MAP_MODBUS_HOLDREG(133, fill_bottle_module[0].reject_keep_delay);
	MAP_MODBUS_HOLDREG(134, fill_bottle_module[0].big_gate_delay);
	MAP_MODBUS_HOLDREG(135, fill_bottle_module[0].nozzle_delay);
	MAP_MODBUS_HOLDREG(136, fill_bottle_module[0].shift_bottle_delay);
	MAP_MODBUS_HOLDREG(137, counter_module[0].input_buf_high_map);
	MAP_MODBUS_HOLDREG(138, counter_module[0].input_buf_low);//MAP_MODBUS_HOLDREG(138, rand_tmp);
	MAP_MODBUS_HOLDREG(139, fill_bottle_module[0].output_buf_map);
	MAP_MODBUS_HOLDREG(140, counter_module[0].output_buf_low_map);
	for (i = 0; i < 16; i++){
		MAP_MODBUS_HOLDREG(141 + i, counter_module[0].chanel[i].length);
	}
	for (i = 0; i < 16; i++){
		MAP_MODBUS_HOLDREG(157 + i, counter_module[0].chanel[i].piece_interval);
	}
	for (i = 0; i < 16; i++){
		MAP_MODBUS_HOLDREG(173 + i, counter_module[0].chanel[i].close_door_interval);
	}
	for (i = 0; i < 16; i++){
		MAP_MODBUS_HOLDREG(189 + i, counter_module[0].chanel[i].door_switch_interval);
	}//205
	MAP_MODBUS_HOLDREG(286, fill_bottle_module[0].fill_complete_delay);
	MAP_MODBUS_HOLDREG(287, fill_bottle_module[0].input_buf_map);
	MAP_MODBUS_HOLDREG(288, fill_bottle_module[0].output_buf_map);
	MAP_MODBUS_HOLDREG(289, counter_module[0].max_length);
	MAP_MODBUS_HOLDREG(290, counter_module[0].min_length);
	MAP_MODBUS_HOLDREG(291, counter_module[0].min_piece_interval);
	MAP_MODBUS_HOLDREG(292, counter_module[0].min_close_door_interval);
	MAP_MODBUS_HOLDREG(293, counter_module[0].min_door_switch_interval);
	MAP_MODBUS_HOLDREG(294, counter_env.auto_scan);
	MAP_MODBUS_HOLDREG(295, counter_env.scan_process_time);
	MAP_MODBUS_HOLDREG(296, counter_env.max_scan_process_time);
	MAP_MODBUS_HOLDREG(297, counter_env.min_scan_process_time);
	MAP_MODBUS_HOLDREG(298, counter_module[0].normal_count_rej_flag);
	MAP_MODBUS_HOLDREG(299, counter_module[0].pre_count_rej_flag);
	MAP_MODBUS_HOLDREG(300, Modbus_test_value);
	MAP_MODBUS_HOLDREG(301, counter_module[0].counter_time);
	
	save_para (0); //save_para(1) �������save_para(0) ��ȡ����
	Modbus_HoldReg_NULL = 999;
}
//


void get_reg (s_reg_file *reg_file)
{
	uint16_t i;
//	W25QXX_Read ((U8*)reg_file, FLASH_REG_INFO_ADDR, sizeof(s_reg_file));
	STMFLASH_Read (FLASH_REG_INFO_ADDR, (uint16_t *)reg_file, sizeof(s_reg_file) / 2);
	my_println ();
	my_println ("-----------------------------------------------------------------");
	my_print("read reg info: ");
	for (i = 0; i < REG_SIZE; i++){
		my_print("%02x", reg_file->reg_info[i]);
	}
	my_println ();
	my_println ("-----------------------------------------------------------------");
}
int16_t check_reg_info (int16_t flag)
{	
	int16_t i;
	char id[32];
	char hash[16];
//	char reg_temp[REG_SIZE];
	s_reg_file *p_reg_file = NULL;
	
	p_reg_file = malloc (sizeof (s_reg_file));
	if (p_reg_file == NULL){
		my_println ("malloc (sizeof (s_reg_file)) failed! save para abort!");
		return -1;
	}
	memset (id, 0, sizeof(id));
	GetLockCode (id);
	MD5Digest(id, hash);
//	sprintf((char *)reg_temp, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
//		hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7],
//		hash[8], hash[9], hash[10], hash[11], hash[12], hash[13], hash[14], hash[15]
//	);
	get_reg (p_reg_file);
	for (i = 0; i < REG_SIZE; i++){
		if (p_reg_file->reg_info[i] != hash[i]){
			flag = -1;
			break;
		}
	}

	if (p_reg_file != NULL){
		free (p_reg_file);
	}
	return flag;
}
//
void read_para_from_flash (s_para_data_file * p_spi_flash_data)
{
//		W25QXX_Read ((U8*), FLASH_DATA_ADDR, sizeof(s_para_data_file));
	STMFLASH_Read (FLASH_DATA_ADDR, (uint16_t *)p_spi_flash_data, sizeof(s_para_data_file) / 2);
}
//
void save_para_to_flash (s_para_data_file *p_spi_flash_data)
{
//		W25QXX_Write ((U8*), FLASH_DATA_ADDR, sizeof(s_para_data_file));
	STMFLASH_Write (FLASH_DATA_ADDR, (uint16_t *)p_spi_flash_data, sizeof(s_para_data_file) / 2);
}
//
int16_t save_para (int16_t flag)
{
	int16_t i;
	s_para_data_file *p_spi_flash_data = NULL;

	if (flag == 1){
		if (data_change_flag == 0)
			return 0;
		else
			data_change_flag = 0;
	}
	p_spi_flash_data = malloc (sizeof (s_para_data_file));
	if (p_spi_flash_data == NULL){
		my_println ("malloc (sizeof (s_para_data_file)) failed! save para abort!");
		return -1;
	}
//check reg info*****************************************************************************
	flag = check_reg_info (flag);
//check reg info*****************************************************************************
	read_para_from_flash (p_spi_flash_data);
	if (flag > 0){//�������
		for (i = 0; i < MODBUS_SAVE_DATA_NUM; i++){
			p_spi_flash_data->SAVE_DATA[i] = *Modbus_HoldReg[MODBUS_SAVE_DATA_START + i];
		}
		save_para_to_flash (p_spi_flash_data);
	}else if(flag == 0){//��ȡ����
		chack_para_flag (p_spi_flash_data);
		for (i = 0; i < MODBUS_SAVE_DATA_NUM; i++){
			*Modbus_HoldReg[MODBUS_SAVE_DATA_START + i] = p_spi_flash_data->SAVE_DATA[i];
		}
	}
	if (flag == -1){
		my_env.is_registered = NOT_REGISTERED;
		my_println("-_-");//δע��
	}else{
		my_env.is_registered = REGISTERED;
		my_println("^_^");//��ע��
		cmd ();
	}
	if (p_spi_flash_data != NULL){
		free (p_spi_flash_data);
	}
	return 0;
}
//
void chack_para_flag (s_para_data_file *p_spi_flash_data)
{
	#define MAGIC_PARA_NUM 0xA5A1
	int16_t i, j;
	s_counter_module * p_counter_module;
	s_fill_bottle_module * p_fill_bottle_module;
	if (p_spi_flash_data->para_flag != MAGIC_PARA_NUM){
		p_spi_flash_data->para_flag = MAGIC_PARA_NUM;
		///////////////////////////////////////////////////////////////
		//////////////////////////Ĭ�ϲ���/////////////////////////////
		for (j = 0; j < COUNTER_MODULE_NUM; j++){
			p_counter_module = &counter_module[j];
			p_fill_bottle_module = &fill_bottle_module[j];
			//////////////////////////////////////////////
			SET_COUNT = 100; // ������
			SET_PRE_COUNT = 40; // Ԥ����
			SET_START_REJECT = 3; // �����޳�
			SET_MAX_LENGTH = 18; // ��󳤶�
			SET_MIN_LENGTH = 10; // ��С����
			SET_DOOR_CLOSE_DELAY = 16; // ������ʱ
			SET_DOOR_CLOSE_INTERVAL = 30; // С���Ź��ż��
			SET_DOOR_SWITCH_INTERVAL = 200; // С���ſ��ؼ��
			SET_SIM_LENGTH_X = 10; // 
			SET_SIM_LENGTH_Y = 20; // 
			SET_SIM_INTERVAL_X = 35; // 
			SET_SIM_INTERVAL_Y = 200; // 
			SET_SIM_SCAN_CTR = 30; // 
		
			SET_FILL_WAIT_PIECE_FALL_TIME = 450; // װƿ��ʱ
			SET_FILL_OPEN_BIG_GATE_DOOR_DELAY = 100; // �����Ŵ���ʱ
			SET_FILL_CLOSE_BIG_GATE_DOOR_DELAY = 20; // �����Źر���ʱ
			SET_FILL_OPEN_SHIFT_BOTTLE_DELAY = 350; // ��λ��ʱ
			SET_FILL_REJECT_START_DELAY = 2000; // �޳���ʱ
			SET_FILL_REJECT_KEEP_DELAY = 1000; // �޳�����ʱ��
			SET_FILL_NOZZLE_VIB_DOWN_TIME = 30; //��������ʱ��
			SET_FILL_NOZZLE_VIB_UP_TIME = 30; //�����½�ʱ��
		}
		///////////////////////////////////////////////////////////////
		for (i = 0; i < MODBUS_SAVE_DATA_NUM; i++){
			p_spi_flash_data->SAVE_DATA[i] = *Modbus_HoldReg[MODBUS_SAVE_DATA_START + i];
		}
		save_para_to_flash (p_spi_flash_data);
	}
}
//
void check_data (void)
{
	int16_t j;
	s_counter_module * p_counter_module;
	s_fill_bottle_module * p_fill_bottle_module;
	for (j = 0; j < COUNTER_MODULE_NUM; j++){
		p_counter_module = &counter_module[j];
		p_fill_bottle_module = &fill_bottle_module[j];
		///////////////////////////////////////////////////////////////
		LIMIT_MIN_VALUE (SET_MIN_LENGTH, 1);
		LIMIT_MIN_VALUE (SET_FILL_WAIT_PIECE_FALL_TIME, 10);
		LIMIT_MIN_VALUE (SET_FILL_OPEN_SHIFT_BOTTLE_DELAY, 10);
		///////////////////////////////////////////////////////////////
	}
}
//
void modbus_init(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*����GPIOB��GPIOF������ʱ��*/
	RCC_APB2PeriphClockCmd( HMI_RS485_TX_EN_RCC_PORT, ENABLE);
	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIOB����*/
	GPIO_InitStructure.GPIO_Pin = IO_RS485_TX_EN_PIN_MAP;
	GPIO_Init(IO_RS485_TX_EN_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = HMI_RS485_TX_EN_PIN_MAP;
	GPIO_Init(HMI_RS485_TX_EN_PORT, &GPIO_InitStructure);
	/* �ر�����led��	*/
	GPIO_SetBits(IO_RS485_TX_EN_PORT, IO_RS485_TX_EN_PIN_MAP);
	GPIO_SetBits(HMI_RS485_TX_EN_PORT, HMI_RS485_TX_EN_PIN_MAP);

	IO_RS485_TX_EN=0;//��������ģʽ
	HMI_RS485_TX_EN=0;//��������ģʽ

	Modbus_RegMap ();
	check_data ();
}
void Modbus_01_Handle(u8 * _data_buf);
void Modbus_02_Handle(u8 * _data_buf);
void Modbus_03_Handle(u8 * _data_buf);
void Modbus_05_Handle(u8 * _data_buf);
void Modbus_06_Handle(u8 * _data_buf);
void Modbus_15_Handle(u8 * _data_buf);
void Modbus_16_Handle(u8 * _data_buf);

//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len�����͵��ֽ���
void RS485_SendData(u8 *buff, u8 len)
{
	if (RS485_tx_buf[1] & 0x80){
		Modbus_com_error++;
	}
	if (modbus_ch == 1){
		start_uart1_dma ((uint32_t)RS485_tx_buf, len);
	}else if (modbus_ch == 3){
		HMI_RS485_TX_EN = 1;//�л�Ϊ����ģʽ
		start_uart3_dma ((uint32_t)RS485_tx_buf, len);
//		uart3_send_data (RS485_tx_buf, len);
//		OSQPost(modbusRTU_msg, (void *) 0xaa);//������Ϣ
	}
	modbus_ch = 0;
}

void modbus_analyze(u8 * _data_buf, uint16_t chanel, uint16_t len)
{
	u16 crc;
	u16 startRegAddr;
	if((_data_buf[1]==01)||(_data_buf[1]==02)||(_data_buf[1]==03)||(_data_buf[1]==05)||(_data_buf[1]==06)||(_data_buf[1]==15)||(_data_buf[1]==16)){//��������ȷ
		modbus_ch = chanel;
		startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
		if(startRegAddr < MODBUS_REG_NUM+1){//�Ĵ�����ַ�ڷ�Χ��
			crc = _data_buf[len - 2] << 8 | _data_buf[len - 1];
			if (crc == CRC16 (_data_buf, len - 2)){//CRCУ����ȷ
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				switch(_data_buf[1])//���ݲ�ͬ�Ĺ�������д���
				{
					case 1:{//�����������
						Modbus_01_Handle(_data_buf);
						break;
					}
					case 2:{//�����뿪����
						Modbus_02_Handle(_data_buf);
						break;
					}
					case 03:{//������Ĵ���
						Modbus_03_Handle(_data_buf);
						break;
					}
					case 5:{//д�������������
						Modbus_05_Handle(_data_buf);
						OSQPost(io_msg, (void *) 0xaa);//������Ϣ
						break;
					}
					case 06: {//д�����Ĵ���
						Modbus_06_Handle(_data_buf);
						break;
					}
					case 15:{//д������������
						Modbus_15_Handle(_data_buf);
						OSQPost(io_msg, (void *) 0xaa);//������Ϣ
						break;
					}
					case 16:{ //д����Ĵ���
						Modbus_16_Handle(_data_buf);
						break;
					}
				}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}else{//CRCУ�����
				RS485_tx_buf[0]=_data_buf[0];
				RS485_tx_buf[1]=_data_buf[1]| 0x80;
				RS485_tx_buf[2]=0x04; //�쳣��
				RS485_SendData(RS485_tx_buf,3);
			}
		}else{//�Ĵ�����ַ������Χ
			RS485_tx_buf[0]=_data_buf[0];
			RS485_tx_buf[1]=_data_buf[1]|0x80;
			RS485_tx_buf[2]=0x02; //�쳣��
			RS485_SendData(RS485_tx_buf,3);
		}
	}else{//���������
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x01; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}


//Modbus������01�������
//�����������
void Modbus_01_Handle(u8 * _data_buf)
{
	u16 ByteNum;
	u16 i;
	u16 calCRC;
	u16 RegNum;
	u16 startRegAddr;

	startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
	RegNum= (((u16)_data_buf[4])<<8)|_data_buf[5];//��ȡ�Ĵ�������
	if((startRegAddr+RegNum - 1)<MODBUS_OUTPUT_NUM)//�Ĵ�����ַ+�����ڷ�Χ��
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1];
		ByteNum=RegNum/8;//�ֽ���
		if(RegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
		RS485_tx_buf[2]=ByteNum;//����Ҫ��ȡ���ֽ���
		for(i=0;i<RegNum;i++)
		{
			if(i%8==0) RS485_tx_buf[3+i/8]=0x00; //���ֽڿ�ʼ
			RS485_tx_buf[3+i/8] >>= 1;//��λ�ȷ���
			RS485_tx_buf[3+i/8] |= ((*Modbus_OutputIO[startRegAddr+i])<<7)&0x80;
			if(i==RegNum-1)//���͵����һ��λ��
			{
				if(RegNum%8) RS485_tx_buf[3+i/8] >>= 8-(RegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
			}
		}
		calCRC=CRC16(RS485_tx_buf,ByteNum+3);
		RS485_tx_buf[ByteNum+3]=(calCRC>>8)&0xFF;
		RS485_tx_buf[ByteNum+4]=calCRC&0xFF;
		RS485_SendData(RS485_tx_buf,ByteNum+5);
	}
	else//�Ĵ�����ַ+����������Χ
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x02; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}


//Modbus������02�������
//�����뿪����
void Modbus_02_Handle(u8 * _data_buf)
{
	u16 ByteNum;
	u16 i;
	u16 calCRC;
	u16 RegNum;
	u16 startRegAddr;

	startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
	RegNum= (((u16)_data_buf[4])<<8)|_data_buf[5];//��ȡ�Ĵ�������
	if((startRegAddr+RegNum - 1)<MODBUS_INPUT_NUM)//�Ĵ�����ַ+�����ڷ�Χ��
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1];
		ByteNum=RegNum/8;//�ֽ���
		if(RegNum%8) ByteNum+=1;//���λ���������������ֽ���+1
		RS485_tx_buf[2]=ByteNum;//����Ҫ��ȡ���ֽ���
		for(i=0;i<RegNum;i++)
		{
			if(i%8==0) RS485_tx_buf[3+i/8]=0x00;
			RS485_tx_buf[3+i/8]>>=1;//��λ�ȷ���
			RS485_tx_buf[3+i/8]|=((*Modbus_InputIO[startRegAddr+i])<<7)&0x80;
			if(i==RegNum-1)//���͵����һ��λ��
			{
				if(RegNum%8) RS485_tx_buf[3+i/8]>>=8-(RegNum%8);//������һ���ֽڻ�����������ʣ��MSB���0
			}
		}
		calCRC=CRC16(RS485_tx_buf,ByteNum+3);
		RS485_tx_buf[ByteNum+3]=(calCRC>>8)&0xFF;
		RS485_tx_buf[ByteNum+4]=calCRC&0xFF;
		RS485_SendData(RS485_tx_buf,ByteNum+5);
	}
	else//�Ĵ�����ַ+����������Χ
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x02; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}


//Modbus������03�������
//�����ּĴ���
void Modbus_03_Handle(u8 * _data_buf)
{
	u8 i;
	u16 calCRC;
	u16 RegNum;
	u16 startRegAddr;

	startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
	RegNum= (((u16)_data_buf[4])<<8)|_data_buf[5];//��ȡ�Ĵ�������
	if((startRegAddr+RegNum - 1)<MODBUS_REG_NUM)//�Ĵ�����ַ+�����ڷ�Χ��
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1];
		RS485_tx_buf[2]=RegNum * 2;
		for(i=0;i<RegNum;i++)
		{
			RS485_tx_buf[3+i*2] = (*Modbus_HoldReg[startRegAddr+i] >> 8) & 0xFF; //�ȷ��͸��ֽ�
			RS485_tx_buf[4+i*2] = *Modbus_HoldReg[startRegAddr+i] & 0xFF;//���͵��ֽ�
		}
		calCRC=CRC16(RS485_tx_buf,RegNum*2+3);

        RS485_tx_buf[RegNum*2+3]=(calCRC>>8)&0xFF;
        RS485_tx_buf[RegNum*2+4]=calCRC&0xFF;
		RS485_SendData(RS485_tx_buf,RegNum*2+5);
	}
	else//�Ĵ�����ַ+����������Χ
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x02; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}



//Modbus������05�������
//д�������������
void Modbus_05_Handle(u8 * _data_buf)
{
	u16 calCRC;
	u16 startRegAddr;

	startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
	if(startRegAddr < MODBUS_OUTPUT_NUM)//�Ĵ�����ַ�ڷ�Χ��
	{
		if((_data_buf[4]==0xFF)||(_data_buf[5]==0xFF)) *Modbus_OutputIO[startRegAddr]=0x01;
		else *Modbus_OutputIO[startRegAddr]=0x00;

		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1];
		RS485_tx_buf[2]=_data_buf[2];
		RS485_tx_buf[3]=_data_buf[3];
		RS485_tx_buf[4]=_data_buf[4];
		RS485_tx_buf[5]=_data_buf[5];

		calCRC=CRC16(RS485_tx_buf,6);
		RS485_tx_buf[6]=(calCRC>>8)&0xFF;
		RS485_tx_buf[7]=calCRC&0xFF;
		RS485_SendData(RS485_tx_buf,8);
	}
	else//�Ĵ�����ַ������Χ
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x02; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}


//Modbus������06�������
//д�������ּĴ���
void Modbus_06_Handle(u8 * _data_buf)
{
	u16 calCRC;
	u16 startRegAddr;

	startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
	if(startRegAddr < MODBUS_REG_NUM){//�Ĵ�����ַ�ڷ�Χ��
	
		*Modbus_HoldReg[startRegAddr] = ((u16)_data_buf[4]) << 8;//���ֽ���ǰ
		*Modbus_HoldReg[startRegAddr] |= _data_buf[5];//���ֽ��ں�

		check_data ();
		if (((startRegAddr < MODBUS_SAVE_DATA_START + MODBUS_SAVE_DATA_NUM + 1) && (startRegAddr >= MODBUS_SAVE_DATA_START))){
			data_change_flag = 1;
			save_para (1); //save_para(1) �������save_para(0) ��ȡ����
//			inc_Modbus_test_value ();
		}

		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1];
		RS485_tx_buf[2]=_data_buf[2];
		RS485_tx_buf[3]=_data_buf[3];
		RS485_tx_buf[4]=_data_buf[4];
		RS485_tx_buf[5]=_data_buf[5];

		calCRC=CRC16(RS485_tx_buf,6);
		RS485_tx_buf[6]=(calCRC>>8)&0xFF;
		RS485_tx_buf[7]=calCRC&0xFF;
		RS485_SendData(RS485_tx_buf,8);
	}
	else//�Ĵ�����ַ������Χ
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x02; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}
//Modbus������15�������
//д������������
void Modbus_15_Handle(u8 * _data_buf)
{
	u16 i;
	u16 calCRC;
	u16 RegNum;
	u16 startRegAddr;

	startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
	RegNum=(((u16)_data_buf[4])<<8)|_data_buf[5];//��ȡ�Ĵ�������
	if((startRegAddr+RegNum - 1)<MODBUS_OUTPUT_NUM)//�Ĵ�����ַ+�����ڷ�Χ��
	{
		for(i=0;i<RegNum;i++)
		{
		        if(_data_buf[7+i/8]&0x01) *Modbus_OutputIO[startRegAddr+i]=0x01;
		        else *Modbus_OutputIO[startRegAddr+i]=0x00;
		        _data_buf[7+i/8]>>=1;//�ӵ�λ��ʼ
		}

		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1];
		RS485_tx_buf[2]=_data_buf[2];
		RS485_tx_buf[3]=_data_buf[3];
		RS485_tx_buf[4]=_data_buf[4];
		RS485_tx_buf[5]=_data_buf[5];
		calCRC=CRC16(RS485_tx_buf,6);
		RS485_tx_buf[6]=(calCRC>>8)&0xFF;
		RS485_tx_buf[7]=calCRC&0xFF;
		RS485_SendData(RS485_tx_buf,8);
	}
	else//�Ĵ�����ַ+����������Χ
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x02; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}
//Modbus������16�������
//д������ּĴ���
void Modbus_16_Handle(u8 * _data_buf)
{
	u8 i;
	u16 calCRC;
	u16 RegNum;
	u16 startRegAddr;

	startRegAddr=(((u16)_data_buf[2])<<8)|_data_buf[3];//��ȡ�Ĵ�����ʼ��ַ
	RegNum= (((u16)_data_buf[4])<<8)|_data_buf[5];//��ȡ�Ĵ�������
	if((startRegAddr+RegNum - 1)<MODBUS_REG_NUM)//�Ĵ�����ַ+�����ڷ�Χ��
	{
		for(i=0;i<RegNum;i++){
			*Modbus_HoldReg[startRegAddr+i]=((u16)_data_buf[7+i*2])<<8; //���ֽ���ǰ
			*Modbus_HoldReg[startRegAddr+i]|=_data_buf[8+i*2]; //���ֽ��ں�
			if ((startRegAddr+i < MODBUS_SAVE_DATA_START + MODBUS_SAVE_DATA_NUM + 1) && (startRegAddr+i >= MODBUS_SAVE_DATA_START)){
				data_change_flag = 1;
			}
		}
		check_data ();
		if (data_change_flag == 1){
			save_para (1); //save_para(1) �������save_para(0) ��ȡ����
		}

		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1];
		RS485_tx_buf[2]=_data_buf[2];
		RS485_tx_buf[3]=_data_buf[3];
		RS485_tx_buf[4]=_data_buf[4];
		RS485_tx_buf[5]=_data_buf[5];

		calCRC=CRC16(RS485_tx_buf,6);
		RS485_tx_buf[6]=(calCRC>>8)&0xFF;
		RS485_tx_buf[7]=calCRC&0xFF;
		RS485_SendData(RS485_tx_buf,8);

		data_change_flag = 1;
	}
	else//�Ĵ�����ַ+����������Χ
	{
		RS485_tx_buf[0]=_data_buf[0];
		RS485_tx_buf[1]=_data_buf[1]|0x80;
		RS485_tx_buf[2]=0x02; //�쳣��
		RS485_SendData(RS485_tx_buf,3);
	}
}

//

void Modbus_master_03_Handle(u8 * _data_buf)
{
	u8 i;
	u16 RegNum;
	u16 startRegAddr;
	s_MB_M_Frame_head *p_modbus_head;

	p_modbus_head = (s_MB_M_Frame_head*)_data_buf;
	if ((p_modbus_head->addr_d == modbus_head.addr_d) && 
			(p_modbus_head->func == modbus_head.func) 
		)
	{
		startRegAddr = modbus_head.addr_r;//��ȡ�Ĵ�����ʼ��ַ
		RegNum= modbus_head.data_len;//��ȡ�Ĵ�������
		if ((startRegAddr + RegNum) < MODBUS_DA_NUM)
		for (i = 0; i < RegNum; i++){
			counter_env.counter_da_module_read_back[startRegAddr + i] = (_data_buf[3 + i*2] << 8) |  _data_buf[4 + i*2];
		}
	}
}
//
void Modbus_master_16_Handle(u8 * _data_buf)
{
	s_MB_M_Frame_head *p_modbus_head;

	p_modbus_head = (s_MB_M_Frame_head*)_data_buf;
	if ((p_modbus_head->addr_d == modbus_head.addr_d) && 
			(p_modbus_head->func == modbus_head.func)
		)
	{
		memset(&modbus_head, sizeof(modbus_head), 0);
	}
}
//
s_MB_M_Frame_head modbus_head; 
void modbus_master_process (u8 * _data_buf, uint16_t chanel, uint16_t len)
{
	u16 crc;
	if((_data_buf[1]==01)||(_data_buf[1]==02)||(_data_buf[1]==03)||(_data_buf[1]==05)||(_data_buf[1]==06)||(_data_buf[1]==15)||(_data_buf[1]==16)){//��������ȷ
		modbus_ch = chanel;
			crc = _data_buf[len - 2] << 8 | _data_buf[len - 1];
			if (crc == CRC16 (_data_buf, len - 2)){//CRCУ����ȷ
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				switch(_data_buf[1])//���ݲ�ͬ�Ĺ�������д���
				{
					case 03://������Ĵ���
						Modbus_master_03_Handle(_data_buf);
						break;
					case 16://д����Ĵ���
						Modbus_master_16_Handle(_data_buf);
						break;
					default:break;
				}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
	}
}
//
uint8_t req_modbus_da[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x06, 0xc5, 0xc8}; 

uint8_t write_modbus_da[] = {0x01, 0x10, 0x00, 0x00, 0x00, 0x06, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xD7}; 

void modbus_poll (void)
{
	uint16_t status = 0;
	uint16_t calCRC, i;
	
//	if (GET_BIT(fill_bottle_module[0].output_buf_manual, OUTPUT_BUF_VIB) == 0){
//		for (i = 0; i < MODBUS_DA_NUM; i++){
//			if (counter_env.counter_da_module_read_back[i] != counter_env.counter_da_module_test[i]){
//				counter_env.counter_da_module_read_back[i] = counter_env.counter_da_module_test[i];
//				status = 1;
//			}
//		}
//	}else{
//		for (i = 0; i < MODBUS_DA_NUM; i++){
//			counter_env.counter_da_module_real[i] = (counter_env.counter_da_module[i] * counter_env.counter_da_module_apply[i]) / 1000;
//			if (counter_env.counter_da_module_read_back[i] != counter_env.counter_da_module_real[i]){
//				counter_env.counter_da_module_read_back[i] = counter_env.counter_da_module_real[i];
//				status = 1;
//			}
//		}
//	}
	
		for (i = 0; i < MODBUS_DA_NUM; i++){
			if (counter_env.counter_da_module_read_back[i] != counter_env.counter_da_module[i]){
				counter_env.counter_da_module_read_back[i] = counter_env.counter_da_module[i];
				status = 1;
			}
		}
	
	if (status == 1){		
		my_println ("write da module");
		modbus_head.addr_d = 0x01;
		modbus_head.func = 0x10;
		modbus_head.addr_r = 0x0000;
		modbus_head.data_len = 0x0006;
		for (i = 0; i < 6; i++){
			uint16_t DA_temp;
			if (GET_BIT(fill_bottle_module[0].output_buf_manual, OUTPUT_BUF_VIB) == 0){
				my_println ("vib test mode");
			}
			counter_env.counter_da_module_read_back[i] = counter_env.counter_da_module[i];
			DA_temp = counter_env.counter_da_module_read_back[i];
			write_modbus_da[7 + i * 2] = (uint8_t)((DA_temp & 0xFF00) >> 8);
			write_modbus_da[7 + i * 2 + 1] = (uint8_t)(DA_temp & 0xFF);
		}
		calCRC=CRC16(write_modbus_da, sizeof (write_modbus_da) - 2);
    write_modbus_da[sizeof (write_modbus_da) - 2]=(calCRC>>8)&0xFF;
    write_modbus_da[sizeof (write_modbus_da) - 1]=calCRC&0xFF;
		start_uart2_dma ((uint32_t)write_modbus_da, sizeof(write_modbus_da));
		status = 0;
	}else{
		modbus_head.addr_d = 0x01;
		modbus_head.func = 0x03;
		modbus_head.addr_r = 0x0000;
		modbus_head.data_len = 0x0006;
		start_uart2_dma ((uint32_t)req_modbus_da, sizeof(req_modbus_da));
		status = 1;
	}
}

void HardFault_Handler (void)
{
	while (1);
}










