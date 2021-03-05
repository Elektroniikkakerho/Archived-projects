/** Viimeisin toimiva versio '*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/eeprom.h>

#define		play						0
#define		rest						1
#define		high_score					2
#define		game_over					3
#define		demo						4
#define		score_1_addr				0
#define		score_10_addr				1
#define		light_counter_rest_start	240
#define		light_counter_hs_start		10
#define		light_counter_go_on_start	128
#define		light_counter_go_off_start	24
#define		light_counter_rst_start		8
#define		light_counter_off_start		32
#define		light_counter_play_start	235
#define		light_counter_demo_start	10
#define		button_counter_start		10
#define		game_start_delay			32		// Pelin aloitusviive n * 0,21 s eli n. 1 s
#define		game_over_delay				120		// Pelin loppuviive n * 0,21 s eli n. 4 s
#define		high_score_delay			120		// Uuden enn‰tyksen loppuviive n * 0,21 s eli n. 4 s
#define		demo_delay					240
#define		light_0						4
#define		light_1						8
#define		light_2						16
#define		light_3						32
#define		light_mask					(light_0 + light_1 + light_2 + light_3)
#define		light_port					PORTD
#define		button_0					1
#define		button_1					2
#define		button_2					4
#define		button_3					8
#define		button_mask					(button_0 + button_1 + button_2 + button_3)
#define		button_port					PINB
#define		SER							16
#define		RCK							32
#define		SCK							64
#define		disp_1						1
#define		disp_10						2

#define		TCCR1B_init					1

#define		do_nothing					asm ("nop"::)
#define		lights_off					light_port = light_port | light_mask 
#define		lights_on					light_port = light_port & (~light_mask)
#define		disp_10_on					PORTD = PORTD | 2
#define		disp_1_on					PORTD = PORTD | 1
#define		disp_off					PORTD = PORTD & (uint8_t)(~3)

volatile uint8_t disp_counter, light_counter, delay_counter ;
volatile uint8_t button_counter, light_flag, time_counter;

uint8_t light_counter_start, random_l, random_h;
uint8_t light_buf_1, light_buf_2, light_buf_3, button_index;
uint8_t end_game, mode, disp_data, pressed, light_mode;
uint8_t score_1, score_10, high_score_1, high_score_10;
uint8_t previous, repeat_counter;

uint8_t disp_code [16] = {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04, 0x08, 0x60, 0x61, 0x42, 0x60, 0x37};
uint8_t demo_code [2] = {0x08, 0x20};
uint8_t light_masks [4] = {light_0, light_1, light_2, light_3};

void load_to_disp (uint8_t data) {		// Lataa tavun siirtorekisteriin
	uint8_t i, temp;
	for (i = 7; i < 0xFF; i --) {
		temp = data & 1;
		if (temp == 0)
			PORTB = PORTB & (~SER);
		else
			PORTB = PORTB | SER;
		PORTB = PORTB | SCK;
		PORTB = PORTB & (~SCK);
		data = data >> 1;
	}
	PORTB = PORTB | RCK;
	PORTB = PORTB & (~RCK);
}

SIGNAL (SIG_OVERFLOW0) {		// Timer/Counter0 Overflow Interrupt
	uint8_t disp_data;
	disp_counter ++;
	if (mode == rest) {
		if ((disp_counter & 1) == 0)
			light_counter --;
	} else
		light_counter --;	
	if (light_counter == 0)
		light_flag = 1;
	if (button_counter != 0) button_counter --;
		else button_counter = button_counter_start;	
	if ((disp_counter & 7) == 0)
		if (delay_counter != 0)
			delay_counter --;
	if ((disp_counter & 1) == 0)
		time_counter ++;
	disp_off;
	if (mode == play) {
		if ((disp_counter & 1) == 0) {
			disp_data = disp_code [score_10];
			load_to_disp (disp_data);
			disp_10_on;
		} else {
			disp_data = disp_code [score_1];
			load_to_disp (disp_data);
			disp_1_on;
		}
	} else if (mode == rest) {
		if ((time_counter & 128) == 128) {
			if ((disp_counter & 1) == 0) {
				disp_data = disp_code [high_score_10];
				load_to_disp (disp_data);
				disp_10_on;
			} else {
				disp_data = disp_code [high_score_1];
				load_to_disp (disp_data);
				disp_1_on;
			}
		}
	} else if (mode == high_score) {
		if ((time_counter & 6) != 6) {
			if ((disp_counter & 1) == 0) {
				disp_data = disp_code [score_10];
				load_to_disp (disp_data);
				disp_10_on;
			} else {
				disp_data = disp_code [score_1];
				load_to_disp (disp_data);
				disp_1_on;
			}
		}
	} else if (mode == game_over) {
		if ((time_counter & 16) == 16) {
			if ((disp_counter & 1) == 0) {
				disp_data = disp_code [score_10];
				load_to_disp (disp_data);
				disp_10_on;
			} else {
				disp_data = disp_code [score_1];
				load_to_disp (disp_data);
				disp_1_on;
			}
		}
	} else if (mode == demo) {
		if ((time_counter & 64) == 64) {
			if ((disp_counter & 3) == 1) {
				disp_data = demo_code [1];
				load_to_disp (disp_data);
				disp_1_on;
			} else {
				disp_data = demo_code [0];
				load_to_disp (disp_data);
				disp_10_on;
			}
		} else if ((time_counter & 64) == 0) {
			if ((disp_counter & 3) == 0) {
				disp_data = demo_code [0];
				load_to_disp (disp_data);
				disp_10_on;
			} else  {
				disp_data = demo_code [1];
				load_to_disp (disp_data);
				disp_1_on;
			}
		}
	}
}

/**void reset_high_score () {
	uint8_t i, mask;
	high_score_1 += high_score_10 << 4;
	for (i = 0; i < 3; i ++) {
		mask = 1 << (high_score_1 & 3);
		while ((button_port & button_mask) != mask) do_nothing;
		high_score_1 >> 2;
	}
	high_score_1 = 0;
	high_score_10 = 0;
	eeprom_write_byte ((uint8_t*)score_1_addr, high_score_1);
	eeprom_write_byte ((uint8_t*)score_10_addr, high_score_10);
}**/

/**void check_eeprom_corruption () {			// Korjaa EEPROMilta luetun enn‰tyksen tarvittaessa
	if ((high_score_1 > 9) | (high_score_10 > 15)) {
		high_score_1 = 0;
		high_score_10 = 0;
		eeprom_write_byte ((uint8_t*)score_1_addr, high_score_1);
		eeprom_write_byte ((uint8_t*)score_10_addr, high_score_10);
	}
}**/

void initialize () {
/**	end_game = 0;
	disp_counter = 0;
	time_counter = 0; **/
	high_score_1 = eeprom_read_byte ((uint8_t*)score_1_addr);
	high_score_10 = eeprom_read_byte ((uint8_t*)score_10_addr);
//	check_eeprom_corruption ();

	/** Pisteiden nollaus **/
/**	if ((button_port & button_mask) == button_mask)
		reset_high_score ();**/ 

/**	previous = 0;
	button_index = 0; **/
	pressed = 4;
	random_h = 0x34;
	random_l = 0xDF;
	
	/* Asetukset lepotilaa varten */
	mode = rest;
	light_counter_start = light_counter_rest_start;
	light_counter = light_counter_rst_start;
	light_mode = 0;
/**	light_flag = 0; **/
	
	/* IO-asetukset */
	PORTB = 0x00;		// Siirtorekisterin ohjausnastat nollia ja ei pull-uppeja
	DDRB = 0xF0;
	PORTD = 0x3C;		// Ei pull-uppeja, valot (2-5) pois (1) ja n‰ytˆt (0-1) pois (0)
	DDRD = 0x7F;		// Vain button_out ja k‰ytt‰m‰tˆn 7-bitti nollia, muut ykkˆsi‰
	
	/* Keskeytys- ja ajastinasetukset */
	TCCR0 = 0x03;
	TCCR1B = TCCR1B_init;
	TIMSK = _BV (TOIE0);
	
	sei ();
}

void check_buttons () {
	if (button_counter != 0) return;
	uint8_t buttons;
	buttons = button_port;
	if (pressed == 4) {
		if ((buttons & button_mask) == 0)
			pressed = 5;			
	} else if (pressed == 5) {
		if ((buttons & button_0) == button_0) pressed = 0;
		else if ((buttons & button_1) == button_1) pressed = 1;
		else if ((buttons & button_2) == button_2) pressed = 2;
		else if ((buttons & button_3) == button_3) pressed = 3;
	}
}

void light_off (uint8_t mask) {
	light_port = light_port | mask;
}

void light_on (uint8_t mask) {
	light_port = light_port & (uint8_t)(~mask);
}

uint8_t new_random () {
	uint8_t temp, i, result;
	do {
		for (i = 2; i > 0; i --) {
			temp = 0;
			if ((random_h & 128) == 0) temp ++;
			if ((random_h & 64) == 0) temp ++;
			if ((random_h & 16) == 0) temp ++;
			if ((random_l & 8) == 0) temp ++;
			random_h = random_h << 1;
			if (random_l > 127)
				random_h ++;
			random_l = random_l << 1;
			random_l += (temp & 1);
		}
		result = random_l & 3;
		if (result == previous)
			repeat_counter ++;
	} while ((result == previous) & (repeat_counter >= 3));
	if (repeat_counter >= 3)
		repeat_counter = 0;
	previous = result;
	return (result);
}

uint8_t next_button () {	// Palauttaa seuraavana odotettavan nappula koodin
	uint8_t buf, index, mask;
	if (button_index < 4) {
		buf = light_buf_1;
		index = button_index;
	} else if (button_index < 8) {
		buf = light_buf_2;
		index = button_index - 4;
	} else {
		buf = light_buf_3;
		index = button_index - 8;
	}
	/** return ((uint8_t)((buf & (uint8_t)(3 << (uint8_t)(index << 1))) >> (uint8_t)(index << 1))); **/
	index = (uint8_t)(index << 1);
	mask = (uint8_t)(3 << index);
	buf = buf & mask;
	buf = (uint8_t)(buf >> index);
	return (buf);
}

void start_game () {		// Huolehtii pelin aloittamiseen liittyvist‰ toimenpiteist‰
	cli ();
	mode = play;
	lights_off;
	random_l = TCNT1L;
	random_h = TCNT1H;
	light_counter_start = light_counter_play_start;  	// Ensimm‰inen valo pysyy n. 1,5 s
	light_counter = 0; 		// N‰in peli arpoo ja sytytt‰‰ ensimm‰isen valon heti aluksi
	light_flag = 1;
	button_index = 0xFF;		// Pit‰‰ olla -1, koska new_light () kasvattaa t‰t‰ joka kerta
	score_1 = 0;
	score_10 = 0;
	sei ();
	while ((button_port & button_mask) != 0) do_nothing;
	delay_counter = game_start_delay;			// Odota sekunti, ennen kuin peli l‰htee k‰yntiin
	while (delay_counter != 0) do_nothing;
}

void light_and_delay (uint8_t code, uint8_t time) { // finish_gamen apufunktio koodin lyhent‰miseksi
	light_port = (uint8_t)((light_port & ((uint8_t)(~light_mask))) | (uint8_t)((code) & 0x3C));
	light_counter = time;
	while (light_counter != 0) do_nothing;
}

void finish_game (uint8_t result) {	// Huolehtii pelin loppumiseen liittyvist‰ toimenpiteist‰
	mode = result;
	if (result == game_over) {	// Jos peli loppui ilman uutta enn‰tyst‰
		delay_counter = game_over_delay;
		while (delay_counter != 0) {
			lights_on;
			light_counter = light_counter_go_on_start;
			while (light_counter != 0) do_nothing;
			lights_off;
			light_counter = light_counter_go_off_start;
			while (light_counter != 0) do_nothing;
		}
	} else {	// Jos tuli uusi enn‰tys
		cli ();
		eeprom_write_byte ((uint8_t*)score_1_addr, score_1);
		eeprom_write_byte ((uint8_t*)score_10_addr, score_10);
		high_score_1 = score_1;
		high_score_10 = score_10;
		sei ();
		delay_counter = high_score_delay;
		lights_off;
		while (delay_counter != 0) {
			light_and_delay (light_0, light_counter_hs_start);
			light_off (light_0);
			light_and_delay (light_1, light_counter_hs_start);
			light_off (light_1);
			light_and_delay (light_2, light_counter_hs_start);
			light_off (light_2);
			light_and_delay (light_3, light_counter_hs_start);
			light_off (light_3);
			light_and_delay (light_2, light_counter_hs_start);
			light_off (light_2);
			light_and_delay (light_1, light_counter_hs_start);
			light_off (light_1);
		}
	}
	/* Demo-ruutu */
	if ((score_1 == 0) & (score_10 == 0) & ((button_port & button_mask) == button_mask)) {
		mode = demo;
		delay_counter = demo_delay;
		lights_off;
		while (delay_counter != 0) {
			light_and_delay (light_0, light_counter_demo_start);
			light_and_delay (light_0 + light_1, light_counter_demo_start);
			light_and_delay (light_0 + light_1 + light_2, light_counter_demo_start);
			light_and_delay (light_0 + light_1 + light_2 + light_3, light_counter_demo_start);
			light_and_delay (light_1 + light_2 + light_3, light_counter_demo_start);
			light_and_delay (light_2 + light_3, light_counter_demo_start);
			light_and_delay (light_3, light_counter_demo_start);
			light_and_delay (0, light_counter_demo_start);
			light_and_delay (light_3, light_counter_demo_start);
			light_and_delay (light_2 + light_3, light_counter_demo_start);
			light_and_delay (light_1 + light_2 + light_3, light_counter_demo_start);
			light_and_delay (light_0 + light_1 + light_2 + light_3, light_counter_demo_start);
			light_and_delay (light_0 + light_1 + light_2, light_counter_demo_start);
			light_and_delay (light_0 + light_1, light_counter_demo_start);
			light_and_delay (light_0, light_counter_demo_start);
			light_and_delay (0, light_counter_demo_start);
		}
	}
	/* Asetukset lepotilaa vastaavaan kuntoon */
	mode = rest;
	light_counter_start = light_counter_rest_start;
	light_counter = light_counter_start;
}
	
void new_light () {
	light_flag = 0;
	if (light_mode == 0) {
		light_mode = 1;
		light_counter = light_counter_start;		// Resetoi light_counter
		if (mode == play) {
			if (light_counter_start > 155) // 160
				light_counter_start -= 5;
			else if (light_counter_start > 107) // 115
				light_counter_start -= 3;
			else if (light_counter_start > 47) // 55
				light_counter_start -= 2;
			else
				light_counter_start -= 1;
			button_index ++;
			if (button_index == 12) end_game = 1;
			if (end_game == 1) return;
			light_buf_3 = light_buf_3 << 2;
			light_buf_3 += light_buf_2 >> 6;
			light_buf_2 = light_buf_2 << 2;
			light_buf_2 += light_buf_1 >> 6;
		}
		light_buf_1 = light_buf_1 << 2;	
		light_buf_1 += new_random ();
		/** light_on (light_masks [(light_buf_1 & 3)]);	// Korvaa switchin **/
		switch (light_buf_1 & 3) {
			case 0: light_on (light_0); break;
			case 1: light_on (light_1); break;
			case 2: light_on (light_2); break;
			case 3: light_on (light_3); break;
		}
	} else {
		light_mode = 0;
		lights_off;
		light_counter = light_counter_off_start;
	}	
}

int main () {
	initialize ();
	
	/* Loputon p‰‰silmukka alkaa */
	while (1 == 1) {
	
		/* Nappuloiden tarkastus */
		check_buttons ();

		/* Mahdollisen painalluksen oikeellisuuden tarkastus */
		if ((pressed & 4) == 0) {
			if (mode == play) {
				if ((pressed  == next_button ()) & (button_index != 0xFF)) {	// Oikea nappula painettu
					button_index --;
					/* Sammuta valo, jos se on viimeksi arvottu */
					if (button_index == 0xFF)
						lights_off;
					/* Lis‰‰ pisteit‰ */
					cli ();
					score_1 ++;
					if (score_1 == 16) {
						score_10 ++;
						score_1 = 0;
					}
					sei ();
				} else {		// V‰‰r‰ nappula painettu => Tieto talteen
					end_game = 1;
				}
			} else {	// Jos ei oltu peli-moodissa, niin sitten ollaan levossa
				// Peli aloitettu => Alusta satunnaislukugeneraattori ja laita asetukset kohdalleen
				start_game ();
			}
			pressed = 4;
		}

		/* Uuden valon tarpeellisuuden tarkastus */
		if (light_flag == 1) new_light ();		// Jos on uuden valon aika, niin arvo ja sytyt‰ se

		/* Pelin loppumisen tarkastus */
		if (end_game == 1) {		// Onko peli p‰‰ttynyt?
			end_game = 0;
			if ((score_10 > high_score_10) | ((score_10 == high_score_10) & (score_1 > high_score_1))) {
				finish_game (high_score);
			} else {
				finish_game (game_over);
			}
		}

	}	// P‰‰silmukka (loputon while) loppuu

}		// main loppuu

/********************************************************************************************/

	
