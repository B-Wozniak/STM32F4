
#include <stdint.h>
#include "inc/stm32f4xx.h"
#include "gpio.h"
#include "hdr/hdr_rcc.h"

void RCC_config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
}

void PLL_config(uint32_t crystal, uint32_t frequency)
{
	uint32_t PLLM_div, PLLP_div, PLLN_mult, vco_input_freq, vco_output_freq, pll_out_freq;
	uint32_t PLLM_div_best = 0, PLLP_div_best = 0, PLLN_mult_best = 0, pll_out_freq_best = 0;

	for(PLLM_div = 2; PLLM_div <= 63; PLLM_div++)	//PLLM divider [2; 63]
	{
		vco_input_freq = crystal / PLLM_div;
		if((vco_input_freq < 1000000ul) || (vco_input_freq > 2000000ul) )	// edge values from RM
			continue;

		for(PLLN_mult = 50; PLLN_mult <= 432; PLLN_mult++)	//PLLN - VCO multiplication [50; 432]
		{
			vco_output_freq = vco_input_freq * PLLN_mult;
			if((vco_output_freq < 100000000ul) || (vco_output_freq > 432000000ul))	//edge values from RM
				continue;

			for(PLLP_div = 2; PLLP_div <= 8; PLLP_div++)	//PLLP divider [2, 4, 6, 8]
			{
				pll_out_freq = vco_output_freq / PLLP_div;
				if(pll_out_freq > 180000000ul)	//MAX FREQ 180MHz
					continue;

				if(pll_out_freq > frequency)
					continue;

				if(pll_out_freq > pll_out_freq_best)
				{
					PLLM_div_best = PLLM_div;
					PLLN_mult_best = PLLN_mult;
					PLLP_div_best = PLLP_div;
					pll_out_freq_best = pll_out_freq;
				}
			}
		}
	}


}



int main(void)
{

	gpio_pin_cfg(GPIOG, 13, GPIO_OUT_PP_25MHz);

	while (1)
	{

	}
}

