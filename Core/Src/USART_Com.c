#include "USART_Com.h"
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 64  // Define o tamanho do buffer
#define RX_SIZE 32  // Define o tamanho do buffer


char rxData[RX_SIZE] = {0};  //variável para conversa pelo hyperterminal
const char comando_adc[] = "LER_ADC"; // para que compare e retorne o valor do ADC


UART_HandleTypeDef huart2;  //struct de config da UART2

// Declara a estrutura do RingBuffer
typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} RingBuffer;

void RingBuffer_Write(RingBuffer *rb, uint8_t data) {
    uint16_t nextHead = (rb->head + 1) % BUFFER_SIZE;
    // Se avançar o head vai alcançar a tail, força a tail avançar também (sobrescreve)
    if (nextHead == rb->tail) {
        rb->tail = (rb->tail + 1) % BUFFER_SIZE; // Descarta o dado mais antigo
    }
    rb->buffer[rb->head] = data;
    rb->head = nextHead;
}

uint8_t RingBuffer_Read(RingBuffer *rb, uint8_t *data) {
    if (rb->head == rb->tail) {
        // buffer vazio
        return 0;
    }

    *data = rb->buffer[rb->tail];   // lê dado na posição atual do tail
    rb->tail = (rb->tail + 1) % BUFFER_SIZE; // avança o tail
    return 1;
}

/*-----------------Utilização da USART----------------------*/
// cria a instância de RingBuffer e já declara as posições iniciais.
RingBuffer rxBufferUart2 = { .head = 0, .tail = 0 };

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    // Verifica se recebeu o comando
	  if(strcmp(rxData, comando_adc) == 0) {
	      uint32_t mV0 = ((uint32_t)adcBuffer[0] * 3300) / 4095;
	      uint32_t mV1 = ((uint32_t)adcBuffer[1] * 3300) / 4095;

	      char response[32];
	      snprintf(response, sizeof(response), "ADC0:%lu.%02luV,ADC1:%lu.%02luV\r\n",
	               mV0 / 1000, (mV0 % 1000) / 10,
	               mV1 / 1000, (mV1 % 1000) / 10);
	      HAL_UART_Transmit_IT(&huart2, (uint8_t*)response, strlen(response));
	  }
    else {
        // retorna erro
        const char *error = "Comando invalido\r\n";
        HAL_UART_Transmit_IT(&huart2, (uint8_t*)error, strlen(error));
    }

    // Limpa buffer e reinicia recepção
    memset(rxData, 0, RX_SIZE);
    HAL_UART_Receive_IT(&huart2, (uint8_t*)rxData, RX_SIZE - 1); // -1 para segurança
  }
}
/*-----------------Configuração da USART----------------------*/

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}


void Liga_Usart2(void)
{
  MX_USART2_UART_Init();
  // Recebe para a variável temporária rxData
  HAL_UART_Receive_IT(&huart2, (uint8_t*)rxData, RX_SIZE);
}
