#ifndef BUTTON_H
#define BUTTON_H

static void IRAM_ATTR gpio_isr_handler(void *args);
void trataInterrupcaoBotao(void *params);
void init_button();

#endif