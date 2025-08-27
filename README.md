# STM32F407 ADC com DMA e Leitura via UART

Este projeto mostra como configurar o ADC do STM32F407 com DMA, usando as bibliotecas HAL, e como ler os valores via UART2.
## Configuração do ADC e DMA
O ADC está conectado ao APB2 BUS, cujo clock é 84 MHz (com o clock do sistema em 168 MHz).
Segundo o datasheet, o clock do ADC não pode ultrapassar 36 MHz, então é usado o prescaler:
```c
hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
```
O tempo de amostragem influencia a precisão. Para sinais lentos, recomenda-se maior tempo de amostragem:
```c
sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
```

O valores lidos pelo ADC são armazenado por DMA na seguinte variável:
```c
uint16_t adcBuffer[2] = {0};
```

##Leitura via UART

A UART2 recebe comandos e responde com os valores do ADC.

Comando válido:
```nginx
LER_ADC
```
Exemplo de resposta:
```makefile
ADC0:3.30V,ADC1:1.63V
```
