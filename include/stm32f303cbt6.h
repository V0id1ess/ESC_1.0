
#define RCC 0x40021000 // RCC Base Address

#define RCC_CR 0x00 // CR Offset
#define RCC_CFGR 0x04 // CFGR Offset
#define RCC_AHBENR 0x14 // AHBENR Offset
#define RCC_APB2ENR 0x18 // APB2ENR Offset

#define GPIOA 0x48000000 // GPIOA Base address
#define GPIOB 0x48000400 // GPIOB Base address

#define GPIO_MODER 0x00 // GPIOx_MODER Offset
#define GPIO_OSPEEDR 0x08 // GPIOx_OSPEEDR Offset
#define GPIO_IDR 0x10 // GPIOx_IDR Offset
#define GPIO_ODR 0x14 // GPIOx_ODR Offset
#define GPIO_BSRR 0x18 // GPIOx_BSRR Offset
#define GPIO_AFRL 0x20 // GPIOx_AFRL Offset
#define GPIO_AFRH 0x24 // GPIOx_AFRH Offset
#define GPIO_BRR 0x28 // GPIOx_BRR Offset

#define TIM1 0x40012C00 // TIM1 Base Address
#define TIM_CR1 0x00 // TIMx_CR1 Offset
#define TIM_CR2 0x04 // TIMx_CR2 Offset
#define TIM_SR 0x10 // TIMx_SR Offset
#define TIM_EGR 0x14 // TIMx_EGR Offset
#define TIM_CCMR1 0x18 // TIMx_CCMR1 Offset
#define TIM_CCMR2 0x1C // TIMx_CCMR2 Offset
#define TIM_CCER 0x20 // TIMx_CCER Offset
#define TIM_CNT 0x24 // TIMx_CNT Offset
#define TIM_PSC 0x28 // TIMx_PSC Offset
#define TIM_ARR 0x2C // TIMx_ARR Offset
#define TIM_RCR 0x30 // TIMx_RCR Offset
#define TIM_CCR1 0x34 // TIMx_CCR1 Offset
#define TIM_CCR2 0x38 // TIMx_CCR2 Offset
#define TIM_CCR3 0x3C // TIMx_CCR3 Offset