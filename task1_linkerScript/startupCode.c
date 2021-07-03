
#include<stdint.h>
extern uint32_t __data_load__  ;
extern uint32_t __data_start__ ;
extern uint32_t __data_end__   ;
extern uint32_t __bss_start__  ;
extern uint32_t __bss_end__    ;
extern uint32_t _stack         ;

void ResetHandler(void)
{
  /*Init Stack pointer: Done By HW */
  
  
  
  uint32_t *Ptr2Sbss = (uint32_t*)&__bss_start__;
  uint32_t *Ptr2Ebss = (uint32_t*)&__bss_end__;
  
  uint32_t *Ptr2Src =  (uint32_t*)&__data_load__;
  uint32_t *Ptr2sDes = (uint32_t*)&__data_start__;
  uint32_t *Ptr2eDes = (uint32_t*)&__data_end__;
  
  /* Init .BSS */
  while(Ptr2Sbss< Ptr2Ebss)
  {
	  *(Ptr2Sbss++) = 0;
  }
  
  
  /*Init .DATA */
	while(Ptr2sDes< Ptr2eDes)
	{
		
		
		*(Ptr2sDes++) = *(Ptr2Src++);
	}
	

	/*jUMB tO MAIN */
	main();
	
	while(1);

}