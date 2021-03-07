
ISR(TIMER1_OVF_vect)                    // interrupt service routine for Inhale timer overflow
{
  Serial.println("Inside Inhale ISR");
  set_exhale_timer = true;
  exhale_complete = false;
}

ISR(TIMER3_OVF_vect)                    // interrupt service routine for Exhale timer overflow
{
  Serial.println("Inside Exhale ISR");
  set_inhale_timer = true;
  inhale_complete = false;
}

ISR(TIMER2_OVF_vect)                    // interrupt service routine for Exhale timer overflow
{
      //Serial.println("Inside Exhale ISR");
      //  set_inhale_timer = true;
      //  inhale_complete = false;

  Serial.println("Inside display ISR");
}
