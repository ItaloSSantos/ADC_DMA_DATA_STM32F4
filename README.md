# ADC DMA Configuration for STM32F407 using HAL libraries

The DMA in STM32F407 is connected to the APB2 bus, which runs at 84MHz when the system clock is configured to 168MHz.

Since the ADC clock must not exceed 36MHz, the maximum recommended prescaler setting is division by 4:

```c
hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
```

This configuration ensures the ADC operates at 21MHz (84MHz / 4), which is within the specified limits for reliable operation.

Outra configuração relevante é o tempo de amostragem, para sinais lentos deve-se melhorar a precisão mantendo um maior tempo de amostragem: 
sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;   
Como o clock do ADC é de 21MHz, o tempo de amostragem ficará próximo a 7us. 
