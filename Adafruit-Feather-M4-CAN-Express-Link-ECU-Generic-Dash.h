#ifndef Adafruit_Feather_M4_CAN_Express_Link_ECU_GENERIC_DASH_h
#define Adafruit_Feather_M4_CAN_Express_Link_ECU_GENERIC_DASH_h

// CAN BUS configuration
#define CAN_BAUD 500000
#define ECU_HEADER 200

// Serial port configuration
#define SERIAL_BAUD 9600
#define SERIAL_UPDATE_MILLISECONDS 500

// No user-serviceable parts below unless you REALLY know what you are doing

// List of known parameters sent via Generic Dash
enum GenericDashParameters {
  ECU_ENGINE_SPEED_RPM,
  ECU_MAP_KPA,
  ECU_MGP_KPA,
  ECU_BAROMETRIC_PRESSURE_KPA,
  ECU_THROTTLE_POSITION_PERCENT,
  ECU_INJECTOR_DUTY_CYCLE_PERCENT,
  ECU_SECOND_STAGE_INJECTOR_DUTY_CYCLE_PERCENT,
  ECU_INJECTOR_PULSE_WIDTH_MS,
  ECU_ENGINE_COOLANT_TEMPERATURE_DEGREES_C,
  ECU_INTAKE_AIR_TEMPERATURE_DEGRESS_C,
  ECU_BATTERY_VOLTAGE,
  ECU_MASS_AIR_FLOW_GRAMS_PER_SECOND,
  ECU_GEAR_POSITION,
  ECU_INJECTOR_TIMING_DEGREES,
  ECU_IGNITION_TIMING_DEGREES,
  ECU_CAM_INLET_POSITION_L_DEGREES,
  ECU_CAM_INLET_POSITION_R_DEGREES,
  ECU_CAM_EXHAUST_POSITION_L_DEGREES,
  ECU_CAM_EXHAUST_POSITION_R_DEGREES,
  ECU_LAMBDA_1_LAMBDA,
  ECU_LAMBDA_2_LAMBDA,
  ECU_TRIGGER_1_ERROR_COUNT,
  ECU_FAULT_CODES_COUNT,
  ECU_FUEL_PRESSURE_KPA,
  ECU_OIL_TEMPERATURE_DEGREES_C,
  ECU_OIL_PRESSURE_KPA,
  ECU_LEFT_FRONT_WHEEL_SPEED_KPH,
  ECU_LEFT_REAR_WHEEL_SPEED_KPH,
  ECU_RIGHT_FRONT_WHEEL_SPEED_KPH,
  ECU_RIGHT_REAR_WHEEL_SPEED_KPH,
  ECU_KNOCK_LEVEL_1_COUNT,
  ECU_KNOCK_LEVEL_2_COUNT,
  ECU_KNOCK_LEVEL_3_COUNT,
  ECU_KNOCK_LEVEL_4_COUNT,
  ECU_KNOCK_LEVEL_5_COUNT,
  ECU_KNOCK_LEVEL_6_COUNT,
  ECU_KNOCK_LEVEL_7_COUNT,
  ECU_KNOCK_LEVEL_8_COUNT,
  ECU_LIMIT_FLAGS_BITFIELD
};

// List of known limits flags sent as ECU_LIMIT_FLAGS_BITFIELD
#define LIMITS_FLAG_RPM_LIMIT                   0
#define LIMITS_FLAG_MAP_LIMIT                   1
#define LIMITS_FLAG_SPEED_LIMIT                 2
#define LIMITS_FLAG_MAXIMUM_IGNITION_FLAG       3
#define LIMITS_FLAG_ANTI_LAG_IGNITION_CUT       4
#define LIMITS_FLAG_HIGH_VOLTAGE_SUPPLIY_LIMIT  5
#define LIMITS_FLAG_OVERRUN_FLAG                6
#define LIMITS_FLAG_TRACTION_LIMIT              7
#define LIMITS_FLAG_LOW_SUPPLY_VOLTAGE_LIMIT    8
#define LIMITS_FLAG_LAUNCH_RPM_LIMIT            9
#define LIMITS_FLAG_WAKEUP_FLAG                 10
#define LIMITS_FLAG_GP_RPM_LIMIT_1              11
#define LIMITS_FLAG_CL_STEPPER_LIMIT            12
#define LIMITS_FLAG_GP_RPM_LIMIT_2              13
#define LIMITS_FLAG_ETHROTTLE_LIMIT             14
#define LIMITS_FLAG_CYCLIC_IDLE_ACTIVE          15

// Constants used by GenericDash multi-dimensional array - DO NOT CHANGE THESE
#define GenericDashFrames 13
#define GenericDashBytes 8

// Function to get a specific value from the Generic Dash buffer
float getGenericDashValue(volatile unsigned char dataList[GenericDashFrames][GenericDashBytes], GenericDashParameters param) {
  switch (param) {
    case ECU_ENGINE_SPEED_RPM:
      return (signed int)((dataList[0][3] << 8) + (dataList[0][2] << 0));
      break;
    case ECU_MAP_KPA:
      return (signed int)((dataList[0][5] << 8) + (dataList[0][4] << 0));
      break;
    case ECU_MGP_KPA:
      return (signed int)((dataList[0][7] << 8) + (dataList[0][6] << 0)) - 100;
      break;
    case ECU_BAROMETRIC_PRESSURE_KPA:
      return (signed int)((dataList[1][3] << 8) + (dataList[1][2] << 0)) * 0.1;
      break;
    case ECU_THROTTLE_POSITION_PERCENT:
      return (signed int)((dataList[1][5] << 8) + (dataList[1][4] << 0)) * 0.1;
      break;
    case ECU_INJECTOR_DUTY_CYCLE_PERCENT:
      return (signed int)((dataList[1][7] << 8) + (dataList[1][6] << 0)) * 0.1;
      break;
    case ECU_SECOND_STAGE_INJECTOR_DUTY_CYCLE_PERCENT:
      return (signed int)((dataList[2][3] << 8) + (dataList[2][2] << 0)) * 0.1;
      break;
    case ECU_INJECTOR_PULSE_WIDTH_MS:
      return (signed int)((dataList[2][5] << 8) + (dataList[2][4] << 0)) * 0.001;
      break;
    case ECU_ENGINE_COOLANT_TEMPERATURE_DEGREES_C:
      return (signed int)((dataList[2][7] << 8) + (dataList[2][6] << 0)) - 50;
      break;
    case ECU_INTAKE_AIR_TEMPERATURE_DEGRESS_C:
      return (signed int)((dataList[3][3] << 8) + (dataList[3][2] << 0)) - 50;
      break;
    case ECU_BATTERY_VOLTAGE:
      return (signed int)((dataList[3][5] << 8) + (dataList[3][4] << 0)) * 0.01;
      break;
    case ECU_MASS_AIR_FLOW_GRAMS_PER_SECOND:
      return (signed int)((dataList[3][7] << 8) + (dataList[3][6] << 0)) * 0.1;
      break;
    case ECU_GEAR_POSITION:
      return (signed int)((dataList[4][3] << 8) + (dataList[4][2] << 0));
      break;
    case ECU_INJECTOR_TIMING_DEGREES:
      return (signed int)((dataList[4][5] << 8) + (dataList[4][4] << 0));
      break;
    case ECU_IGNITION_TIMING_DEGREES:
      return (signed int)(((dataList[4][7] << 8) + (dataList[4][6] << 0)) * 0.1) - 100;
      break;
    case ECU_CAM_INLET_POSITION_L_DEGREES:
      return (signed int)((dataList[5][3] << 8) + (dataList[5][2] << 0)) * 0.1;
      break;
    case ECU_CAM_INLET_POSITION_R_DEGREES:
      return (signed int)((dataList[5][5] << 8) + (dataList[5][4] << 0)) * 0.1;
      break;
    case ECU_CAM_EXHAUST_POSITION_L_DEGREES:
      return (signed int)((dataList[5][7] << 8) + (dataList[5][6] << 0)) * -0.1;
      break;
    case ECU_CAM_EXHAUST_POSITION_R_DEGREES:
      return (signed int)((dataList[6][3] << 8) + (dataList[6][2] << 0)) * -0.1;
      break;
    case ECU_LAMBDA_1_LAMBDA:
      return (signed int)((dataList[6][5] << 8) + (dataList[6][4] << 0)) * 0.001;
      break;
    case ECU_LAMBDA_2_LAMBDA:
      return (signed int)((dataList[6][7] << 8) + (dataList[6][6] << 0)) * 0.001;
      break;
    case ECU_TRIGGER_1_ERROR_COUNT:
      return (signed int)((dataList[7][3] << 8) + (dataList[7][2] << 0));
      break;
    case ECU_FAULT_CODES_COUNT:
      return (signed int)((dataList[7][5] << 8) + (dataList[7][4] << 0));
      break;
    case ECU_FUEL_PRESSURE_KPA:
      return (signed int)((dataList[7][7] << 8) + (dataList[7][6] << 0));
      break;
    case ECU_OIL_TEMPERATURE_DEGREES_C:
      return (signed int)((dataList[8][3] << 8) + (dataList[8][2] << 0)) - 50;
      break;
    case ECU_OIL_PRESSURE_KPA:
      return (signed int)((dataList[8][5] << 8) + (dataList[8][4] << 0));
      break;
    case ECU_LEFT_FRONT_WHEEL_SPEED_KPH:
      return (signed int)((dataList[8][7] << 8) + (dataList[8][6] << 0)) * 0.1;
      break;
    case ECU_LEFT_REAR_WHEEL_SPEED_KPH:
      return (signed int)((dataList[9][3] << 8) + (dataList[9][2] << 0)) * 0.1;
      break;
    case ECU_RIGHT_FRONT_WHEEL_SPEED_KPH:
      return (signed int)((dataList[9][5] << 8) + (dataList[9][4] << 0)) * 0.1;
      break;
    case ECU_RIGHT_REAR_WHEEL_SPEED_KPH:
      return (signed int)((dataList[9][7] << 8) + (dataList[9][6] << 0)) * 0.1;
      break;
    case ECU_KNOCK_LEVEL_1_COUNT:
      return (signed int)((dataList[10][3] << 8) + (dataList[10][2] << 0)) * 5;
      break;
    case ECU_KNOCK_LEVEL_2_COUNT:
      return (signed int)((dataList[10][5] << 8) + (dataList[10][4] << 0)) * 5;
      break;
    case ECU_KNOCK_LEVEL_3_COUNT:
      return (signed int)((dataList[10][7] << 8) + (dataList[10][6] << 0)) * 5;
      break;
    case ECU_KNOCK_LEVEL_4_COUNT:
      return (signed int)((dataList[11][3] << 8) + (dataList[11][2] << 0)) * 5;
      break;
    case ECU_KNOCK_LEVEL_5_COUNT:
      return (signed int)((dataList[11][5] << 8) + (dataList[11][4] << 0)) * 5;
      break;
    case ECU_KNOCK_LEVEL_6_COUNT:
      return (signed int)((dataList[11][7] << 8) + (dataList[11][6] << 0)) * 5;
      break;
    case ECU_KNOCK_LEVEL_7_COUNT:
      return (signed int)((dataList[12][3] << 8) + (dataList[12][2] << 0)) * 5;
      break;
    case ECU_KNOCK_LEVEL_8_COUNT:
      return (signed int)((dataList[12][5] << 8) + (dataList[12][4] << 0)) * 5;
      break;
    case ECU_LIMIT_FLAGS_BITFIELD:
      return (signed int)((dataList[12][7] << 8) + (dataList[12][6] << 0));
      break;
    default:
      return -1;
      break;
  }
}

#endif // Adafruit_Feather_M4_CAN_Express_Link_ECU_GENERIC_DASH_h
