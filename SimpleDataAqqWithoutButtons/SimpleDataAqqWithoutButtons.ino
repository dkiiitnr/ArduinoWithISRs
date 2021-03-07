#include "definations.h"
uint32_t get_flowrate_from_sfm()
{
  float flow_rate = senseFlow.getvalue(); // slm
  return (16.67 * (flow_rate - offset) / scale);
}

float get_AS21_Pressure(int analog_pin)
{
  float AS21_adc = analogRead(A7);
  float air_pressure = ((AS21_adc - 517) / 41.66);
  return (air_pressure);
}

float get_SDP11108_flow_rate(int analog_pin)
{
  float sensor_ADC = (analogRead(analog_pin));
  float voltage_mV = ((5000 / 1023) * sensor_ADC);
  float voltage = (voltage_mV / 1000);
  float differential_pressure = sq((voltage - 0.25 ) / (3.75)) * 500;
  double flow_rate_mL_sec = discharg_coefficient * sqrt(2 * (abs(differential_pressure)) / fluid_density) * area_constant;

  return (flow_rate_mL_sec * 1000000);
}

float get_oxygen_value()
{
  return ads.readADC_Differential_0_1();
}

int get_pressure_knob(int analog_pin)
{
  int pressure_value = analogRead(analog_pin);
  int scaled_pressure = map(pressure_value, 0, 1023, 0, 180);
  return scaled_pressure;
}

int get_oxygen_knob(int analog_pin)
{
  int oxygen_value = analogRead(analog_pin);
  int scaled_oxygen = map(oxygen_value, 0, 1023, 0, 180);
  return scaled_oxygen;
}

int cmh2o_to_motor_angle(uint32_t cmh2o)
{
  int motor_angle = map( cmh2o, 0, 30, 0, 180);
  return motor_angle;
}

void setup() {

  //Parameters For ADS1115 And Flow Rate
  ads.setGain(GAIN_TWO);
  ads.begin();
  senseFlow.init();
  
  analogReference(EXTERNAL);
  Serial.begin(9600);
  Serial2.begin(9600);
  nexInit();
  // Pins For Interrupt used for Modes and Other Quantity Change.
  pinMode(PEEP, INPUT_PULLUP);
  pinMode(BPM, INPUT_PULLUP);
  pinMode(IE, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);

  // Motor Setup and Min-Max Caliberation.
  motor.attach(servo_pin, 1000, 2000);
  Serial.println("Initilizing ESC...");
  motor.write(18);
  delay(3000);
  motor.write(0);
  delay(3000);
  oxygen_motor.attach(oxygen_motor_pin);
  oxygen_motor.write(0);
  CPAP_start.attachPop(CPAP_startPopCallback);
  BiPAP_start.attachPop(BiPAP_startPopCallback);
  Display_ISR(1.0);
}

NexTouch *nex_listen_list[] =
{
  &CPAP_start,
  &BiPAP_start,
  NULL
};

void loop() {

  nexLoop(nex_listen_list);
//  display_mode_counter = 12;
  if (display_mode_counter == 0)
  {
        Serial.println("No Mode");
  }
  else if (display_mode_counter == 1)
  {
    continuous_positive_airway_pressure();
  }
  else if (display_mode_counter == 2)
  {

    bilevel_positive_airway_pressure((float)inhale_value, (float)exhale_value, (float)bpm_value);
  }
  else
  {
    display_mode_counter = 0;
  }
}
