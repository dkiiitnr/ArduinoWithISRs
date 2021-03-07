void CPAP_startPopCallback(void *ptr)
{
  display_mode_counter = 1;
  c_inhale.getValue(&inhale_value);
  c_exhale.getValue(&exhale_value);
  c_pressure.getValue(&pressure_value);
  c_bpm.getValue(&bpm_value);
  
  Serial.print("inhale: ");
  Serial.print(inhale_value);
  Serial.print("  ");
  Serial.print("exhale: ");
  Serial.print(exhale_value);
  Serial.print("  ");
  Serial.print("pressure: ");
  Serial.print(pressure_value);
  Serial.print("  ");
  Serial.print("BPM: ");
  Serial.print(bpm_value);
  Serial.println();
}


void BiPAP_startPopCallback(void *ptr)
{
  
  display_mode_counter = 2;
  b_inhale.getValue(&inhale_value);
  b_exhale.getValue(&exhale_value);
  b_pressure.getValue(&pressure_value);
  b_bpm.getValue(&bpm_value);
  peep_speed.getValue(&peep_speed_value);
  tidal_volume.getValue(&tidal_volume_value);
  Serial.print("inhale: ");
  Serial.print(inhale_value);
  Serial.print("  ");
  Serial.print("exhale: ");
  Serial.print(exhale_value);
  Serial.print("  ");
  Serial.print("pressure: ");
  Serial.print(pressure_value);
  Serial.print("  ");
  Serial.print("BPM: ");
  Serial.print(bpm_value);
  Serial.print("  ");
  Serial.print("Tidal_volume: ");
  Serial.print(tidal_volume_value);
  Serial.print("  ");
  Serial.print("PEEP_speed: ");
  Serial.print(peep_speed_value);
  Serial.println();
}



void Display_ISR (float display_time)
{
//  set_inhale_timer = false;
//  exhale_complete = true;
  noInterrupts();
  TCCR5A = 0;
  TCCR5B = 0;
  double value = (double)(65535 - (15625 * display_time));
  TCNT5 = value;                        // preload timer

  TCCR5B |= (1 << CS50) | (1 << CS52);  // 1024 prescaler
  TIMSK5 |= (1 << TOIE5);               // enable timer overflow interrupt ISR
  Serial.println(value);
  interrupts();

}
