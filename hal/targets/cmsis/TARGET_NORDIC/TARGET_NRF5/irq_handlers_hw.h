/**
 * @file irq_handlers_hw.h
 * @brief File for hardwiring irq handlers to theirs vectors
 */
 
 #ifndef IRQ_HANDLERS_HW_H__
 #define IRQ_HANDLERS_HW_H__
 
 //#define HARDWIRE_SPI_TWI_INTERRUPT
 
#ifdef NRF51
    #ifdef HARDWIRE_UART_INTERRUPT
        #define UART0_IRQHandler UART0_IRQHandler_v // nRF51
    #endif

    #ifdef HARDWIRE_SPI_TWI_INTERRUPT
        #define SPI0_TWI0_IRQHandler SPI0_TWI0_IRQHandler_v // nRF5x
        #define SPI1_TWI2_IRQHandler SPI1_TWI1_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_GPIOTE_INTERRUPT
        #define GPIOTE_IRQHandler GPIOTE_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_ADC_INTERRUPT
        #define ADC_IRQHandler   ADC_IRQHandler_v // nRF51
    #endif

    #ifdef HARDWIRE_TIMER_INTERRUPT
        #define TIMER1_IRQHandler TIMER1_IRQHandler_v // nRF5x
        #define TIMER2_IRQHandler TIMER2_IRQHandler_v // nRF5x    
    #endif

    #ifdef HARDWIRE_WDT_INTERRUPT
        #define WDT_IRQHandler WDT_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_RTC_INTERRUPT
        #define RTC1_IRQHandler RTC1_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_QDEC_INTERRUPT
        #define QDEC_IRQHandler QDEC_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_COMP_INTERRUPT
        #define LPCOMP_IRQHandler LPCOMP_IRQHandler_v // nRF51
    #endif

    #ifdef HARDWIRE_SWI_INTERRUPT
        #defien SWI0_IRQHandler SWI0_IRQHandler_v // nRF51
        #defien SWI3_IRQHandler SWI3_IRQHandler_v // nRF51
    #endif

#endif // NRF51

#ifdef NRF52
    #ifdef HARDWIRE_UART_INTERRUPT
        #define UART0_IRQHandler UARTE0_UART0_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_SPI_TWI_INTERRUPT
        #define SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler_v // nRF52
        #define SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler_v // nRF52
        #define SPIM2_SPIS2_SPI2_IRQHandler                  SPIM2_SPIS2_SPI2_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_NFCT_INTERRUPT
        #define NFCT_IRQHandler_v NFCT_IRQHandler // nRF52
    #endif

    #ifdef HARDWIRE_GPIOTE_INTERRUPT
        #define GPIOTE_IRQHandler GPIOTE_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_ADC_INTERRUPT
        #define SAADC_IRQHandler SAADC_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_TIMER_INTERRUPT
        #define TIMER0_IRQHandler TIMER0_IRQHandler_v // nRF52
        #define TIMER1_IRQHandler TIMER1_IRQHandler_v // nRF5x
        #define TIMER2_IRQHandler TIMER2_IRQHandler_v // nRF5x
        #define TIMER3_IRQHandler TIMER3_IRQHandler_v // nRF52
        #define TIMER4_IRQHandler TIMER4_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_WDT_INTERRUPT
        #define WDT_IRQHandler WDT_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_RTC_INTERRUPT
        #define RTC1_IRQHandler RTC1_IRQHandler_v // nRF5x
        #define RTC2_IRQHandler RTC2_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_QDEC_INTERRUPT
        #define QDEC_IRQHandler QDEC_IRQHandler_v // nRF5x
    #endif

    #ifdef HARDWIRE_COMP_INTERRUPT
        #define COMP_LPCOMP_IRQHandler COMP_LPCOMP_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_SWI_INTERRUPT
        #define SWI0_EGU0_IRQHandler SWI0_EGU0_IRQHandler_v // nRF52
        #define SWI3_EGU3_IRQHandler SWI3_EGU3_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_TEMP_INTERRUPT
        #define TEMP_IRQHandler TEMP_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_PWM_INTERRUPT
        #define PWM0_IRQHandler  PWM0_IRQHandler_v // nRF52
        #define PWM1_IRQHandler  PWM1_IRQHandler_v // nRF52
        #define PWM2_IRQHandler  PWM2_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_PDM_INTERRUPT
        #define PDM_IRQHandler PDM_IRQHandler_v // nRF52
    #endif

    #ifdef HARDWIRE_I2S_INTERRUPT
        #define I2S_IRQHandler  I2S_IRQHandler_v // nRF52
    #endif

#endif // NRF52

typedef struct
{
    IRQn_Type      IRQn;
    uint32_t       vector;
} peripheral_hanlder_desc_t;

#endif // IRQ_HANDLERS_HW_H__

