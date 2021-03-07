void bilevel_positive_airway_pressure(float inhale, float exhale, float breaths_per_minute)
{
  float total_time = inhale + exhale;
  float inhale_time = (60 / breaths_per_minute) * ( inhale / total_time);
  float exhale_time = (60 / breaths_per_minute) * ( exhale / total_time);
  float flow = get_SDP11108_flow_rate(A4);
  int pressure_knob = get_pressure_knob(pot_pin);
  int inhale_volume = inhale_time * flow;
//  pressure_time(pressure_knob);
//  volume_time(inhale_volume);
//  flow_time(flow);
//  Serial.print("inhale_time: ");
//  Serial.print(inhale_time);
//  Serial.print(" ");
//  Serial.print("exhale_time: ");
//  Serial.print(exhale_time);
//  Serial.print(" ");
//  Serial.print("total_time: ");
//  Serial.print(total_time);
//  Serial.println();
  if (set_inhale_timer)
  {
    inhale_timer_init(inhale_time);
  }
  if (exhale_complete)
  {
    motor.write(pressure_knob);
    //    Serial.println("Inside Inhale Loop");
  }
  if (set_exhale_timer)
  {
    exhale_timer_init(exhale_time);
  }
  if (inhale_complete)
  {
    int pressure_angle = cmh2o_to_motor_angle(peep_speed_value);
    motor.write(pressure_angle);
    //    Serial.println("Inside Exhale Loop");
  }
}


void inhale_timer_init(float time_in_seconds)
{
  set_inhale_timer = false;
  exhale_complete = true;
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  double value = (double)(65535 - (15625 * time_in_seconds));
  TCNT1 = value;                        // preload timer

  TCCR1B |= (1 << CS10) | (1 << CS12);  // 1024 prescaler
  TIMSK1 |= (1 << TOIE1);               // enable timer overflow interrupt ISR
  Serial.println("Inhale_Timer_Init");
  interrupts();

}
void exhale_timer_init(float time_in_seconds)
{
  set_exhale_timer = false;
  inhale_complete = true;
  noInterrupts();
  TCCR3A = 0;
  TCCR3B = 0;
  double value = (double)(65535 - (15625 * time_in_seconds));
  TCNT3 = value;                        // preload timer

  TCCR3B |= (1 << CS30) | (1 << CS32);  // 1024 prescaler
  TIMSK3 |= (1 << TOIE3);               // enable timer overflow interrupt ISR
  Serial.println("Exhale_Timer_Init");
  interrupts();

}
