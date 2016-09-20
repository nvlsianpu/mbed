
#include "nrf.h"
#include "cmsis_nvic.h"
#include "stdint.h"

uint32_t *  nrf_dispatch_vector[NVIC_NUM_VECTORS];

typedef void (*generic_irq_handler_t)(void);
/**
 * dispatcher of IRQn handler
 * @todo naked
 */
void dispatch_IRQHandler(void)
{
    IPSR_Type ipsr_value;
    ipsr_value.w = __get_IPSR();
    ((generic_irq_handler_t)nrf_dispatch_vector[ipsr_value.b.ISR])();
}
