;******************** (C) COPYRIGHT 2018 MindMotion ********************
;* File Name          : startup_MM32F103xCxE_o.s
;* Version            : V1.0.1
;* Date               : 2018/09/17
;* Description        : MM32F031 Medium-density devices vector table for EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == __iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR 
;*                        address
;*                      - Configure the system clock
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*******************************************************************************
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table 

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler                    ; NMI Handler
        DCD     HardFault_Handler              ; Hard Fault Handler
        DCD     MemManage_Handler              ; MPU Fault Handler
        DCD     BusFault_Handler               ; Bus Fault Handler
        DCD     UsageFault_Handler             ; Usage Fault Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     SVC_Handler                    ; SVCall Handler
        DCD     DebugMon_Handler               ; Debug Monitor Handler
        DCD     0                              ; Reserved
        DCD     PendSV_Handler                 ; PendSV Handler
        DCD     SysTick_Handler                ; SysTick Handler

        ; External Interrupts
        DCD     WWDG_IRQHandler                ; Window Watchdog
        DCD     PVD_IRQHandler                 ; PVD through EXTI Line detect
		DCD     TAMPER_IRQHandler              ; Intrusion detection interrupted
        DCD     RTC_IRQHandler                 ; RTC through EXTI Line
        DCD     FLASH_IRQHandler               ; FLASH
        DCD     RCC_CRS_IRQHandler             ; RCC and CRS
        DCD     EXTI0_IRQHandler               ; EXTI Line 0
        DCD     EXTI1_IRQHandler               ; EXTI Line 1
        DCD     EXTI2_IRQHandler               ; EXTI Line 2
        DCD     EXTI3_IRQHandler               ; EXTI Line 3
        DCD     EXTI4_IRQHandler               ; EXTI Line 4
        DCD     DMA1_Channel1_IRQHandler       ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQHandler       ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQHandler       ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQHandler       ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQHandler       ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQHandler       ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQHandler       ; DMA1 Channel 7
        DCD     ADC1_IRQHandler                ; ADC1 
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     CAN1_RX_IRQHandler             ; CAN1 
        DCD     0                              ; Reserved
        DCD     EXTI9_5_IRQHandler             ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQHandler            ; TIM1 Break
        DCD     TIM1_UP_IRQHandler             ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQHandler        ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler             ; TIM1 Capture Compare
        DCD     TIM2_IRQHandler                ; TIM2
        DCD     TIM3_IRQHandler                ; TIM3
        DCD     TIM4_IRQHandler                ; TIM4
        DCD     I2C1_IRQHandler                ; I2C1 Event
        DCD     0                              ; Reserved
        DCD     I2C2_IRQHandler                ; I2C2 Event
        DCD     0                              ; Reserved
        DCD     SPI1_IRQHandler                ; SPI1
        DCD     SPI2_IRQHandler                ; SPI2
        DCD     UART1_IRQHandler               ; UART1
        DCD     UART2_IRQHandler               ; UART2
        DCD     UART3_IRQHandler               ; UART3
        DCD     EXTI15_10_IRQHandler           ; EXTI Line 15..10
        DCD     RTCAlarm_IRQHandler            ; RTC Alarm through EXTI Line 17
        DCD     USB_WKUP_IRQHandler            ; USB Wakeup from suspend
        DCD     TIM8_BRK_IRQHandler            ; TIM8 Break
        DCD     TIM8_UP_IRQHandler             ; TIM8 Update
        DCD     TIM8_TRG_COM_IRQHandler        ; TIM8 Trigger and Commutation
        DCD     TIM8_CC_IRQHandler             ; TIM8 Capture Compare
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     SDIO_IRQHandler                ; SDIO
		DCD     TIM5_IRQHandler                ; TIM5
		DCD     SPI3_IRQHandler                ; SPI3
		DCD     UART4_IRQHandler               ; UART4
        DCD     UART5_IRQHandler               ; UART5
		DCD     TIM6_IRQHandler                ; TIM6
		DCD     TIM7_IRQHandler                ; TIM7
		DCD     DMA2_Channel1_IRQHandler       ; DMA2 Channel 1
        DCD     DMA2_Channel2_IRQHandler       ; DMA2 Channel 2
        DCD     DMA2_Channel3_IRQHandler       ; DMA2 Channel 3
        DCD     DMA2_Channel4_IRQHandler       ; DMA2 Channel 4
        DCD     DMA2_Channel5_IRQHandler       ; DMA2 Channel 5
		DCD     ETHERNET_MAC_IRQHandler        ; Ethernet
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     COMP1_2_IRQHandler             ; COMP1,COMP2
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     USB_OTG_FS_IRQHandler          ; USB_FS
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     UART6_IRQHandler               ; UART6  
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     AES_IRQHandler                 ; AES
		DCD     TRNG_IRQHandler                ; TRNG
		DCD     0                              ; Reserved
		DCD     UART7_IRQHandler               ; UART7
		DCD     UART8_IRQHandler               ; UART8
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved
		DCD     0                              ; Reserved 
        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
     
        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, = SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler
        
        
        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler
       
        
		PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
        B MemManage_Handler
       
        
		PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
        B BusFault_Handler
       
        
		PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler
       
        
        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler
       
        
		PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler
       
        
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler
        
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler
        
        
        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WWDG_IRQHandler
        B WWDG_IRQHandler
        
                
        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PVD_IRQHandler
        B PVD_IRQHandler
        
                
		PUBWEAK TAMPER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TAMPER_IRQHandler
        B TAMPER_IRQHandler
        
              
        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler
        
                
        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler
        
                
        PUBWEAK RCC_CRS_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RCC_CRS_IRQHandler
        B RCC_CRS_IRQHandler
        
                
        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI0_IRQHandler
        B EXTI0_IRQHandler
        
                
        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI1_IRQHandler
        B EXTI1_IRQHandler
        
                
        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI2_IRQHandler
        B EXTI2_IRQHandler
        
                    
        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler
        
                    
        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI4_IRQHandler
        B EXTI4_IRQHandler

		
        PUBWEAK DMA1_Channel1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel1_IRQHandler
        B DMA1_Channel1_IRQHandler
        
                
        PUBWEAK DMA1_Channel2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel2_IRQHandler
        B DMA1_Channel2_IRQHandler
        
                
        PUBWEAK DMA1_Channel3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel3_IRQHandler
        B DMA1_Channel3_IRQHandler
        
                      
        PUBWEAK DMA1_Channel4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel4_IRQHandler
        B DMA1_Channel4_IRQHandler
        
                     
        PUBWEAK DMA1_Channel5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel5_IRQHandler
        B DMA1_Channel5_IRQHandler
        
                     
        PUBWEAK DMA1_Channel6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel6_IRQHandler
        B DMA1_Channel6_IRQHandler
        
                     
        PUBWEAK DMA1_Channel7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Channel7_IRQHandler
        B DMA1_Channel7_IRQHandler
        
               
        PUBWEAK ADC1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC1_IRQHandler
        B ADC1_IRQHandler
        
                 
        PUBWEAK CAN1_RX_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
CAN1_RX_IRQHandler
        B CAN1_RX_IRQHandler
        
                    
        PUBWEAK EXTI9_5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI9_5_IRQHandler
        B EXTI9_5_IRQHandler
        
                    
        PUBWEAK TIM1_BRK_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_BRK_IRQHandler
        B TIM1_BRK_IRQHandler
        
                           
        PUBWEAK TIM1_UP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_UP_IRQHandler
        B TIM1_UP_IRQHandler
        
                               
        PUBWEAK TIM1_TRG_COM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_TRG_COM_IRQHandler
        B TIM1_TRG_COM_IRQHandler
        
            
        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler
        
                
        PUBWEAK TIM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM2_IRQHandler
        B TIM2_IRQHandler
        
                
        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM3_IRQHandler
        B TIM3_IRQHandler
        
                
        PUBWEAK TIM4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM4_IRQHandler
        B TIM4_IRQHandler
        
                    
        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_IRQHandler
        B I2C1_IRQHandler
        
                
        PUBWEAK I2C2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C2_IRQHandler
        B I2C2_IRQHandler
        
                
        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler
        
                
        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
        B SPI2_IRQHandler
        
                
        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART1_IRQHandler
        B UART1_IRQHandler
        
                
        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART2_IRQHandler
        B UART2_IRQHandler
        
         
        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART3_IRQHandler
        B UART3_IRQHandler
        
  
        PUBWEAK EXTI15_10_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI15_10_IRQHandler
        B EXTI15_10_IRQHandler
        
  
        PUBWEAK RTCAlarm_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTCAlarm_IRQHandler
        B RTCAlarm_IRQHandler
        
  
        PUBWEAK USB_WKUP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USB_WKUP_IRQHandler
        B USB_WKUP_IRQHandler
        
  
        PUBWEAK TIM8_BRK_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_BRK_IRQHandler
        B TIM8_BRK_IRQHandler
        
  
        PUBWEAK TIM8_UP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_UP_IRQHandler
        B TIM8_UP_IRQHandler
        
  
        PUBWEAK TIM8_TRG_COM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_TRG_COM_IRQHandler
        B TIM8_TRG_COM_IRQHandler
        
  
        PUBWEAK TIM8_CC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_CC_IRQHandler
        B TIM8_CC_IRQHandler
        
  
        PUBWEAK SDIO_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SDIO_IRQHandler
        B SDIO_IRQHandler
        
  
        PUBWEAK TIM5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM5_IRQHandler
        B TIM5_IRQHandler
        
  
        PUBWEAK SPI3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI3_IRQHandler
        B SPI3_IRQHandler
        
  
        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART4_IRQHandler
        B UART4_IRQHandler
        
  
        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART5_IRQHandler
        B UART5_IRQHandler
        
  
        PUBWEAK TIM6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM6_IRQHandler
        B TIM6_IRQHandler
        
  
        PUBWEAK TIM7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM7_IRQHandler
        B TIM7_IRQHandler
        
  
        PUBWEAK DMA2_Channel1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Channel1_IRQHandler
        B DMA2_Channel1_IRQHandler
        
  
        PUBWEAK DMA2_Channel2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Channel2_IRQHandler
        B DMA2_Channel2_IRQHandler
        
  
        PUBWEAK DMA2_Channel3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Channel3_IRQHandler
        B DMA2_Channel3_IRQHandler
        
  
        PUBWEAK DMA2_Channel4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Channel4_IRQHandler
        B DMA2_Channel4_IRQHandler
        
  
        PUBWEAK DMA2_Channel5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Channel5_IRQHandler
        B DMA2_Channel5_IRQHandler
        
  
        PUBWEAK ETHERNET_MAC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETHERNET_MAC_IRQHandler
        B ETHERNET_MAC_IRQHandler
        
  
        PUBWEAK COMP1_2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
COMP1_2_IRQHandler
        B COMP1_2_IRQHandler
        
  
        PUBWEAK USB_OTG_FS_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USB_OTG_FS_IRQHandler
        B USB_OTG_FS_IRQHandler
        
  
        PUBWEAK UART6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART6_IRQHandler
        B UART6_IRQHandler
        
  
        PUBWEAK AES_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
AES_IRQHandler
        B AES_IRQHandler
        
  
        PUBWEAK TRNG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TRNG_IRQHandler
        B TRNG_IRQHandler
        
  
        PUBWEAK UART7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART7_IRQHandler
        B UART7_IRQHandler
        
  
        PUBWEAK UART8_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART8_IRQHandler
        B UART8_IRQHandler
        
 
//        PUBWEAK ADC1_IRQHandler
//        SECTION .text:CODE:NOROOT:REORDER(1)
//ADC1_IRQHandler
//        B ADC1_IRQHandler
        
                
        PUBWEAK CEC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
CEC_IRQHandler
        B CEC_IRQHandler

        END
;******************** (C) COPYRIGHT 2018 MindMotion ********************
