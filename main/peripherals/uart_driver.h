#ifndef UART_DRIVER_H_INCLUDED
#define UART_DRIVER_H_INCLUDED

void init_uart();
int uart_sync_write(uint8_t *data, int len);
int uart_async_write(uint8_t *data, int len);
int uart_read_rx_buffer(uint8_t *buffer);
void uart_clean_rx_buffer();

#endif

