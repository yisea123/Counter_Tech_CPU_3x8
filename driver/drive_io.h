#ifndef __DRIVE_OUTPUT_H
#define __DRIVE_OUTPUT_H




#define EXT_IN0  PDin(10) //C_10
#define EXT_IN1  PDin(11)
#define EXT_IN2  PDin(12) //C_09
#define EXT_IN3  PDin(13)
#define EXT_IN4  PDin(14) //C_08
#define EXT_IN5  PDin(15) //C_07
#define EXT_IN6  PDin(0) //C_06
#define EXT_IN7  PDin(1)
#define EXT_IN8  PDin(2) //C_05
#define EXT_IN9  PDin(3)
#define EXT_IN10 PDin(4) //C_04
#define EXT_IN11 PDin(5)
#define EXT_IN12 PDin(6) //C_03
#define EXT_IN13 PDin(7) //C_02
#define EXT_IN14 PEin(0) //C_01
#define EXT_IN15 PEin(1) //C_00

#define EXT_IN16 PEin(2)
#define EXT_IN17 PEin(3)
#define EXT_IN18 PEin(4)
#define EXT_IN19 PEin(5)
#define EXT_IN20 PEin(6)
#define EXT_IN21 PEin(7)
#define EXT_IN22 PEin(8)
#define EXT_IN23 PEin(9)
#define EXT_IN24 PEin(10)
#define EXT_IN25 PEin(11)
#define EXT_IN26 PEin(12)
#define EXT_IN27 PEin(13)
#define EXT_IN28 PEin(14)
#define EXT_IN29 PEin(15)
#define EXT_IN30 PDin(8) //C_11
#define EXT_IN31 PDin(9)

//#define EXT_IN31 PDin(10)
//#define EXT_IN30 PDin(11)
//#define EXT_IN29 PDin(12)
//#define EXT_IN28 PDin(13)
//#define EXT_IN27 PDin(14)
//#define EXT_IN26 PDin(15)
//#define EXT_IN25 PDin(0)
//#define EXT_IN24 PDin(1)
//#define EXT_IN23 PDin(2)
//#define EXT_IN22 PDin(3)
//#define EXT_IN21 PDin(4)
//#define EXT_IN20 PDin(5)
//#define EXT_IN19 PDin(6)
//#define EXT_IN18 PDin(7)
//#define EXT_IN17 PEin(0)
//#define EXT_IN16 PEin(1)

//#define EXT_IN15 PEin(2)
//#define EXT_IN14 PEin(3)
//#define EXT_IN13 PEin(4)
//#define EXT_IN12 PEin(5)
//#define EXT_IN11 PEin(6)
//#define EXT_IN10 PEin(7)
//#define EXT_IN9  PEin(8)
//#define EXT_IN8  PEin(9)
//#define EXT_IN7  PEin(10)
//#define EXT_IN6  PEin(11)
//#define EXT_IN5  PEin(12)
//#define EXT_IN4  PEin(13)
//#define EXT_IN3  PEin(14)
//#define EXT_IN2  PEin(15)
//#define EXT_IN1  PDin(8)
//#define EXT_IN0  PDin(9)

#ifdef BOARD_V1_0
#define COUNTER_INPUT0 PEin(1)
#define COUNTER_INPUT1 PEin(0)
#define COUNTER_INPUT2 PDin(7)
#define COUNTER_INPUT3 PDin(6)
#define COUNTER_INPUT4 PDin(4)
#define COUNTER_INPUT5 PDin(2)
#define COUNTER_INPUT6 PDin(0)
#define COUNTER_INPUT7 PDin(15)
#define COUNTER_INPUT8 PDin(14)
#define COUNTER_INPUT9 PDin(12)
#define COUNTER_INPUT10 PDin(10)
#define COUNTER_INPUT11 PDin(8)
#else
#define COUNTER_INPUT0 PBin(6)
#define COUNTER_INPUT1 PBin(4)
#define COUNTER_INPUT2 PBin(3)
#define COUNTER_INPUT3 PCin(11)
#define COUNTER_INPUT4 PCin(12)
#define COUNTER_INPUT5 PAin(15)
#define COUNTER_INPUT6 PCin(10)
#define COUNTER_INPUT7 PAin(11)
#define COUNTER_INPUT8 PAin(12)
#define COUNTER_INPUT9 PAin(8)
#define COUNTER_INPUT10 PCin(9)
#define COUNTER_INPUT11 PCin(7)
#endif



#ifdef BOARD_V1_0
#define CONFIG_INPUT0 PBin(3)
#define CONFIG_INPUT1 PBin(4)
#define CONFIG_INPUT2 PBin(5)
#define CONFIG_INPUT3 PBin(6)
#else
#define CONFIG_INPUT0 PCin(13)
#define CONFIG_INPUT1 PCin(15)
#define CONFIG_INPUT2 PCin(14)
#define CONFIG_INPUT3 PCin(0)
#endif


#define OUTPUT0 		GPIO_Pin_0
#define OUTPUT1 		GPIO_Pin_1
#define OUTPUT2 		GPIO_Pin_2
#define OUTPUT3 		GPIO_Pin_3
#define OUTPUT4 		GPIO_Pin_4
#define OUTPUT5 		GPIO_Pin_5
#define OUTPUT6 		GPIO_Pin_6
#define OUTPUT7 		GPIO_Pin_7
#define OUTPUT8 		GPIO_Pin_8
#define OUTPUT9 		GPIO_Pin_9
#define OUTPUT10 		GPIO_Pin_10
#define OUTPUT11 		GPIO_Pin_11
#define OUTPUT12		GPIO_Pin_12
#define OUTPUT13		GPIO_Pin_13
#define OUTPUT14		GPIO_Pin_14
#define OUTPUT15		GPIO_Pin_15

#define OUTPUT_PIN_MAP 				OUTPUT0 | OUTPUT1 | OUTPUT2 | OUTPUT3 | \
									OUTPUT4 | OUTPUT5 | OUTPUT6 | OUTPUT7 | \
									OUTPUT8 | OUTPUT9 | OUTPUT10 | OUTPUT11 | \
									OUTPUT12 | OUTPUT13 | OUTPUT14 | OUTPUT15

#define EXT_OUT0  PGout(3)
#define EXT_OUT1  PGout(5)
#define EXT_OUT2  PGout(7)
#define EXT_OUT3  PGout(9)
#define EXT_OUT4  PGout(11)
#define EXT_OUT5  PGout(13)
#define EXT_OUT6  PGout(15)
#define EXT_OUT7  PFout(0)
#define EXT_OUT8  PFout(2)
#define EXT_OUT9  PFout(4)
#define EXT_OUT10 PFout(6)
#define EXT_OUT11 PFout(8)
#define EXT_OUT12 PFout(12)
#define EXT_OUT13 PFout(14)
#define EXT_OUT14 PGout(0)
#define EXT_OUT15 PGout(1)

#define EXT_OUT16 PGout(2)
#define EXT_OUT17 PGout(4)
#define EXT_OUT18 PGout(6)
#define EXT_OUT19 PGout(8)
#define EXT_OUT20 PGout(10)
#define EXT_OUT21 PGout(12)
#define EXT_OUT22 PGout(14)
#define EXT_OUT23 PFout(1)
#define EXT_OUT24 PFout(3)
#define EXT_OUT25 PFout(5)
#define EXT_OUT26 PFout(7)
#define EXT_OUT27 PFout(9)
#define EXT_OUT28 PFout(10)
#define EXT_OUT29 PFout(11)
#define EXT_OUT30 PFout(13)
#define EXT_OUT31 PFout(15)

#define EXT_OUT32 EXT_MOTOR_PWM0

#define EXT_MOTOR_PWM0 PCout(1)

#define BOTTLE_BELT_OUTPUT EXT_OUT24
//////////////////////////////////////////////////////////////////////////
#define EMERGENCY_STOP_CH 			EXT_IN16
#define CONVEYOR_STOP_CH 				EXT_IN17
#define POWER_OFF_CH 						EXT_IN18
#define PRESSURE_LOW_CH 				EXT_IN19
#define BOTTLE_FALL_1_CH 				EXT_IN20
#define BOTTLE_FALL_2_CH 				EXT_IN20
#define BOTTLE_JAM_1_CH 				EXT_IN21
#define BOTTLE_JAM_2_CH 				EXT_IN21
#define LIFTER_UP_LIMIT_CH			EXT_IN22
#define LIFTER_DOWN_LIMIT_CH		EXT_IN23
#define EXT_RUNNING_SIGNAL 			EXT_IN24
#define EXT_FILL_BOTTLE_SIGNAL 	EXT_IN25
#define FRONT_BOTTLE_JAM_CH			EXT_IN26
#define SCREW_READY_SIGNAL 			EXT_IN27
#define VIB_1_OFF_CH							VIBRATOR_SWITCH_1
#define VIB_2_OFF_CH							VIBRATOR_SWITCH_2
#define VIB_UNLOCK_CH						VIB_LOCK_SWITCH
#define DETECT_ERROR_CH					DETECT_ERROR
#define PRODUCT_NUM_REACH_CH		PRODUCT_NUM_REACH
#define CONTINUE_REJ_STOP_CH		CONTINUE_REJ_STOP
//////////////////////////////////////////////////////////////////////////
#define UINT16_INPUT_FILTER_FUNC(IN_CH,F_TIME,ALARM_FLAG) uint16_t IN_CH##_FILTER (void) { \
	static uint16_t fall_filter_time = 0; \
	static uint16_t raise_filter_time = 0; \
	static uint16_t input_status = 0; \
	if (IN_CH == 0){ \
		if (fall_filter_time < F_TIME){ \
			fall_filter_time++; \
		}else{ \
			input_status = !ALARM_FLAG; \
		} \
		raise_filter_time = 0; \
	}else{ \
		fall_filter_time = 0; \
		if (raise_filter_time < F_TIME){ \
			raise_filter_time++; \
		}else{ \
			input_status = ALARM_FLAG; \
		} \
	} \
	return input_status; \
}
//////////////////////////////////////////////////////////////////////////
#define UPDATE_ALARM_STATUS(IN_CH,ALARM_VALUE) { \
	if (IN_CH##_FILTER()){ \
		SET_OUTPUT_ON (ALARM_VALUE, ALM_##IN_CH); \
	}else{ \
		SET_OUTPUT_OFF (ALARM_VALUE, ALM_##IN_CH); \
	} \
}
//////////////////////////////////////////////////////////////////////////



#define VIB_START (0)
#define VIB_STOP (1)


#define IS_SYS_RUNNING 0

void io_reset (void);
void drive_io_init (void);


extern int null_temp_value;

#endif

 
