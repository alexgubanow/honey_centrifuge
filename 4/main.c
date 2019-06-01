#include "main.h"
#include "stm8s.h"
#include "setjmp.h"

__IO uint8_t buttonstate = 0;
   uint8_t result_timeL;
   uint8_t result_timeH;
   uint16_t result_time;
   uint8_t result_pwmL;
   uint8_t result_pwmH;
   uint16_t result_pwm;
   uint16_t pwm;
   uint8_t i,j;
   uint16_t a[26] = { 0, 41, 82, 123, 164, 205, 246, 287, 328, 369, 410, 451, 492, 533, 574, 615, 656, 697, 738, 779, 820, 861, 902, 943, 984, 1023};
   uint16_t b[25] = { 10000, 11666, 13332, 14998, 16664, 18330, 19996, 21662, 23328, 24994, 26660, 28326, 29992, 31658, 33324, 34990, 36656, 38322, 39988, 41654, 43320, 44986, 46652, 48318, 50000};
   uint16_t c[25] = { 700, 712, 724, 736, 748, 760, 772, 784, 796, 808, 820, 832, 844, 856, 868, 880, 892, 904, 916, 928, 940, 952, 964, 976, 1001};


void init_button();
void init_adc();
void init_pwm();
void init_timer();
void init_EXTI();


void init_button()
{
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_IT);
}

void init_EXTI()
{
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC,EXTI_SENSITIVITY_FALL_ONLY); 
}


void init_pwm ()
{
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, 1000); 
  TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,0, TIM2_OCPOLARITY_HIGH);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_ARRPreloadConfig(ENABLE);
}

void init_adc()
{
  ADC1->CR1=0x01;
  ADC1->CR2=0x08;
  ADC1->TDRL=0x18;
}


void init_timer()
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 90);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
}


int main(void)
{
	
	init_button();
	init_EXTI();
        init_adc();
	init_pwm();
	
	while(1)
        {
          
            ADC1->CSR=0x04;
            ADC1->CR1=0x01;
            while(ADC1->CSR ==0x04);
            ADC1->CSR =0x00;
            result_pwmL=ADC1->DRL;
            result_pwmH=ADC1->DRH;
            result_pwm=result_pwmH<<8|result_pwmL;
            j=0;
            for (i=0;i<25;i++)
            if (result_pwm>=a[i] && result_pwm<=a[i+1]+1)
            {
              pwm=c[j];
              break;
            }
             else
            {
              j++;
             }
            
            TIM2->CCR1H = (uint8_t)(pwm >> 8);
            TIM2->CCR1L = (uint8_t)(pwm);
        }
}



