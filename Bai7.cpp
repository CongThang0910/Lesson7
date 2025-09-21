#include "stm32f10x.h"
#include "stdio.h"

void UART1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate            = 9600;
    USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits            = USART_StopBits_1;
    USART_InitStruct.USART_Parity              = USART_Parity_No;
    USART_InitStruct.USART_Mode                = USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);
}

void UART1_SendChar(char c) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

void UART1_SendString(char *s) {
    while(*s) {
        UART1_SendChar(*s++);
    }
}

void SPI1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_SetBits(GPIOA, GPIO_Pin_4);

    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI1, &SPI_InitStruct);

    SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_Transfer(uint8_t data) {
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

void MAX7219_Send(uint8_t address, uint8_t data) {
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    SPI1_Transfer(address);
	SPI1_Transfer(data);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void MAX7219_Init(void) {
    MAX7219_Send(0x09, 0x00);
    MAX7219_Send(0x0A, 0x03);
    MAX7219_Send(0x0B, 0x07);
    MAX7219_Send(0x0C, 0x01);
    MAX7219_Send(0x0F, 0x00);
    for(int i=1; i<=8; i++) MAX7219_Send(i, 0x00);
}

void delay_ms(volatile uint32_t t) {
    for(; t>0; t--) {
        for(volatile int i=0; i<8000; i++);
    }
}

int main(void) {
    UART1_Init();
    SPI1_Init();
    MAX7219_Init();
    UART1_SendString("SPI + MAX7219 Blink with UART\r\n");

    while(1) {
        for(int i=1; i<=8; i++) MAX7219_Send(i, 0xFF);
        UART1_SendString("LED ON\r\n");
        delay_ms(500);

        for(int i=1; i<=8; i++) MAX7219_Send(i, 0x00);
        UART1_SendString("LED OFF\r\n");
        delay_ms(500);
    }
}