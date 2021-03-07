CPAP_params continuous_positive_airway_pressure()
{
  CPAP_params parameters;
  int pressure_knob = get_pressure_knob(pot_pin);
  Serial.print("  ");
  motor.write(pressure_knob);
  parameters.pressure = get_AS21_Pressure(AS21_pin);
  Serial.print("Pressure: ");
  Serial.print(parameters.pressure);

  parameters.flow = get_flowrate_from_sfm();
  Serial.print(" ");
  Serial.print("CPAP_flow: ");
  Serial.print(parameters.flow);
  Serial.println();
  return parameters;

}
