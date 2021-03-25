#ifndef UART_DRIVER_H_INCLUDED
#define UART_DRIVER_H_INCLUDED

void init_uart();
int uart_blocking_write(uint8_t *data, int len);
int uart_async_write(uint8_t *data, int len);

#endif

