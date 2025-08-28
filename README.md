# STM32F407 - ADC/DMA Data Storage and UART Read-by-Interrupt (with HAL Libraries)

## Overview
Aqui estão as configurações necessárias para o funcionamento do **ADC** com armazenamento via **DMA**. Após o término de leitura do ADCs ocorre o armazenamento em memória via interrupção de hardware. 
Enquanto que a decisão de leitura ocorre via software no loop principal, sendo arbitrária a chamada em determinados períodos. 
Essas implementações são parte de um desenvolvimento de sistema para registro de sinais em servidor em nuvem. 

## Tech-Stack  
- C language
- Analog Digital Conversion
- Assyncronous Serial Communication
- ARMv4
- MX CUBE IDE
- HAL Libraries 

## CUBE IDE
- O repositório contém todo o diretório de utilizado para a execução deste projeto. 
- As configurações foram feitas utilizando os recursos gráficos da MX CUBE da STM.
- Clock ajustado para 168MHz; Cristal Externo da placa de 8MHz; 
- ADC1 Ports PA0 e PA1; USART2 Ports PA2 e PA3; 

## Results : 
![Detecção e Rastreamento](results/ppe_StrongSort.gif)


