#include <main.h>
volatile UNSIGNED char idx1=0, str[10], ini=0; 
volatile SIGNED int32 val=0, a=0, b=0, c=0, trigger=0, Da=0, Db=0, Dc=0, Dd=0, time=65340;  //110  65390
volatile char flag_recepcion=0, flag_time=0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#INT_RDA 
void RDA_isr(VOID){
   UNSIGNED char ch;
   IF (kbhit()){
      ch = getc ();
      IF ((ch == '\n'||ch == '\r')){
         IF (idx1 != 0){
            str[idx1] = '\0';
            idx1 = 0;
            val = atol (str) ;
            idx1 = 0;
            flag_recepcion=1;
         }
      }
      ELSE{
            str[idx1] = ch;
            idx1++;
      }
   }
}

#INT_TIMER0
void  TIMER0_isr(VOID){
   set_timer0 (time);
   a=a+1;
   b=b+1;
   c=c+1;
   enable_interrupts (INT_RDA);
   flag_time=1;}
 
#INT_EXT
void  EXT_isr(VOID){
   a = 0;
   if(ini<=3){
      ini=ini+1;}}

#INT_EXT1
void  EXT1_isr(void) {
   b = 0;}

#INT_EXT2
void  EXT2_isr(void) {
   c = 0;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(){
   flag_recepcion=1; 
   val=50;
   val=val-10;
   output_low(PIN_B3);  //D1
   output_low(PIN_B4);  //D2
   output_low(PIN_B5);  //D3
   output_low(PIN_B6);  //D4
   output_low(PIN_B7);  //D5
   output_low(PIN_A4);  //D6
   
   enable_interrupts (INT_TIMER0);
   setup_timer_0 (RTCC_INTERNAL|RTCC_DIV_1);    //|RTCC_8_BIT
   set_timer0 (time);
   enable_interrupts (INT_EXT);
   enable_interrupts (INT_EXT1);
   enable_interrupts (INT_EXT2);
   enable_interrupts (GLOBAL);
   WHILE (TRUE){
      if(flag_recepcion==1){
         flag_recepcion=0;
         trigger=(0.9259259259*val);   // conversion de grados a segundos y escalizacion de segundos a desbordes *segundo((grados*(pi/180)*(1/60))/(2*pi))*10000
         printf("angulo de disparo %ld\r\n ",val);
         
         Da=0+trigger;
         Db=14+trigger;
         Dc=166+trigger; 
         Dd=180+trigger; 
      }
      
      if(flag_time==1 && ini>3){
         if(a==Da){
            output_high(Pin_B3);}
         else if(a==Db){
            output_low(Pin_B3);}
         if(a==Dc){
            output_high(Pin_B4);}
         else if(a==Dd){
            output_low(Pin_B4);}
         
         if(b==Da){
            output_high(Pin_B5);}
         else if(b==Db){
            output_low(Pin_B5);}
         else if(b==Dc){
            output_high(Pin_B6);}
         else if(b==Dd){
            output_low(Pin_B6);}
                  
         if(c==Da){
            output_high(Pin_B7);}
         else if(c==Db){
            output_low(Pin_B7);}
         else if(c==Dc){
            output_high(Pin_A4);}
         else if(c==Dd){
            output_low(Pin_A4);}
         
         flag_time=0;
      }
   }
}

