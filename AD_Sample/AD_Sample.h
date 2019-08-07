#ifndef __AD_SAMPLE__
#define __AD_SAMPLE__

#define SAMPLE_NUM 8 //每通道采样次数
#define CHANEL_NUM 12 //为12个通道
#define CHANEL_SENSOR_NUM 8

#define START_DATA 5
#define PROCESS_RDY 15


#define WAVE_DOWN 	g_counter[0].set_wave_down_flag //16
#define WAVE_UP 	g_counter[0].set_wave_up_flag //16
#define WAVE_UP_V 	g_counter[0].set_wave_up_value //10

#define STD_REF_OFFSET (g_counter[0].std_ref_value_offset)
#define STD_REF_VALUE (g_counter[0].std_ref_value)
#define STD_REF_VALUE_L (STD_REF_VALUE - STD_REF_OFFSET)
#define STD_REF_VALUE_H (STD_REF_VALUE + STD_REF_OFFSET)
#define STD_DOWN_V_OFFSET g_counter[0].ch[i].std_down_offset
#define STD_UP_V_OFFSET g_counter[0].ch[i].std_up_offset

#define AD_MAX_V 4096 * 8

#define CH0 	ADC_Channel_0
#define CH1 	ADC_Channel_1
#define CH2 	ADC_Channel_2
#define CH3 	ADC_Channel_3
#define CH4 	ADC_Channel_14
#define CH5 	ADC_Channel_15
#define CH6 	ADC_Channel_6
#define CH7 	ADC_Channel_7
#define CH8 	ADC_Channel_8
#define CH9 	ADC_Channel_9
#define CH10 	ADC_Channel_10
#define CH11 	ADC_Channel_11
#define CH12 	ADC_Channel_12
#define CH13 	ADC_Channel_13


#define AD_BUFF_SIZE 1024 * 2
#define AD_FITTER_BUFF_SIZE 8

//#define SAMPLE_BUF_SIZE 512 //每通道采50次
#define SET_SENSOR_EN_PORT GPIOE
#define SET_SENSOR_EN_PIN GPIO_Pin_6
#define WRITE_SENSOR_ADDR(ADDR) GPIO_Write(GPIOE, ((GPIO_ReadOutputData(GPIOE) & (~(7 << 3))) | (ADDR<<3)))
#define RESET_SENSOR_EN() GPIO_ResetBits(SET_SENSOR_EN_PORT, SET_SENSOR_EN_PIN)
#define SET_SENSOR_EN() GPIO_SetBits(SET_SENSOR_EN_PORT, SET_SENSOR_EN_PIN)


#define OUTPUT_PIECE_SIGNAL(CH) { \
	CHANEL_##CH = g_counter[0].ch[CH].piece_in_status; \
}
#define OUTPUT_ALL_PIECE_SIGNAL() { \
	OUTPUT_PIECE_SIGNAL(0); \
	OUTPUT_PIECE_SIGNAL(1); \
	OUTPUT_PIECE_SIGNAL(2); \
	OUTPUT_PIECE_SIGNAL(3); \
	OUTPUT_PIECE_SIGNAL(4); \
	OUTPUT_PIECE_SIGNAL(5); \
	OUTPUT_PIECE_SIGNAL(6); \
	OUTPUT_PIECE_SIGNAL(7); \
	OUTPUT_PIECE_SIGNAL(8); \
	OUTPUT_PIECE_SIGNAL(9); \
	OUTPUT_PIECE_SIGNAL(10); \
	OUTPUT_PIECE_SIGNAL(11); \
}



#define ENABLE_SENSOR() SET_SENSOR_EN()
#define DISABLE_SENSOR() RESET_SENSOR_EN()


#define DETECTG_BUF_SIZE 1024 * 4
#define DETECTG_BUF_GROUP 2
#define AD_DMA_BUF_GROUP 2

typedef struct{
	u16 buffer[AD_BUFF_SIZE];
	u16 buffer_index;
	u16 buffer_en;
	u16 cir;
}s_buf;

typedef struct {
	u16 AD_Value_0[SAMPLE_NUM][CHANEL_NUM]; //用来存放ADC转换结果，也是DMA的目标地址
	u16 AD_Value_1[SAMPLE_NUM][CHANEL_NUM]; //用来存放ADC转换结果，也是DMA的目标地址
	u16 AD_Value_2[SAMPLE_NUM][CHANEL_NUM]; //用来存放ADC转换结果，也是DMA的目标地址
}s_AD_buf;

void AD_filter(U16 _detect_chanel_index, U16 _chanel_pos_index);
int AD_Sample_init (void);
void calibrate_IR (void);
uint32_t send_IR_value (void);
void re_calibration_detect (void);
u16 GetVolt(u16 advalue);
extern u16 After_filter[CHANEL_NUM]; //用来存放求平均值之后的结果
//extern vu16 Detect_Buf[DETECTG_BUF_GROUP][DETECTG_BUF_SIZE];
extern vu16 Detect_Buf[DETECTG_BUF_SIZE];

extern U16 ADC1_irq_cycle;
extern U16 ADC1_process_time;

extern u16 dma_irq_cycle;
extern u16 counter_process_time;

extern U16 detect_chanel_index;//检测通道索引
extern U16 chanel_pos_index;	//通道光敏二极管位置索引

extern s_buf AD_buff;
extern s_AD_buf AD_DMA_buf;


extern vu16 process_rdy;
extern u16 wave_down_flag;
extern u16 wave_up_flag;
extern u16 wave_up_value;

void AD_Start_Sample (u32 _memBaseAddr);

#endif

