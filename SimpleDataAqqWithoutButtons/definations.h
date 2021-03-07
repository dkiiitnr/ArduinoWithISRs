#include <Servo.h>
#include <SFM4200CORE.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>
#include <Nextion.h>
#include <NexNumber.h>
#include <NexButton.h>
#include <NexText.h>
#include <NexPicture.h>
#include <NexHotspot.h>

//global variables
uint32_t inhale_value = 1;
uint32_t exhale_value = 4;
uint32_t pressure_value = 0;
uint32_t bpm_value = 12;
uint32_t tidal_volume_value = 350;
uint32_t peep_speed_value = 5;
uint8_t display_mode_counter = 0;
float display_time  =0.01;

//Insatnces of Sensors And Motor.
SFM4200CORE senseFlow(0x40);
Adafruit_ADS1115 ads(0x48);
Servo oxygen_motor;
Servo motor;

// Variables for Pressure and Flow Rate Calculation.
double differential_pressure;
float sensor_ambient_air = 44.0;
float first_static_pressure, second_static_pressure;
float flow_rate, air_pressure;
int potValue;
float differential_voltage = 0.0;
int offset = 0; // Offset
float scale = 256.0; // Scale factor for Air & N2 is 140.0, O2 is 142.8
int oxygen_motor_pin = 11;

//PID_Variables
float kp =0.5;


//Pin Configurations.
#define servo_pin 4
//#define first_mpxv A1
//#define second_mpxv A0
#define AS21_pin A3
//#define mpxv_pin A7
#define pot_pin A0
#define oxygen_knob_pin A1

//Flow Calculation Constant.
#define discharg_coefficient 0.95
#define fluid_density 1.25
#define area_constant 0.000066

//Main Array Constant.
#define MAX 5

//mode change Definations and Variables.
#define PEEP 3
#define BPM 2
#define IE 19
#define MODE 18


volatile int peep_counter = 0;
volatile int bpm_counter = 0;
volatile int ie_counter = 0;
volatile int mode_counter = 2;

//BiPAP Variable.
NexNumber b_inhale = NexNumber(2, 10, "b_inahle");
NexNumber b_exhale = NexNumber(2, 11, "b_exhale");
NexNumber b_pressure = NexNumber(2, 12, "b_pressure");
NexNumber b_bpm = NexNumber(2, 13, "b_bpm");
NexNumber tidal_volume = NexNumber(2, 14, "tidal_volume");
NexNumber peep_speed = NexNumber(2, 15, "peep_speed");
NexHotspot BiPAP_start = NexHotspot(2, 4, "bipap_start");

bool end_loop = false;
bool inhale_complete = false;
bool exhale_complete = true;
//float total_time = inhale_value + exhale_value;
int oxygen_knob = 0;
volatile bool set_inhale_timer = true;
volatile bool set_exhale_timer = false;
//SIMV Variables
float inspiration_trigger_window = 2.0;


//CPAP Parameters
NexNumber c_inhale = NexNumber(1, 6, "c_inahle");
NexNumber c_exhale = NexNumber(1, 7, "c_exhale");
NexNumber c_pressure = NexNumber(1, 4, "c_pressure");
NexNumber c_bpm = NexNumber(1, 5, "c_bpm");
NexHotspot CPAP_start = NexHotspot(1, 3, "cpap_start");

//void bilevel_positive_airway_pressure(inhale_time, exhale_time, breaths_per_minute, total_time);

//Structure for CPAP Parameters
struct CPAP_params
{
  float flow;
  float pressure;
} Suyash;
