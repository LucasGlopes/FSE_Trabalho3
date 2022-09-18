#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "mqtt.h"

#define BOTAO 0

xQueueHandle filaDeInterrupcao;

static void IRAM_ATTR gpio_isr_handler(void *args)
{
  int pino = (int)args;
  xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

// void trataInterrupcaoBotao(void *params)
// {
//   int pino = 0;

//   while(true)
//   {
//     if(xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY))
//     {
//       // De-bouncing
//       int estado = gpio_get_level(pino);
//       printf("ESTADO: %d", estado);
//       if(estado == 1)
//       {
//         gpio_isr_handler_remove(pino);
//         while(gpio_get_level(pino) == estado)
//         {
//           vTaskDelay(50 / portTICK_PERIOD_MS);
//         }
//         // Habilitar novamente a interrupção
//         vTaskDelay(50 / portTICK_PERIOD_MS);
//         gpio_isr_handler_add(pino, gpio_isr_handler, (void *) pino);
//       }

//     }
//     // char JSONAtributos[200];
//     // if(led){
//     //     sprintf(JSONAtributos, "{\"led\": 0}");
//     //     led = 0; 
//     //   } else {
//     //     sprintf(JSONAtributos, "{\"led\": 1}");
//     //     led = 1; 
//     //   }

//     //   mqtt_envia_mensagem("v1/devices/me/attributes", JSONAtributos);


//   }
// }

void trataInterrupcaoBotao(void *params)
{
  int pino = 0;
  int led = 0; 
  char JSONAtributos[200];

  while(true)
  {
    if(xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY))
    {
      // De-bouncing
      int estado = gpio_get_level(pino);
      if(estado == 1)
      {
        gpio_isr_handler_remove(pino);
        while(gpio_get_level(pino) == estado)
        {
          vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        if(led){
            sprintf(JSONAtributos, "{\"led\": 0}");
            led = 0; 
        } else {
            sprintf(JSONAtributos, "{\"led\": 1}");
            led = 1; 
        }
        mqtt_envia_mensagem("v1/devices/me/attributes", JSONAtributos);

        // Habilitar novamente a interrupção
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_isr_handler_add(pino, gpio_isr_handler, (void *) pino);
      }

    }
  }
}


void init_button()
{
  // Configuração do pino do Botão
  esp_rom_gpio_pad_select_gpio(BOTAO);
  // Configura o pino do Botão como Entrada
  gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
  // Configura o resistor de Pulldown para o botão (por padrão a entrada estará em Zero)
  gpio_pulldown_en(BOTAO);
  // Desabilita o resistor de Pull-up por segurança.
  gpio_pullup_dis(BOTAO);

  // Configura pino para interrupção
  gpio_set_intr_type(BOTAO, GPIO_INTR_POSEDGE);

  filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  xTaskCreate(trataInterrupcaoBotao, "TrataBotao", 2048, NULL, 1, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BOTAO, gpio_isr_handler, (void *) BOTAO);

}