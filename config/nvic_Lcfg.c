#include "nvic.h"
#include "nvic_cfg.h"

const Nvic_CfgType Nvic_Cfg[NVIC_ACTIVATED_INT_SIZE] ={ //Array of Nvic_CfgType structs
/* Interrupt_Number	    Group_Priority          SubGroup_Priority */
 { NVIC_GPIO_PORT_A,    2,                       1                  },
 { NVIC_UART_2,         1,                       0                  },
 { NVIC_UART_3,         2,                       0                  },
 { NVIC_16_32_TIMER_5_B,3,                       1                  },
 { NVIC_PWM_1_GEN_0,    0,                       0                  },
	
};
