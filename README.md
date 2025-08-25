# ADC DMA Configuration for STM32F407 using HAL libraries

O DMA no STM32F407 é conectado ao APB2 BUS, que tem o clock de 84MHz para a configuração de clock so sistema de 168MHz.

Como o clock do ADC pelo datasheet não pode ultrapassar 36MHz, o prescaler é selecionado para 4:

```c
hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
```

Outra configuração relevante é o tempo de amostragem. Para sinais lentos deve-se melhorar a precisão mantendo um maior tempo de amostragem: 
```c
sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
```  
Como o clock do ADC é de 21MHz, o tempo de amostragem ficará próximo a 7us. 

A leitura não será automatica e contínua (sem parar). Somente irá ler por start de software:
```c
hadc1.Init.ContinuousConvMode = DISABLE;
```  
O start inicia com a função da biblioteca HAL: 
```c
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, 2);
``` 