#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "nvs.h"

#define LED_1 2
#define PWM_MAX 255 

void start_led()
{
  
  // Configuração do Timer
  ledc_timer_config_t timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 1000,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_config);

  // Configuração do Canal
  ledc_channel_config_t channel_config = {
    .gpio_num = LED_1,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
  };
  ledc_channel_config(&channel_config);

  ledc_fade_func_install(0);

  int32_t valor_lido = 0;
  valor_lido = le_valor_nvs("pwm");
  if(valor_lido == -1){
      valor_lido = PWM_MAX;
  }

  ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, valor_lido, 1000 ,LEDC_FADE_WAIT_DONE);



} 

void set_pwm(double intensity)
{
    double percentage = intensity / 100;
    int pwm = PWM_MAX * percentage; 
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0,pwm , 1000 ,LEDC_FADE_WAIT_DONE);
    grava_valor_nvs("pwm", pwm);
}