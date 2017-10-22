#include "CTS_Layer.h"
#define DELAY 5000      // Timer delay timeout count, 5000*0.1msec = 500 msec
struct Element * keyPressed;            // Pointer to the Element structure
struct Element * prev;                  // For remembering previous keypressed
#ifdef ELEMENT_CHARACTERIZATION_MODE
// Delta Counts returned from the API function for the sensor during characterization
unsigned int wheelCnt[5];               // Becuase the Wheel is composed of five elements
#endif

// Sleep Function
// Configures Timer A to run off ACLK, count in UP mode, places the CPU in LPM3 
// and enables the interrupt vector to jump to ISR upon timeout 
/*void sleep(unsigned int time)
{
    TA0CCR0 = time;
    TA0CTL = TASSEL_1+MC_1+TACLR;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE; 
    __bis_SR_register(LPM3_bits+GIE);
    __no_operation();
}*/

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
                     prev = keyPressed;
                     return 4;
                 }

                 else if(prev ==  &right_element)
                {
                    P1OUT = BIT3 | (~BIT5) &(~BIT0);
                    _delay_cycles(100000);
                    P1OUT = BIT3 | (~BIT4) &(~BIT0);
                    _delay_cycles(100000);
                    prev = keyPressed;
                    return 5;
                }
                 else if(prev == keyPressed)
                  return 0;


            }
            // Down Element
            else if(keyPressed == &down_element)
            {
                //P1OUT |= BIT0;              // Turn on center LED
                if(prev ==  &left_element)
                {
                    P1OUT = BIT6;
                    _delay_cycles(100000);
                    P1OUT = BIT7;
                    _delay_cycles(100000);
                    prev = keyPressed;
                    return 7;

                }
                else if(prev ==  &right_element)
                {
                      P1OUT = BIT3 | (~BIT6)&(~BIT0);
                      _delay_cycles(100000);
                      P1OUT = BIT3 | (~BIT7)&(~BIT0);
                     _delay_cycles(100000);
                     prev = keyPressed;
                     return 2;
                }
                else if(prev == keyPressed)
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

                     prev = keyPressed;
                     return 3;
                }
                else if(prev ==  &up_element)
                {
                      P1OUT = BIT4;
                      _delay_cycles(100000);
                      P1OUT = BIT5;
                     _delay_cycles(100000);
                     prev = keyPressed;
                     return 8;
                }
                else if(prev == keyPressed)
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
                     prev = keyPressed;
                     return 6;
                }
                else  if(prev ==  &up_element)
                {
                      P1OUT = BIT3 | (~BIT4) &(~BIT0);
                      _delay_cycles(100000);
                      P1OUT = BIT3 | (~BIT5)&(~BIT0);
                     _delay_cycles(100000);
                     prev = keyPressed;
                     return 1;
                }
                else if(prev == keyPressed)
                                 return 0;

            }
            // Middle Element
            else if(keyPressed == &middle_element)
            {
                P1OUT = BIT0;              // Turn on center LED
                _delay_cycles(100000);
               // prev = keyPressed;
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
    int a[4],M,i;int b[4] = {1,2,3,4};
    if(key(keyPressed) == 9 )
    {
        for(i = 0;i<4;i++)
        {
            prev = (struct Element *)TI_CAPT_Buttons(&wheel_buttons);
            _delay_cycles(100000);
            keyPressed = (struct Element *)TI_CAPT_Buttons(&wheel_buttons);
            M = key(keyPressed) ;
            if(M== 0)
            {
                i--;
            }
            else if(prev != 0)
            {
              /* if(i == 0)
                   key(keyPressed);
               keyPressed = (struct Element *)TI_CAPT_Buttons(&wheel_buttons);*/
               a[i] =  M;

            }
        }
    for(i = 0;i<4 && a[i] != 0;i++)
        if(a[i] != b[i])
        {
            P1OUT = BIT1;
            while(1);
        }
        else
        {
            P1OUT = BIT2;
            while(1);
        }

    }




    // Put the MSP430 into LPM3 for a certain DELAY period
    //sleep(DELAY);


  }
} // End Main

/******************************************************************************/
// Timer0_A0 Interrupt Service Routine: Disables the timer and exists LPM3  
/******************************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);
  __bic_SR_register_on_exit(LPM3_bits+GIE);
}

#pragma vector=PORT2_VECTOR,             \
  PORT1_VECTOR,                          \
  TIMER0_A1_VECTOR,                      \
  USI_VECTOR,                            \
  NMI_VECTOR,COMPARATORA_VECTOR,         \
  ADC10_VECTOR
__interrupt void ISR_trap(void)
{
  // the following will cause an access violation which results in a PUC reset
  WDTCTL = 0;
}*/
