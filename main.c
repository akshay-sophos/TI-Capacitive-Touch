#include "CTS_Layer.h"
#define DELAY 5000      // Timer delay timeout count, 5000*0.1msec = 500 msec
struct Element * keyPressed;            // Pointer to the Element structure
struct Element * prev;                  // For remembering previous keypressed
#ifdef ELEMENT_CHARACTERIZATION_MODE
// Delta Counts returned from the API function for the sensor during characterization
unsigned int wheelCnt[5];               // Becuase the Wheel is composed of five elements
#endif

int key(struct Element * keyPressed)
{
    // If a button has been touched, then take some action
        if(keyPressed)
        {
            // Up Element
            if(keyPressed == &up_element)
            {
                //P1OUT |= BIT0;              // Turn on center LED
                 if(prev ==  &left_element)
                 {
                     P1OUT = BIT5;
                     _delay_cycles(100000);
                     P1OUT = BIT4;
                     _delay_cycles(100000);
                     return 4;
                 }
                  if(prev ==  &right_element)
                {
                    P1OUT = BIT3 | (~BIT5) &(~BIT0);
                    _delay_cycles(100000);
                    P1OUT = BIT3 | (~BIT4) &(~BIT0);
                    _delay_cycles(100000);
                    return 5;
                }


                prev = keyPressed;
                return 0;
            }
            // Down Element
            else if(keyPressed == &down_element)
            {

                if(prev ==  &left_element)
                {
                    P1OUT = BIT6;
                    _delay_cycles(100000);
                    P1OUT = BIT7;
                    _delay_cycles(100000);
                    return 7;
                }
                if(prev ==  &right_element)
                {
                      P1OUT = BIT3 | (~BIT6)&(~BIT0);
                      _delay_cycles(100000);
                      P1OUT = BIT3 | (~BIT7)&(~BIT0);
                     _delay_cycles(100000);
                     return 2;
                }



                prev = keyPressed;
                return 0;
            }
            // Left Element
            else if(keyPressed == &left_element)
            {
                //P1OUT |= BIT0;              // Turn on center LED
                if(prev ==  &down_element)
                {
                      P1OUT = BIT7;
                      _delay_cycles(100000);
                      P1OUT = BIT6;
                     _delay_cycles(100000);
                     return 3;
                }
                if(prev ==  &up_element)
                {
                      P1OUT = BIT4;
                      _delay_cycles(100000);
                      P1OUT = BIT5;
                     _delay_cycles(100000);
                     return 8;
                }


                prev = keyPressed;
                return 0;
            }
            // Right Element
            else if(keyPressed == &right_element)
            {
               // P1OUT |= BIT0;              // Turn on center LED
                if(prev ==  &down_element)
                {
                      P1OUT = (BIT3 | (~BIT7)) & (~BIT0);
                      _delay_cycles(100000);
                      P1OUT = BIT3 | (~BIT6)&(~BIT0);
                     _delay_cycles(100000);
                     return 6;
                }
                if(prev ==  &up_element)
                {
                      P1OUT = BIT3 | (~BIT4) &(~BIT0);
                      _delay_cycles(100000);
                      P1OUT = BIT3 | (~BIT5)&(~BIT0);
                     _delay_cycles(100000);
                     return 1;
                }


                prev = keyPressed;

                 return 0;
            }
            // Middle Element
            else if(keyPressed == &middle_element)
            {
                P1OUT = BIT0;              // Turn on center LED
                _delay_cycles(100000);
                prev = keyPressed;
                return 9;
            }
              }
          else
          {
              P1OUT = 0;
              prev = 0;
              return 0;
          }
}

// Main Function
void main(void)
{ 
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  BCSCTL1 = CALBC1_1MHZ;                // Set DCO to 1, 8, 12 or 16MHz
  DCOCTL = CALDCO_1MHZ;
  BCSCTL1 |= DIVA_0;                    // ACLK/1 [ACLK/(0:1,1:2,2:4,3:8)]
  BCSCTL2 |= DIVS_3;                    // SMCLK/8 [SMCLK/(0:1,1:2,2:4,3:8)]
  BCSCTL3 |= LFXT1S_2;                  // LFXT1 = VLO Clock Source
  
  P1OUT = 0x00;                         // Drive all Port 1 pins low
  P1DIR = 0xFF;                         // Configure all Port 1 pins outputs
  
  P2SEL &= ~(BIT6 + BIT7);              // Configure XIN (P2.6) and XOUT (P2.7) to GPIO
  P2OUT = 0x00;                         // Drive all Port 2 pins low
  P2DIR = 0xFF;                         // Configure all Port 2 pins outputs

  // Initialize Baseline measurement
  TI_CAPT_Init_Baseline(&wheel_buttons);
  // Update baseline measurement (Average 5 measurements)
  TI_CAPT_Update_Baseline(&wheel_buttons,5);  

  // Main loop starts here
  while (1)
  {
    keyPressed = (struct Element *)TI_CAPT_Buttons(&wheel_buttons);
    int a[4],i;int  m, b[4] = {1,3,1,2};
    if(key(keyPressed) == 9 )
    {
        for(i = 0;i<4;i++)
        {
            prev = (struct Element *)TI_CAPT_Buttons(&wheel_buttons);

            keyPressed = (struct Element *)TI_CAPT_Buttons(&wheel_buttons);
            m=key(keyPressed);
            P1OUT = 0;

            if(m == 0)
            {
                i--;
            }
            else if(prev != 0)
            {
               a[i] =  m;
            }
        }
    for(i = 0;i<4;i++)
        if(a[i] != b[i])
        {
            P1OUT = BIT2;
            while(1);
        }
        else
        {
            P1OUT = BIT1;
            while(1);
        }

    }

  }
} // End Main


