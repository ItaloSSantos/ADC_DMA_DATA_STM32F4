# ADC DMA Configuration for STM32F407 using HAL libraries

O DMA no STM32F407 é conectado ao APB2 BUS, cujo clock é de 84MHz quando a configuração de clock do sistema de 168MHz.

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
# Resultados 

## ADC-DMA 
Por meio das configurações das bibliotecas HAL, o ADC realizará a leitura e o DMA fará o salvamento na variável: 
```c
uint16_t adcBuffer[2] = {0}; 
``` 
que foi criada no main.c

## UART para leitura

Através da biblioteca USART_Com ocorre o tratamento das callback advindas das funções HAL, que por sua vez 
viabilizam o tratamento das interrupções da USART2. Sendo necessário somente implementar a lógica para ler o ADC
e retornar o valor do adcBuffer.
```c
uint16_t adcBuffer[2] = {0}; 
``` 
