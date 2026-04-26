#include <common.h>

#define RCC 0x40021000UL // RCC Base Address

#define RCC_CR 0x00 // CR Offset
#define RCC_CFGR 0x04 // CFGR Offset
#define RCC_AHBENR 0x14 // AHBENR Offset
#define RCC_APB2ENR 0x18 // APB2ENR Offset

#define GPIOA 0x48000000UL // GPIOA Base address
#define GPIOB 0x48000400UL // GPIOB Base address

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

#define ADC1 0x50000000UL // ADC1 Base Address
#define ADC2 0x50000200UL // ADC2 Base Address
#define ADC3 0x50000400UL // ADC3 Base Address
#define ADC4 0x50000600UL // ADC4 Base Address

#define ADC1_2_IRQn 18 // IRQn for ADC1_2
#define ADC4_IRQn 61 // IRQn for ADC4

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

#define TIM1 0x40012C00UL // TIM1 Base Address

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

#define NVIC 0xE000E100UL

// Interrupt Set-Enable Register Offsets
#define NVIC_ISER0 0x000
#define NVIC_ISER1 0x004
#define NVIC_ISER2 0x008
#define NVIC_ISER3 0x00C
#define NVIC_ISER4 0x010
#define NVIC_ISER5 0x014
#define NVIC_ISER6 0x018
#define NVIC_ISER7 0x01C
// Interrupt Clear-Enable Register Offsets
#define NVIC_ICER0 0x080
#define NVIC_ICER1 0x084
#define NVIC_ICER2 0x088
#define NVIC_ICER3 0x08C
#define NVIC_ICER4 0x090
#define NVIC_ICER5 0x094
#define NVIC_ICER6 0x098
#define NVIC_ICER7 0x09C
// Interrupt Set-Pending Register Offsets
#define NVIC_ISPR0 0x100
#define NVIC_ISPR1 0x104
#define NVIC_ISPR2 0x108
#define NVIC_ISPR3 0x10C
#define NVIC_ISPR4 0x110
#define NVIC_ISPR5 0x114
#define NVIC_ISPR6 0x118
#define NVIC_ISPR7 0x11C
// Interrupt Clear-Pending Register Offsets
#define NVIC_ICPR0 0x180
#define NVIC_ICPR1 0x184
#define NVIC_ICPR2 0x188
#define NVIC_ICPR3 0x18C
#define NVIC_ICPR4 0x190
#define NVIC_ICPR5 0x194
#define NVIC_ICPR6 0x198
#define NVIC_ICPR7 0x19C
// Intterupt Active Bit Register Offsets
#define NVIC_IABR0 0x200
#define NVIC_IABR1 0x204
#define NVIC_IABR2 0x208
#define NVIC_IABR3 0x20C
#define NVIC_IABR4 0x210
#define NVIC_IABR5 0x214
#define NVIC_IABR6 0x218
#define NVIC_IABR7 0x21C
// Interrupt Priority Registers
#define NVIC_IPR0  0x400
#define NVIC_IPR1  0x404
#define NVIC_IPR2  0x408
#define NVIC_IPR3  0x40C
#define NVIC_IPR4  0x410
#define NVIC_IPR5  0x414
#define NVIC_IPR6  0x418
#define NVIC_IPR7  0x41C
#define NVIC_IPR8  0x420
#define NVIC_IPR9  0x424
#define NVIC_IPR10 0x428
#define NVIC_IPR11 0x42C
#define NVIC_IPR12 0x430
#define NVIC_IPR13 0x434
#define NVIC_IPR14 0x438
#define NVIC_IPR15 0x43C
#define NVIC_IPR16 0x440
#define NVIC_IPR17 0x444
#define NVIC_IPR18 0x448
#define NVIC_IPR19 0x44C
#define NVIC_IPR20 0x450
#define NVIC_IPR21 0x454
#define NVIC_IPR22 0x458
#define NVIC_IPR23 0x45C
#define NVIC_IPR24 0x460
#define NVIC_IPR25 0x464
#define NVIC_IPR26 0x468
#define NVIC_IPR27 0x46C
#define NVIC_IPR28 0x470
#define NVIC_IPR29 0x474
#define NVIC_IPR30 0x478
#define NVIC_IPR31 0x47C
#define NVIC_IPR32 0x480
#define NVIC_IPR33 0x484
#define NVIC_IPR34 0x488
#define NVIC_IPR35 0x48C
#define NVIC_IPR36 0x490
#define NVIC_IPR37 0x494
#define NVIC_IPR38 0x498
#define NVIC_IPR39 0x49C
#define NVIC_IPR40 0x4A0
#define NVIC_IPR41 0x4A4
#define NVIC_IPR42 0x4A8
#define NVIC_IPR43 0x4AC
#define NVIC_IPR44 0x4B0
#define NVIC_IPR45 0x4B4
#define NVIC_IPR46 0x4B8
#define NVIC_IPR47 0x4BC
#define NVIC_IPR48 0x4C0
#define NVIC_IPR49 0x4C4
#define NVIC_IPR50 0x4C8
#define NVIC_IPR51 0x4CC
#define NVIC_IPR52 0x4D0
#define NVIC_IPR53 0x4D4
#define NVIC_IPR54 0x4D8
#define NVIC_IPR55 0x4DC
#define NVIC_IPR56 0x4E0
#define NVIC_IPR57 0x4E4
#define NVIC_IPR58 0x4E8
#define NVIC_IPR59 0x4EC
#define NVIC_STIR  0xE00 // Software Trigget Interrupt Register Offset

#define FPU_CPACR 0xE0000ED88
#define FPU_FPCCR 0xE000EF34
#define FPU_FPCAR 0xE000EF38
#define FPU_FPDSCR 0xE000EF3C

extern "C" void NVIC_EnableIRQ(int IRQn) {
    *((volatile uint32_t*) (NVIC + ((IRQn >> 5) * 4))) = (1 << (IRQn & 0x1F));
}

extern "C" void NVIC_SetPriority(int IRQn, uint8_t priority) {
    volatile uint8_t* ipr = (volatile uint8_t*) 0xE000E400;
    ipr[IRQn] = (uint8_t)((priority << 4) & 0xF0);
}