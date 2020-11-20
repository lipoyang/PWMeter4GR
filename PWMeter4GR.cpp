#include "Arduino.h"
#include "PWMeter4GR.h"

// edge detection setting
#define PULSE_RISING        0x08
#define PULSE_FALLING       0x09

// global objects for global interrupt handlers
#ifdef GRCITRUS
static PWMeter* s_meter0C = NULL;
static PWMeter* s_meter0D = NULL;
static PWMeter* s_meter1A = NULL;
static PWMeter* s_meter1B = NULL;
static PWMeter* s_meter3A = NULL;
static PWMeter* s_meter3B = NULL;
static PWMeter* s_meter3C = NULL;
static PWMeter* s_meter3D = NULL;
static PWMeter* s_meter4B = NULL;
#else // GR-ROSE
static PWMeter* s_meter1A = NULL;
static PWMeter* s_meter0B = NULL;
static PWMeter* s_meter4D = NULL;
static PWMeter* s_meter0C = NULL;
static PWMeter* s_meter8D = NULL;
static PWMeter* s_meter8B = NULL;
static PWMeter* s_meter8C = NULL;
static PWMeter* s_meter8A = NULL;
#endif // GR-ROSE

#ifdef GRCITRUS
/*
 * Timer input capture interrupt handlers (global)
 * (!) MTU0...4 and TPU6...10 use same vector.
 *     handler functionn names are INT_Excep_TPU*, but they are for MTUs.
 */

// MTU0 TGIC0 / TPU6 TGI6C
void INT_Excep_TPU6_TGI6C(void)
{
    if(s_meter0C != NULL){
        MTU0.TIORL.BIT.IOC = s_meter0C->isr(MTU0.TGRC);
    }
}

// MTU0 TGID0 / TPU6 TGI6D
void INT_Excep_TPU6_TGI6D(void)
{
    if(s_meter0D != NULL){
        MTU0.TIORL.BIT.IOD = s_meter0D->isr(MTU0.TGRD);
    }
}

// MTU1 TGIA1 / TPU7 TGI7A
void INT_Excep_TPU7_TGI7A(void)
{
    if(s_meter1A != NULL){
        MTU1.TIOR.BIT.IOA = s_meter1A->isr(MTU1.TGRA);
    }
}

// MTU1 TGIB1 / TPU7 TGI7B
void INT_Excep_TPU7_TGI7B(void)
{
    if(s_meter1B != NULL){
        MTU1.TIOR.BIT.IOB = s_meter1B->isr(MTU1.TGRB);
    }
}

// MTU3 TGIA3 / TPU9 TGI9A
void INT_Excep_TPU9_TGI9A(void)
{
    if(s_meter3A != NULL){
        MTU3.TIORH.BIT.IOA = s_meter3A->isr(MTU3.TGRA);
    }
}

// MTU3 TGIB3 / TPU9 TGI9B
void INT_Excep_TPU9_TGI9B(void)
{
    if(s_meter3B != NULL){
        MTU3.TIORH.BIT.IOB = s_meter3B->isr(MTU3.TGRB);
    }
}

// MTU3 TGIC3 / TPU9 TGI9C
void INT_Excep_TPU9_TGI9C(void)
{
    if(s_meter3C != NULL){
        MTU3.TIORL.BIT.IOC = s_meter3C->isr(MTU3.TGRC);
    }
}

// MTU3 TGID3 / TPU9 TGI9D
void INT_Excep_TPU9_TGI9D(void)
{
    if(s_meter3D != NULL){
        MTU3.TIORL.BIT.IOD = s_meter3D->isr(MTU3.TGRD);
    }
}

// MTU4 TGIB4 / TPU10 TGI10B
void INT_Excep_TPU10_TGI10B(void)
{
    if(s_meter4B != NULL){
        MTU4.TIORH.BIT.IOB = s_meter4B->isr(MTU4.TGRB);
    }
}

#else // GR-ROSE

/*
 * Timer input capture interrupt handlers (global)
 * (!) RX65N has <Software Configurable Interrupt>.
 *     MTU interrupts are assigned to Software Configurable Interrupt A.
 */

extern "C" {
void INT_MTU1_TGIA() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA230)), used));
void INT_MTU0_TGIB() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA231)), used));
void INT_MTU4_TGID() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA232)), used));
void INT_MTU0_TGIC() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA233)), used));
void INT_MTU8_TGID() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA234)), used));
void INT_MTU8_TGIB() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA235)), used));
void INT_MTU8_TGIC() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA236)), used));
void INT_MTU8_TGIA() __attribute__((interrupt(".rvectors", VECT(PERIA, INTA237)), used));

void INT_MTU1_TGIA()
{
    if(s_meter1A != NULL){
        MTU1.TIOR.BIT.IOA = s_meter1A->isr(MTU1.TGRA);
    }
}

void INT_MTU0_TGIB()
{
    if(s_meter0B != NULL){
        MTU0.TIORH.BIT.IOB = s_meter0B->isr(MTU0.TGRB);
    }
}

void INT_MTU4_TGID()
{
    if(s_meter4D != NULL){
        MTU4.TIORL.BIT.IOD = s_meter4D->isr(MTU4.TGRD);
    }
}

void INT_MTU0_TGIC()
{
    if(s_meter0C != NULL){
        MTU0.TIORL.BIT.IOC = s_meter0C->isr(MTU0.TGRC);
    }
}

void INT_MTU8_TGID()
{
    if(s_meter8D != NULL){
        MTU8.TIORL.BIT.IOD = s_meter8D->isr(MTU8.TGRD);
    }
}

void INT_MTU8_TGIB()
{
    if(s_meter8B != NULL){
        MTU8.TIORH.BIT.IOB = s_meter8B->isr(MTU8.TGRB);
    }
}

void INT_MTU8_TGIC()
{
    if(s_meter8C != NULL){
        MTU8.TIORL.BIT.IOC = s_meter8C->isr(MTU8.TGRC);
    }
}

void INT_MTU8_TGIA()
{
    if(s_meter8A != NULL){
        MTU8.TIORH.BIT.IOA = s_meter8A->isr(MTU8.TGRA);
    }
}

} // end of "extern "C""

#endif // GR-ROSE

/**
 * constructor
 */
PWMeter::PWMeter()
{
    this->pin = 0xFF; // initialize by invalid value
}

/*
 * Timer input capture interrupt handler
 * cnt: timer count value
 * return: edge detection setting
 */
uint8_t PWMeter::isr(uint16_t cnt)
{
    pushi();    // backup the interrupt status
    cli();      // disable interrupts
    
    // start edge
    if( pulse_edge == start_edge ){
        pulse_edge = end_edge;
        
        cnt1 = cnt;                 // start time of pulse
    }
    // stop edge
    else{
        pulse_edge = start_edge;
        
        cnt2 = cnt;                 // end time of pulse
        buffer[wptr] = cnt2 - cnt1; // pulse width
        uint8_t temp = wptr;
        if(++temp >= PWMETER_BUFF_SIZE) temp = 0;
        if(temp != rptr) wptr = temp;
    }
    
    popi();     // restore the interrupt status
    
    return pulse_edge;
}

/* <for GR-CITRUS>
 *
 * begin the PWMeter
 * pin: pulse input pin (pin 0,1,2,3,4,7,8,10,11,12,13 are supported.)
 *     supported input pins are divided into following groups.
 *     - 0,1 (using MTU1)
 *     - 2,3,4,10,11,12,13 (using MTU3)
 *     - 4 (using MTU4)
 *     - 7,8  (using MTU0)
 *     same resolution setting is applied to same group pins.
 *     just the last setting is effective.
 *     following pins can not be used together due to resource conflict.
 *     - 2 and 11 (using MTIOC3C)
 *     - 3 and 12 (using MTIOC3A)
 * polarity: pulse polarity
 *     PWMETER_POSITIVE:   positive (default)
 *     PWMETER_NEGATIVE:   negative
 * resolution: pulse meter resolution
 *     PWMETER_48TH_USEC:  1/48usec :Max  1365usec
 *     PWMETER_12TH_USEC:  1/12usec :Max  5461usec
 *     PWMETER_3RD_USEC:   1/3usec  :Max 21845usec
 *     PWMETER_1USEC:      1usec    :Max 21845usec (default)
 *     PWMETER_4_3RD_USEC: 4/3usec  :Max 87380usec
 */
/* <for GR-ROSE>
 *
 * begin the PWMeter
 * pin: pulse input pin (pin 1,3,5,7,14(A0),15(A1),16(A2),17(A3),18(A4) are supported.)
 *     supported input pins are divided into following groups.
 *     - 1 (using MTU1)
 *     - 3,7 (using MTU0)
 *     - 5,14 (using MTU4)
 *     - 15,16,17,18 (using MTU8)
 *     same resolution setting is applied to same group pins.
 *     just the last setting is effective.
 *     following pins can not be used together due to resource conflict.
 *     - 5 and 14 (using MTIOC4D)
 * polarity: pulse polarity
 *     PWMETER_POSITIVE:   positive (default)
 *     PWMETER_NEGATIVE:   negative
 * resolution: pulse meter resolution
 *     PWMETER_60TH_USEC:    1/60usec :Max   1092usec
 *     PWMETER_15TH_USEC:    1/15usec :Max   4369usec
 *     PWMETER_4_15TH_USEC:  4/15usec :Max  17476usec
 *     PWMETER_1USEC:        1usec    :Max  17476usec (default)
 *     PWMETER_32_15TH_USEC: 32/15usec:Max 139808usec
 */
void PWMeter::begin(int pin, int polarity, int resolution)
{
    this->pin = pin;
    this->polarity = polarity;
    this->resolution = resolution;
    
    // initialize variables
    if(polarity == PWMETER_POSITIVE){
        pulse_edge = PULSE_RISING;
        start_edge = PULSE_RISING;
        end_edge   = PULSE_FALLING;
    }else{
        pulse_edge = PULSE_FALLING;
        start_edge = PULSE_FALLING;
        end_edge   = PULSE_RISING;
    }
    rptr = 0;
    wptr = 0;
    
    to1usec = (resolution == PWMETER_1USEC);
    
#ifdef GRCITRUS
    uint8_t clksel = (resolution == PWMETER_1USEC) ? PWMETER_3RD_USEC : resolution;
#else // GR-ROSE
    uint8_t clksel = (resolution == PWMETER_1USEC) ? PWMETER_4_15TH_USEC : resolution;
#endif
    
    pushi();    // backup the interrupt status
    cli();      // disable interrupts
    
    // unlock the protect of pin function selector
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;
    // unlock the protect of module stop
    SYSTEM.PRCR.WORD = 0xA502;
    
    switch(pin){
#ifdef GRCITRUS
    case 0: // P20 / MTIOC1A
        MSTP(MTU1) = 0;             // awake from module stop
        MPC.P20PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORT2.PDR.BIT.B0 = 0;       // port direction (input)
        PORT2.PMR.BIT.B0 = 1;       // port mode (peripheral)
        MTU1.TCR.BIT.TPSC = clksel; // clock select
        MTU1.TIOR.BIT.IOA = pulse_edge; // edge select
        MTU.TSTR.BIT.CST1 = 0x01;   // count start
        MTU1.TIER.BIT.TGIEA = 1;    // interrupt enable
        IEN(MTU1, TGIA1) = 1;       // interrupt enable
        IPR(MTU1, TGIA1) = 1;       // interrupt priority
        s_meter1A = this;
        break;
    case 1: // P21 / MTIOC1B
        MSTP(MTU1) = 0;             // awake from module stop
        MPC.P21PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORT2.PDR.BIT.B1 = 0;       // port direction (input)
        PORT2.PMR.BIT.B1 = 1;       // port mode (peripheral)
        MTU1.TCR.BIT.TPSC = clksel; // clock select
        MTU1.TIOR.BIT.IOB = pulse_edge; // edge select
        MTU.TSTR.BIT.CST1 = 0x01;   // count start
        MTU1.TIER.BIT.TGIEB = 1;    // interrupt enable
        IEN(MTU1, TGIB1) = 1;       // interrupt enable
        IPR(MTU1, TGIB1) = 1;       // interrupt priority
        s_meter1B = this;
        break;
    case 2: // PC0 / MTIOC3C
        MSTP(MTU3) = 0;             // awake from module stop
        MPC.PC0PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B0 = 0;       // port direction (input)
        PORTC.PMR.BIT.B0 = 1;       // port mode (peripheral)
        MTU3.TCR.BIT.TPSC = clksel; // clock select
        MTU3.TIORL.BIT.IOC = pulse_edge; // edge select
        MTU.TSTR.BIT.CST3 = 0x01;   // count start
        MTU3.TIER.BIT.TGIEC = 1;    // interrupt enable
        IEN(MTU3, TGIC3) = 1;       // interrupt enable
        IPR(MTU3, TGIC3) = 1;       // interrupt priority
        s_meter3C = this;
        break;
    case 3: // PC1 / MTIOC3A
        MSTP(MTU3) = 0;             // awake from module stop
        MPC.PC1PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B1 = 0;       // port direction (input)
        PORTC.PMR.BIT.B1 = 1;       // port mode (peripheral)
        MTU3.TCR.BIT.TPSC = clksel; // clock select
        MTU3.TIORH.BIT.IOA = pulse_edge; // edge select
        MTU.TSTR.BIT.CST3 = 0x01;   // count start
        MTU3.TIER.BIT.TGIEA = 1;    // interrupt enable
        IEN(MTU3, TGIA3) = 1;       // interrupt enable
        IPR(MTU3, TGIA3) = 1;       // interrupt priority
        s_meter3A = this;
        break;
    case 4: // PC2 / MTIOC4B
        MSTP(MTU4) = 0;             // awake from module stop
        MPC.PC2PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B2 = 0;       // port direction (input)
        PORTC.PMR.BIT.B2 = 1;       // port mode (peripheral)
        MTU4.TCR.BIT.TPSC = clksel; // clock select
        MTU4.TIORH.BIT.IOB = pulse_edge; // edge select
        MTU.TSTR.BIT.CST4 = 0x01;   // count start
        MTU4.TIER.BIT.TGIEB = 1;    // interrupt enable
        IEN(MTU4, TGIB4) = 1;       // interrupt enable
        IPR(MTU4, TGIB4) = 1;       // interrupt priority
        s_meter4B = this;
        break;
    case 7: // P32 / MTIOC0C
        MSTP(MTU0) = 0;             // awake from module stop
        MPC.P32PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORT3.PDR.BIT.B2 = 0;       // port direction (input)
        PORT3.PMR.BIT.B2 = 1;       // port mode (peripheral)
        MTU0.TCR.BIT.TPSC = clksel; // clock select
        MTU0.TIORL.BIT.IOC = pulse_edge; // edge select
        MTU.TSTR.BIT.CST0 = 0x01;   // count start
        MTU0.TIER.BIT.TGIEC = 1;    // interrupt enable
        IEN(MTU0, TGIC0) = 1;       // interrupt enable
        IPR(MTU0, TGIC0) = 1;       // interrupt priority
        s_meter0C = this;
        break;
    case 8: // P33 / MTIOC0D
        MSTP(MTU0) = 0;             // awake from module stop
        MPC.P33PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORT3.PDR.BIT.B3 = 0;       // port direction (input)
        PORT3.PMR.BIT.B3 = 1;       // port mode (peripheral)
        MTU0.TCR.BIT.TPSC = clksel; // clock select
        MTU0.TIORL.BIT.IOD = pulse_edge; // edge select
        MTU.TSTR.BIT.CST0 = 0x01;   // count start
        MTU0.TIER.BIT.TGIED = 1;    // interrupt enable
        IEN(MTU0, TGID0) = 1;       // interrupt enable
        IPR(MTU0, TGID0) = 1;       // interrupt priority
        s_meter0D = this;
        break;
    case 10: // PC4 / MTIOC3D
        MSTP(MTU3) = 0;             // awake from module stop
        MPC.PC4PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B4 = 0;       // port direction (input)
        PORTC.PMR.BIT.B4 = 1;       // port mode (peripheral)
        MTU3.TCR.BIT.TPSC = clksel; // clock select
        MTU3.TIORL.BIT.IOD = pulse_edge; // edge select
        MTU.TSTR.BIT.CST3 = 0x01;   // count start
        MTU3.TIER.BIT.TGIED = 1;    // interrupt enable
        IEN(MTU3, TGID3) = 1;       // interrupt enable
        IPR(MTU3, TGID3) = 1;       // interrupt priority
        s_meter3D = this;
        break;
    case 11: // PC6 / MTIOC3C
        MSTP(MTU3) = 0;             // awake from module stop
        MPC.PC6PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B6 = 0;       // port direction (input)
        PORTC.PMR.BIT.B6 = 1;       // port mode (peripheral)
        MTU3.TCR.BIT.TPSC = clksel; // clock select
        MTU3.TIORL.BIT.IOC = pulse_edge; // edge select
        MTU.TSTR.BIT.CST3 = 0x01;   // count start
        MTU3.TIER.BIT.TGIEC = 1;    // interrupt enable
        IEN(MTU3, TGIC3) = 1;       // interrupt enable
        IPR(MTU3, TGIC3) = 1;       // interrupt priority
        s_meter3C = this;
        break;
    case 12: // PC7 / MTIOC3A
        MSTP(MTU3) = 0;             // awake from module stop
        MPC.PC7PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B7 = 0;       // port direction (input)
        PORTC.PMR.BIT.B7 = 1;       // port mode (peripheral)
        MTU3.TCR.BIT.TPSC = clksel; // clock select
        MTU3.TIORH.BIT.IOA = pulse_edge; // edge select
        MTU.TSTR.BIT.CST3 = 0x01;   // count start
        MTU3.TIER.BIT.TGIEA = 1;    // interrupt enable
        IEN(MTU3, TGIA3) = 1;       // interrupt enable
        IPR(MTU3, TGIA3) = 1;       // interrupt priority
        s_meter3A = this;
        break;
    case 13: // PC5 / MTIOC3B
        MSTP(MTU3) = 0;             // awake from module stop
        MPC.PC5PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B5 = 0;       // port direction (input)
        PORTC.PMR.BIT.B5 = 1;       // port mode (peripheral)
        MTU3.TCR.BIT.TPSC = clksel; // clock select
        MTU3.TIORH.BIT.IOB = pulse_edge; // edge select
        MTU.TSTR.BIT.CST3 = 0x01;   // count start
        MTU3.TIER.BIT.TGIEB = 1;    // interrupt enable
        IEN(MTU3, TGIB3) = 1;       // interrupt enable
        IPR(MTU3, TGIB3) = 1;       // interrupt priority
        s_meter3B = this;
        break;
#else // GR-ROSE
    case 1: // P20 / MTIOC1A
        MSTP(MTU1) = 0;             // awake from module stop
        MPC.P20PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORT2.PDR.BIT.B0 = 0;       // port direction (input)
        PORT2.PMR.BIT.B0 = 1;       // port mode (peripheral)
        MTU1.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU1.TIOR.BIT.IOA = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST1 = 0x01;  // count start
        MTU1.TIER.BIT.TGIEA = 1;    // interrupt enable
        ICU.SLIAR230.BYTE   = 8;    // interrupt factor MTU1 TGIA1
        IEN(PERIA, INTA230) = 1;    // interrupt enable
        IPR(PERIA, INTA230) = 1;    // interrupt priority
        s_meter1A = this;
        break;
    case 3: // P13 / MTIOC0B
        MSTP(MTU0) = 0;             // awake from module stop
        MPC.P13PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORT1.PDR.BIT.B3 = 0;       // port direction (input)
        PORT1.PMR.BIT.B3 = 1;       // port mode (peripheral)
        MTU0.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU0.TIORH.BIT.IOB = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST0 = 0x01;  // count start
        MTU0.TIER.BIT.TGIEB = 1;    // interrupt enable
        ICU.SLIAR231.BYTE   = 2;    // interrupt factor MTU0 TGIB0
        IEN(PERIA, INTA231) = 1;    // interrupt enable
        IPR(PERIA, INTA231) = 1;    // interrupt priority
        s_meter0B = this;
        break;
    case 5: // PC3 / MTIOC4D
        MSTP(MTU4) = 0;             // awake from module stop
        MPC.PC3PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTC.PDR.BIT.B3 = 0;       // port direction (input)
        PORTC.PMR.BIT.B3 = 1;       // port mode (peripheral)
        MTU4.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU4.TIORL.BIT.IOD = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST4 = 0x01;  // count start
        MTU4.TIER.BIT.TGIED = 1;    // interrupt enable
        ICU.SLIAR232.BYTE   = 24;   // interrupt factor MTU4 TGID4
        IEN(PERIA, INTA232) = 1;    // interrupt enable
        IPR(PERIA, INTA232) = 1;    // interrupt priority
        s_meter4D = this;
        break;
    case 7: // P32 / MTIOC0C
        MSTP(MTU0) = 0;             // awake from module stop
        MPC.P32PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORT3.PDR.BIT.B2 = 0;       // port direction (input)
        PORT3.PMR.BIT.B2 = 1;       // port mode (peripheral)
        MTU0.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU0.TIORL.BIT.IOC = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST0 = 0x01;  // count start
        MTU0.TIER.BIT.TGIEC = 1;    // interrupt enable
        ICU.SLIAR233.BYTE   = 3;    // interrupt factor MTU0 TGIC0
        IEN(PERIA, INTA233) = 1;    // interrupt enable
        IPR(PERIA, INTA233) = 1;    // interrupt priority
        s_meter0C = this;
        break;
    case 14: // PD2 / MTIOC4D
        MSTP(MTU4) = 0;             // awake from module stop
        MPC.PD2PFS.BIT.PSEL = 0x01; // pin function (MTIOC)
        PORTD.PDR.BIT.B2 = 0;       // port direction (input)
        PORTD.PMR.BIT.B2 = 1;       // port mode (peripheral)
        MTU4.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU4.TIORL.BIT.IOD = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST4 = 0x01;  // count start
        MTU4.TIER.BIT.TGIED = 1;    // interrupt enable
        ICU.SLIAR232.BYTE   = 24;   // interrupt factor MTU4 TGID4
        IEN(PERIA, INTA232) = 1;    // interrupt enable
        IPR(PERIA, INTA232) = 1;    // interrupt priority
        s_meter4D = this;
        break;
    case 15: // PD3 / MTIOC8D
        MSTP(MTU8) = 0;             // awake from module stop
        MPC.PD3PFS.BIT.PSEL = 0x08; // pin function (MTIOC)
        PORTD.PDR.BIT.B3 = 0;       // port direction (input)
        PORTD.PMR.BIT.B3 = 1;       // port mode (peripheral)
        MTU8.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU8.TIORL.BIT.IOD = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST8 = 0x01;  // count start
        MTU8.TIER.BIT.TGIED = 1;    // interrupt enable
        ICU.SLIAR234.BYTE   = 44;   // interrupt factor MTU8 TGID8
        IEN(PERIA, INTA234) = 1;    // interrupt enable
        IPR(PERIA, INTA234) = 1;    // interrupt priority
        s_meter8D = this;
        break;
    case 16: // PD4 / MTIOC8B
        MSTP(MTU8) = 0;             // awake from module stop
        MPC.PD4PFS.BIT.PSEL = 0x08; // pin function (MTIOC)
        PORTD.PDR.BIT.B4 = 0;       // port direction (input)
        PORTD.PMR.BIT.B4 = 1;       // port mode (peripheral)
        MTU8.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU8.TIORH.BIT.IOB = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST8 = 0x01;  // count start
        MTU8.TIER.BIT.TGIEB = 1;    // interrupt enable
        ICU.SLIAR235.BYTE   = 42;   // interrupt factor MTU8 TGIB8
        IEN(PERIA, INTA235) = 1;    // interrupt enable
        IPR(PERIA, INTA235) = 1;    // interrupt priority
        s_meter8B = this;
        break;
    case 17: // PD5 / MTIOC8C
        MSTP(MTU8) = 0;             // awake from module stop
        MPC.PD5PFS.BIT.PSEL = 0x08; // pin function (MTIOC)
        PORTD.PDR.BIT.B5 = 0;       // port direction (input)
        PORTD.PMR.BIT.B5 = 1;       // port mode (peripheral)
        MTU8.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU8.TIORL.BIT.IOC = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST8 = 0x01;  // count start
        MTU8.TIER.BIT.TGIEC = 1;    // interrupt enable
        ICU.SLIAR236.BYTE   = 43;   // interrupt factor MTU8 TGIC8
        IEN(PERIA, INTA236) = 1;    // interrupt enable
        IPR(PERIA, INTA236) = 1;    // interrupt priority
        s_meter8C = this;
        break;
    case 18: // PD6 / MTIOC8A
        MSTP(MTU8) = 0;             // awake from module stop
        MPC.PD6PFS.BIT.PSEL = 0x08; // pin function (MTIOC)
        PORTD.PDR.BIT.B6 = 0;       // port direction (input)
        PORTD.PMR.BIT.B6 = 1;       // port mode (peripheral)
        MTU8.TCR2.BIT.TPSC2 = clksel; // clock select
        MTU8.TIORH.BIT.IOA = pulse_edge; // edge select
        MTU.TSTRA.BIT.CST8 = 0x01;  // count start
        MTU8.TIER.BIT.TGIEA = 1;    // interrupt enable
        ICU.SLIAR237.BYTE   = 41;   // interrupt factor MTU8 TGIA8
        IEN(PERIA, INTA237) = 1;    // interrupt enable
        IPR(PERIA, INTA237) = 1;    // interrupt priority
        s_meter8A = this;
        break;
#endif // GR-ROSE
    default:
        this->pin = 0xFF;   // invalid value
    }
    
// Don't lock because GR-CITRUS library doesn't unlock
#if 0
    // lock the protect of module stop
    SYSTEM.PRCR.WORD = 0xA500;
    // lock the protect of pin function selector
    MPC.PWPR.BIT.PFSWE = 0;
    MPC.PWPR.BIT.B0WI = 1;
#endif
    
    popi();     // restore the interrupt status
}

/*
 * stop the PWMeter
 */
void PWMeter::stop()
{
    pushi();    // backup the interrupt status
    cli();      // disable interrupts
    
    this->stop_restart(0);
    
    popi();     // restore the interrupt status
}

/*
 * restart the PWMeter
 */
void PWMeter::restart()
{
    pushi();    // backup the interrupt status
    cli();      // disable interrupts
    
    // initialize variables
    if(polarity == PWMETER_POSITIVE){
        pulse_edge = PULSE_RISING;
    }else{
        pulse_edge = PULSE_FALLING;
    }
    rptr = 0;
    wptr = 0;
    
    this->stop_restart(1);
    
    popi();     // restore the interrupt status
}

/**
 * stop/restart the PWMeter (subroutine)
 * start_stop: 1=start / 0=stop
 */
void PWMeter::stop_restart(uint8_t start_stop)
{
    switch(pin){
#ifdef GRCITRUS
    case 0: // P20 / MTIOC1A
        MTU1.TIOR.BIT.IOA = pulse_edge;     // edge select
        MTU.TSTR.BIT.CST1 = start_stop;     // count start/stop
        MTU1.TIER.BIT.TGIEA = start_stop;   // interrupt enable/disable
        IEN(MTU1, TGIA1) = start_stop;      // interrupt enable/disable
        break;
    case 1: // P21 / MTIOC1B
        MTU1.TIOR.BIT.IOB = pulse_edge;     // edge select
        MTU.TSTR.BIT.CST1 = start_stop;     // count start/stop
        MTU1.TIER.BIT.TGIEB = start_stop;   // interrupt enable/disable
        IEN(MTU1, TGIB1) = start_stop;      // interrupt enable/disable
        break;
    case 2: // PC0 / MTIOC3C
        MTU3.TIORL.BIT.IOC = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST3 = start_stop;     // count start/stop
        MTU3.TIER.BIT.TGIEC = start_stop;   // interrupt enable/disable
        IEN(MTU3, TGIC3) = start_stop;      // interrupt enable/disable
        break;
    case 3: // PC1 / MTIOC3A
        MTU3.TIORH.BIT.IOA = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST3 = start_stop;     // count start/stop
        MTU3.TIER.BIT.TGIEA = start_stop;   // interrupt enable/disable
        IEN(MTU3, TGIA3) = start_stop;      // interrupt enable/disable
        break;
    case 4: // PC2 / MTIOC4B
        MTU4.TIORH.BIT.IOB = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST4 = start_stop;     // count start/stop
        MTU4.TIER.BIT.TGIEB = start_stop;   // interrupt enable/disable
        IEN(MTU4, TGIB4) = start_stop;      // interrupt enable/disable
        break;
    case 7: // P32 / MTIOC0C
        MTU0.TIORL.BIT.IOC = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST0 = start_stop;     // count start/stop
        MTU0.TIER.BIT.TGIEC = start_stop;   // interrupt enable/disable
        IEN(MTU0, TGIC0) = start_stop;      // interrupt enable/disable
        break;
    case 8: // P33 / MTIOC0D
        MTU0.TIORL.BIT.IOD = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST0 = start_stop;     // count start/stop
        MTU0.TIER.BIT.TGIED = start_stop;   // interrupt enable/disable
        IEN(MTU0, TGID0) = start_stop;      // interrupt enable/disable
        break;
    case 10: // PC4 / MTIOC3D
        MTU3.TIORL.BIT.IOD = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST3 = start_stop;     // count start/stop
        MTU3.TIER.BIT.TGIED = start_stop;   // interrupt enable/disable
        IEN(MTU3, TGID3) = start_stop;      // interrupt enable/disable
        break;
    case 11: // PC6 / MTIOC3C
        MTU3.TIORL.BIT.IOC = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST3 = start_stop;     // count start/stop
        MTU3.TIER.BIT.TGIEC = start_stop;   // interrupt enable/disable
        IEN(MTU3, TGIC3) = start_stop;      // interrupt enable/disable
        break;
    case 12: // PC7 / MTIOC3A
        MTU3.TIORH.BIT.IOA = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST3 = start_stop;     // count start/stop
        MTU3.TIER.BIT.TGIEA = start_stop;   // interrupt enable/disable
        IEN(MTU3, TGIA3) = start_stop;      // interrupt enable/disable
        break;
    case 13: // PC5 / MTIOC3B
        MTU3.TIORH.BIT.IOB = pulse_edge;    // edge select
        MTU.TSTR.BIT.CST3 = start_stop;     // count start/stop
        MTU3.TIER.BIT.TGIEB = start_stop;   // interrupt enable/disable
        IEN(MTU3, TGIB3) = start_stop;      // interrupt enable/disable
        break;
#else // GR-ROSE
    case 1: // P20 / MTIOC1A
        MTU1.TIOR.BIT.IOA = pulse_edge;     // edge select
        MTU.TSTRA.BIT.CST1 = start_stop;    // count start/stop
        MTU1.TIER.BIT.TGIEA = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA230) = start_stop;   // interrupt enable
        break;
    case 3: // P13 / MTIOC0B
        MTU0.TIORH.BIT.IOB = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST0 = start_stop;    // count start/stop
        MTU0.TIER.BIT.TGIEB = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA231) = start_stop;   // interrupt enable
        break;
    case 5: // PC3 / MTIOC4D
        MTU4.TIORL.BIT.IOD = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST4 = start_stop;    // count start/stop
        MTU4.TIER.BIT.TGIED = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA232) = start_stop;   // interrupt enable
        break;
    case 7: // P32 / MTIOC0C
        MTU0.TIORL.BIT.IOC = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST0 = start_stop;    // count start/stop
        MTU0.TIER.BIT.TGIEC = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA233) = start_stop;   // interrupt enable
        break;
    case 14: // PD2 / MTIOC4D
        MTU4.TIORL.BIT.IOD = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST4 = start_stop;    // count start/stop
        MTU4.TIER.BIT.TGIED = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA232) = start_stop;   // interrupt enable
        break;
    case 15: // PD3 / MTIOC8D
        MTU8.TIORL.BIT.IOD = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST8 = start_stop;    // count start/stop
        MTU8.TIER.BIT.TGIED = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA234) = start_stop;   // interrupt enable
        break;
    case 16: // PD4 / MTIOC8B
        MTU8.TIORH.BIT.IOB = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST8 = start_stop;    // count start/stop
        MTU8.TIER.BIT.TGIEB = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA235) = start_stop;   // interrupt enable
        break;
    case 17: // PD5 / MTIOC8C
        MTU8.TIORL.BIT.IOC = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST8 = start_stop;    // count start/stop
        MTU8.TIER.BIT.TGIEC = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA236) = start_stop;   // interrupt enable
        break;
    case 18: // PD6 / MTIOC8A
        MTU8.TIORH.BIT.IOA = pulse_edge;    // edge select
        MTU.TSTRA.BIT.CST8 = start_stop;    // count start/stop
        MTU8.TIER.BIT.TGIEA = start_stop;   // interrupt enable/disable
        IEN(PERIA, INTA237) = start_stop;   // interrupt enable
        break;
#endif // GR-ROSE
    default:
        this->pin = 0xFF;   // invalid value
    }
}

/*
 * new value available?
 */
bool PWMeter::available()
{
    bool ret;
    
    pushi();    // backup the interrupt status
    cli();      // disable interrupts
    
    ret = (rptr != wptr);
    
    popi();     // restore the interrupt status
    
    return ret;
}

/*
 * get a buffered value
 * return: pulse witdh[usec], or 0xFFFF if no data is available
 */
uint16_t PWMeter::get()
{
    uint16_t ret;
    
    pushi();    // backup the interrupt status
    cli();      // disable interrupts
    
    if(rptr != wptr){
        ret = buffer[rptr];
        if(++rptr >= PWMETER_BUFF_SIZE) rptr = 0;
    }else{
        ret = 0xFFFF;
    }
    
    popi();     // restore the interrupt status
    
    if(to1usec){
#ifdef GRCITRUS
        if(ret != 0xFFFF) ret /= 3;
#else // GR-ROSE
        if(ret != 0xFFFF) ret = (uint16_t)(((uint32_t)ret * 4) / 15);
#endif
    }
    
    return ret;
}

/*
 * get the last value [usec]
 * return: pulse witdh[usec], or 0xFFFF if no data is available
 */
uint16_t PWMeter::getLast()
{
    uint16_t ret;
    
    pushi();    // backup the interrupt status
    cli();      // disable interrupts
    
    if(rptr != wptr){
        rptr = (wptr > 0) ? (wptr - 1) : (PWMETER_BUFF_SIZE - 1);
        ret = buffer[rptr];
        if(++rptr >= PWMETER_BUFF_SIZE) rptr = 0;
    }else{
        ret = 0xFFFF;
    }
    
    popi();     // restore the interrupt status
    
    if(to1usec){
#ifdef GRCITRUS
        if(ret != 0xFFFF) ret /= 3;
#else // GR-ROSE
        if(ret != 0xFFFF) ret = (uint16_t)(((uint32_t)ret * 4) / 15);
#endif
    }
    
    return ret;
}
