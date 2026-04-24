
#define RCC 0x40021000 // RCC Base Address

#define RCC_CR 0x00 // CR Offset
#define RCC_CFGR 0x04 // CFGR Offset
#define RCC_AHBENR 0x14 // AHBENR Offset
#define RCC_APB2ENR 0x18 // APB2ENR Offset

#define GPIOA 0x48000000 // GPIOA Base address
#define GPIOB 0x48000400 // GPIOB Base address

#define GPIO_MODER 0x00 // GPIOx_MODER Offset
#define GPIO_OTYPER 0x04 // GPIOx_OTYPER Offset
#define GPIO_OSPEEDR 0x08 // GPIOx_OSPEEDR Offset
#define GPIO_PUPDR 0x0C // GPIOx_PUPDR Offset
#define GPIO_IDR 0x10 // GPIOx_IDR Offset
#define GPIO_ODR 0x14 // GPIOx_ODR Offset
#define GPIO_BSRR 0x18 // GPIOx_BSRR Offset
#define GPIO_LCKR 0x1C // GPIOx_LCKR Offset
#define GPIO_AFRL 0x20 // GPIOx_AFRL Offset
#define GPIO_AFRH 0x24 // GPIOx_AFRH Offset
#define GPIO_BRR 0x28 // GPIOx_BRR Offset

#define ADC1 0x50000000 // ADC1 Base Address
#define ADC2 0x50000200 // ADC2 Base Address
#define ADC3 0x50000400 // ADC3 Base Address
#define ADC4 0x50000600 // ADC4 Base Address

#define ADC_ISR 0x00 // ADCx_ISR Offset
#define ADC_IER 0x04 // ADCx_IER Offset
#define ADC_CR 0x08 // ADCx_CR Offset
#define ADC_CFGR 0x0C // ADCx_CFGR Offset
#define ADC_SMPR1 0x14 // ADCx_SMPR1 Offset
#define ADC_SMPR2 0x18 // ADCx_SMPR2 Offset
#define ADC_TR1 0x20 // ADCx_TR1 Offset
#define ADC_TR2 0x24 // ADCx_TR2 Offset
#define ADC_TR3 0x28 // ADCx_TR3 Offset
#define ADC_SQR1 0x30 // ADCx_SQR1 Offset
#define ADC_SQR2 0x34 // ADCx_SQR2 Offset
#define ADC_SQR3 0x38 // ADCx_SQR3 Offset
#define ADC_SQR4 0x3C // ADCx_SQR4 Offset
#define ADC_DR 0x40 // ADCx_DR Offset
#define ADC_JSQR 0x4C // ADCx_JSQR Offset
#define ADC_OFR1 0x60 // ADCx_OFR1 Offset
#define ADC_OFR2 0x64 // ADCx_OFR2 Offset
#define ADC_OFR3 0x68 // ADCx_OFR3 Offset
#define ADC_OFR4 0x6C // ADCx_OFR4 Offset
#define ADC_JDR1 0x80 // ADCx_JDR1 Offset
#define ADC_JDR2 0x84 // ADCx_JDR2 Offset
#define ADC_JDR3 0x88 // ADCx_JDR3 Offset
#define ADC_JDR4 0x8C // ADCx_JDR4 Offset
#define ADC_AWD2CR 0xA0 // ADCx_AWD2CR Offset
#define ADC_AWD3CR 0xA4 // ADCx_AWD3CR Offset
#define ADC_DIFSEL 0xB0 // ADCx_DIFSEL Offset
#define ADC_CALFACT 0xB4 // ADCx_CALFACT Offset
#define ADC_CSR 0x00 // ADC Common Status Register
#define ADC_CCR 0x08 // ADC Common Control Register
#define ADC_CDR 0x0C // ADC Common Regular Data Register

#define TIM1 0x40012C00 // TIM1 Base Address

#define TIM_CR1 0x00 // TIMx_CR1 Offset
#define TIM_CR2 0x04 // TIMx_CR2 Offset
#define TIM_DIER 0x0C // TIMx_DIER Offset
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
#define TIM_CCR4 0x40 // TIMx_CCR4 Offset
#define TIM_BDTR 0x44 // TIMx_BDTR Offset
#define TIM_DCR 0x48 // TIMx_DCR Offset
#define TIM_DMAR 0x4C // TIMx_DMAR Offset
#define TIM_OR 0x50 // TIMx_OR Offset
#define TIM_CCMR3 0x54 // TIMx_CCMR3 Offset
#define TIMx_CCR5 0x58 // TIMx_CCR5 Offset
#define TIMx_CCR6 0x5C // TIMx_CCR6 Offset

#define NVIC 0xE000E004

