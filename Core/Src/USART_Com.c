#include "USART_Com.h"

#define BUFFER_SIZE 64  // Define o tamanho do buffer
#define rx_SIZE 8  // Define o tamanho do buffer
UART_HandleTypeDef huart2;  // struct para uart2
uint8_t rxData[rx_SIZE] = {0};   //

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
    // Salva o dado recebido no buffer
    //RingBuffer_Write(&rxBufferUart2, rxData);

    // Ecoa o caractere recebido (usa a variável rxData)
    HAL_UART_Transmit_IT(&huart2, &rxData, rx_SIZE);

    // Reinicia a recepção via interrupt
    HAL_UART_Receive_IT(&huart2, &rxData, rx_SIZE);
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
  HAL_UART_Receive_IT(&huart2, &rxData, 1);
}
