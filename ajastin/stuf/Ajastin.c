/********************************************************************************
Project  : Ajastin.c
Date     : 1.9.2004
Author   : JSa
Comments : Ajastinohjelma
********************************************************************************/

#include <avr/io.h>
#include "viive.h"
#include "lcd.h"
#include <avr/signal.h>
#include <avr/sleep.h>
#define  LISAYS         0x02       //Mik‰li Run/Stop-nappia painetaan, tulee & operaation arvoksi muu kuin nolla
#define  SIIRTO         0x04
#define  RESET          0x08
#define  RUN_STOP       0x01
#define  LIGHT_ON       sbi(PORTD, 6)
#define  LIGHT_OFF      cbi(PORTD, 6)

void Nayton_paivitys (void);
void Esittely (void);
void Ajan_lasku (void);
void Nappien_luku (void);
void Nollaa_napit (void);
void Nollaa_numerot (void);
void Soitto (void);

        unsigned char keskeytyslaskuri = 0;
        unsigned char sek = 0;
        unsigned char kymsek = 0;
        unsigned char min = 0;
        unsigned char kymmin = 0;
        unsigned char tunnit = 0;
        unsigned char kymtunnit = 0;
        unsigned char update=1;
        unsigned char last_key;
        unsigned char siirtolaskuri = 0;
        unsigned char liipaisu = 0;
        unsigned char run_stop = 0;
        unsigned char lisays = 0;
        unsigned char siirto = 0;
        unsigned char reset = 0;
        unsigned char tila = 1;
        unsigned char nappivertailu = 0;
        unsigned char sekunti = 0;
        unsigned char lukulaskuri = 0;
        unsigned char i = 0;
        unsigned int light = 0;


SIGNAL(SIG_OUTPUT_COMPARE1A)            //Vertailukeskeytys 10ms v‰lein
          {
                keskeytyslaskuri++;
                lukulaskuri++;


                if (keskeytyslaskuri >= 100)
                 {
                 keskeytyslaskuri = 0;
                 sekunti = 1;
                 }
                if (lukulaskuri == 1)
                 {
                 Nappien_luku();
                 lukulaskuri = 0;
                 }

                 if (light < 500)
                    {
                    	light++;
                    	LIGHT_ON;
                    }
                 else
                         LIGHT_OFF;


                last_key = PIND;

          }

int main (void)
    {
        DDRB = 0xFF;                    // B-portti output
        DDRD = 0x70;                    // D-portti input
        PORTD = 0x3F;                   // D-portin pinnien arvo 1

        TCNT1H = 0x00;                  //Timer/Counter1 l‰htee laskemaan nollasta
        TCNT1L = 0x00;

        TCCR1B = 0x0B;                  //Timer/Counter1 asetetaan vertailutilaan sek‰
                                        //m‰‰r‰t‰‰n kellotaajuuden esijakajan arvoksi 64
                                        //Timer/Counter1 nollataan mik‰li vertailu t‰sm‰‰

        OCR1AH = 0x02;                  //Asetetaan vertailurekisterin arvoksi 624 dec.
        OCR1AL = 0x70;

        TIMSK = 0x40;                   //Sallitaan Compare Match- ylivuotokeskeytys

/* Eli kun kellotaajuus 4 MHZ ensin ajetaan esijakajan 64 l‰pi tulee taajuudeksi 62500.
Kun vertailurekisteriin laitetaan arvo 624, tapahtuu keskeytys joka 625 kellojakso,
eli joka 10 millisekunti*/

        lcd_init(LCD_DISP_ON_CURSOR);
        
        LIGHT_ON;

        Esittely();

        SREG = 0x80;
        while(1)                         //ikuinen silmukka
         {

           Nayton_paivitys();


           switch (tila)
            {
               case 1 :


                  if (lisays == 1)
                   {
                    sek++;                      //Lis‰t‰‰n sek muuttujaa yhdell‰
                    
                    if (sek > 9)                //Mik‰li sek-muuttuja on yli 9, niin nollataan se
                    sek = 0;

                    update = 1;
                   }

                  if(siirto == 1)
                    {
                    tila = 2;
                    update = 1;
                    }

                  if ((sek || kymsek || min || kymmin || tunnit || kymtunnit) != 0)
                  {
                  if(run_stop == 1)
                    {
                    	update = 1;
                        tila = 7;
                    }
                   }
                  if (reset == 1)
                      {
                        tila = 1;
                        Nollaa_numerot();
                      }

                  Nollaa_napit();

                break;

               case 2 :

                  if (lisays == 1)
                   {
                    kymsek++;                   //Lis‰t‰‰n kymsek muuttujaa yhdell‰
                    if (kymsek > 5)             //Mik‰li kymsek-muuttuja on yli 9, niin nollataan se
                    kymsek = 0;
                    
                    update = 1;
                   }
                  if(siirto == 1)
                    {
                        update = 1;
                        tila = 3;
                    }
                  if ((sek || kymsek || min || kymmin || tunnit || kymtunnit) != 0)
                  {
                  if(run_stop == 1)
                    {
                    	update = 1;
                        tila = 7;
                    }
                   }
                  if (reset == 1)
                     {
                     	tila = 1;
                        Nollaa_numerot();

                     }
                  Nollaa_napit();

                break;

                case 3 :
                   if (lisays == 1)
                    {
                     min++;
                     if (min > 9)                //Mik‰li min-muuttuja on yli 9, niin nollataan se
                     min = 0;
                     
                     update = 1;
                    }
                   if (siirto == 1)
                     {
                     tila = 4;
                     update = 1;
                     }
                   if ((sek || kymsek || min || kymmin || tunnit || kymtunnit) != 0)
                  {
                  if(run_stop == 1)
                    {
                    	update = 1;
                        tila = 7;
                    }
                   }
                   if (reset == 1)
                      {
                      	tila = 1;
                        Nollaa_numerot();

                      }
                   Nollaa_napit();

                 break;

                 case 4 :
                    if (lisays == 1)
                     {
                      kymmin++;
                      if (kymmin > 5)
                      kymmin = 0;

                      update = 1;
                     }
                    if (siirto == 1)
                      {
                      tila = 5;
                      update = 1;
                      }
                    if ((sek || kymsek || min || kymmin || tunnit || kymtunnit) != 0)
                  {
                  if(run_stop == 1)
                    {
                    	update = 1;
                        tila = 7;
                    }
                   }
                  if (reset == 1)
                     {
                        tila = 1;
                        Nollaa_numerot();

                     }
                  Nollaa_napit();

                  break;

                  case 5 :
                     if (lisays == 1)
                      {
                       tunnit++;
                       if (tunnit > 9)
                       tunnit = 0;
                       update = 1;
                      }
                     if (siirto == 1)
                       {
                       tila = 6;
                       update = 1;
                       }
                     if ((sek || kymsek || min || kymmin || tunnit || kymtunnit) != 0)
                  {
                  if(run_stop == 1)
                    {
                    	update = 1;
                        tila = 7;
                    }
                   }
                   if (reset == 1)
                       {
                       	tila = 1;
                        Nollaa_numerot();

                       }
                   Nollaa_napit();

                   break;

                   case 6:
                        if (lisays == 1)
                        {
                         kymtunnit++;
                         if (kymtunnit > 9)
                          kymtunnit = 0;

                          update = 1;
                        }
                        if (siirto == 1)
                          {
                           tila = 1;
                           update = 1;
                          }
                        if ((sek || kymsek || min || kymmin || tunnit || kymtunnit) != 0)
                  {
                  if(run_stop == 1)
                    {
                    	update = 1;
                        tila = 7;
                    }
                   }
                    if (reset == 1)
                       {
                        tila = 1;
                        Nollaa_numerot();
                       }
                    Nollaa_napit();

                    break;

                    case 7:

                        Ajan_lasku ();

                        if ((sek || kymsek || min || kymmin || tunnit || kymtunnit) == 0)
                        {
                        update = 1;
                        tila =8;
                        }

                        if (run_stop == 1)
                        {
                        update = 1;
                        tila = 1;
                        }

                        Nollaa_napit();

                        break;

                    case 8:

                       for (i = 0; i < 4 ; i++ )
                       {
                       cbi(PORTD, 4);
                       Delay_ms(300);
                       sbi(PORTD, 4);
                       Delay_ms(300);

                        if ( (run_stop || lisays || siirto || reset) == 1 )
                          i = 5;

                       keskeytyslaskuri = 0;
                       sekunti = 0;
                       Nollaa_napit();
                       }


                        tila = 1;
                        update = 1;
                        Nollaa_napit();

                    break;


         }
                MCUCR = 0X20;
                sleep_mode();
                MCUCR = 0X00;

    }
   }


   void Nayton_paivitys (void)
        {

           if (update == 1)                    //mik‰li n‰ytˆnp‰ivitysbitti on 1 suoritetaan seuraava silmukka
            {

            if (tila < 7)
              {
                lcd_clrscr();                  //N‰ytˆn tyhjennys
                lcd_puts("Ajan asetus:");
                lcd_gotoxy(8,1);
              }

           if (tila == 7)
              {
                lcd_clrscr();                  //N‰ytˆn tyhjennys
                lcd_puts("Aikaa jÑljellÑ:");

              }

             if (tila == 8)
              {
                lcd_clrscr();                  //N‰ytˆn tyhjennys
                lcd_puts("Aika loppui: ");
              }
             
                lcd_gotoxy(8,1);

                lcd_putc(kymtunnit | 0x30);

                lcd_putc(tunnit | 0x30);

                lcd_puts (":");

                lcd_putc(kymmin | 0x30);

                lcd_putc(min | 0x30);

                lcd_puts (":");

                lcd_putc(kymsek | 0x30);

                lcd_putc(sek | 0x30);

                if (tila == 1)
                        lcd_gotoxy(15,1);
                if (tila == 2)
                        lcd_gotoxy(14,1);
                if (tila == 3)
                        lcd_gotoxy(12,1);
                if (tila == 4)
                        lcd_gotoxy(11,1);
                if (tila == 5)
                        lcd_gotoxy(9,1);
                if (tila == 6)
                        lcd_gotoxy(8,1);

                update = 0;                       //muuttaa n‰ytˆnp‰ivitysbitin nollaksi

           }
        }                       //!!!Nayton_paivitys p‰‰ttyy

void Esittely (void)
        {
         lcd_clrscr();
         lcd_puts("Elkerho 04 JS&RH:");
         lcd_gotoxy(5,1);
         lcd_puts("Ajastin");
         lcd_gotoxy(17,1);
         Delay_ms(1500);
        }       //!!!Esittely p‰‰ttyy*

void Ajan_lasku (void)
        {
//Silmukka laskee n‰ytˆll‰ olevia numeroita sekunnin v‰lein ja v‰hent‰‰ seuraavasta numerosta yhden
//mik‰li edellinen menee nollaksi. 1:11:11 -> 1:11:10 -> 1:11:09 jne...

          if (sekunti == 1)
            {
            if (sek != 0)               //Mik‰li sek- muuttuja on erisuuri kuin nolla
             sek--;                     //V‰hennet‰‰n sek- muuttujaa yhdell‰

            else                        //Mik‰li sek- muuttuja on nolla
             {
              sek = 9;                  //Laitetaan sek-muuttujaan arvoksi 9

               if (kymsek != 0)         //Ja katsotaan onko kymsek-muuttuja erisuuri kuin nolla
                 kymsek--;              //Mik‰li n‰in on, v‰hennet‰‰n kymsek-muuttujaa yhdell‰

               else                     //Mik‰li kymsek-muuttuja on nolla
                {
                 kymsek = 5;            //Laitetaan kymsek-muuttujan arvoksi 5 jne...

                  if (min != 0)
                    min--;

                  else
                   {
                    min = 9;

                     if (kymmin != 0)
                      kymmin--;

                     else
                      {
                       kymmin = 5;

                        if (tunnit != 0)
                         tunnit--;

                        else
                         {
                          tunnit = 9;

                           if (kymtunnit != 0)
                            kymtunnit--;

                           else
                            {
                             kymtunnit = 0;
                            }
                          }
                       }
                    }
                 }
               }
          update = 1;
           }
          sekunti = 0;
         }

void Nappien_luku (void)
    {
      nappivertailu = (last_key & ~PIND);
      
      if ((PIND & 0x1F) != 0x1F)
         light = 0;

      if ((nappivertailu & RESET) != 0 )
        reset = 1;

      if ((nappivertailu & RUN_STOP) != 0)
        {
                run_stop = 1;
                keskeytyslaskuri = 0;
                sekunti = 0;
        }
      if ((nappivertailu & LISAYS) != 0)
        lisays = 1;



      if ((nappivertailu & SIIRTO) != 0)
        siirto = 1;


    }

void Nollaa_napit (void)
        {
        	siirto = 0;
        	lisays = 0;
        	run_stop = 0;
        	reset = 0;
        }


void Nollaa_numerot (void)
        {
        	sek = 0;
        	kymsek = 0;
        	min = 0;
        	kymmin = 0;
        	tunnit = 0;
        	kymtunnit = 0;

                update = 1;
        }


