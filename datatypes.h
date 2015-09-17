#ifndef DATATYPES_H
#define DATATYPES_H

/*
    Copyright 2012-2015 Benjamin Vedder	benjamin@vedder.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include <QString>
#include <stdint.h>

typedef enum {
    FAULT_CODE_NONE = 0,
    FAULT_CODE_OVER_VOLTAGE,
    FAULT_CODE_UNDER_VOLTAGE,
    FAULT_CODE_DRV8302,
    FAULT_CODE_ABS_OVER_CURRENT,
    FAULT_CODE_OVER_TEMP_FET,
    FAULT_CODE_OVER_TEMP_MOTOR
} mc_fault_code;

typedef struct {
    double v_in;
    double temp_mos1;
    double temp_mos2;
    double temp_mos3;
    double temp_mos4;
    double temp_mos5;
    double temp_mos6;
    double temp_pcb;
    double current_motor;
    double current_in;
    double rpm;
    double duty_now;
    double amp_hours;
    double amp_hours_charged;
    double watt_hours;
    double watt_hours_charged;
    int tachometer;
    int tachometer_abs;
    mc_fault_code fault_code;
    QString fault_str;
} MC_VALUES;

typedef enum {
    COMM_FW_VERSION = 0,
    COMM_JUMP_TO_BOOTLOADER,
    COMM_ERASE_NEW_APP,
    COMM_WRITE_NEW_APP_DATA,
    COMM_GET_VALUES,
    COMM_SET_DUTY,
    COMM_SET_CURRENT,
    COMM_SET_CURRENT_BRAKE,
    COMM_SET_RPM,
    COMM_SET_POS,
    COMM_SET_DETECT,
    COMM_SET_SERVO_POS,
    COMM_SET_MCCONF,
    COMM_GET_MCCONF,
    COMM_SET_APPCONF,
    COMM_GET_APPCONF,
    COMM_SAMPLE_PRINT,
    COMM_TERMINAL_CMD,
    COMM_PRINT,
    COMM_ROTOR_POSITION,
    COMM_EXPERIMENT_SAMPLE,
    COMM_DETECT_MOTOR_PARAM,
    COMM_REBOOT,
    COMM_ALIVE,
    COMM_GET_DECODED_PPM,
    COMM_GET_DECODED_ADC,
    COMM_GET_DECODED_CHUK,
    COMM_FORWARD_CAN
} COMM_PACKET_ID;

typedef enum {
    COMM_MODE_INTEGRATE = 0,
    COMM_MODE_DELAY
} mc_comm_mode;

typedef enum {
    SENSOR_MODE_SENSORLESS = 0,
    SENSOR_MODE_SENSORED,
    SENSOR_MODE_HYBRID
} mc_sensor_mode;

typedef enum {
    MOTOR_TYPE_BLDC = 0,
    MOTOR_TYPE_DC,
} mc_motor_type;

typedef enum {
    PWM_MODE_NONSYNCHRONOUS_HISW = 0, // This mode is not recommended
    PWM_MODE_SYNCHRONOUS, // The recommended and most tested mode
    PWM_MODE_BIPOLAR // Some glitches occasionally, can kill MOSFETs
} mc_pwm_mode;

typedef struct {
    // Switching and drive
    mc_pwm_mode pwm_mode;
    mc_comm_mode comm_mode;
    mc_motor_type motor_type;
    mc_sensor_mode sensor_mode;
    // Limits
    float l_current_max;
    float l_current_min;
    float l_in_current_max;
    float l_in_current_min;
    float l_abs_current_max;
    float l_min_erpm;
    float l_max_erpm;
    float l_max_erpm_fbrake;
    float l_max_erpm_fbrake_cc;
    float l_min_vin;
    float l_max_vin;
    float l_battery_cut_start;
    float l_battery_cut_end;
    bool l_slow_abs_current;
    bool l_rpm_lim_neg_torque;
    float l_temp_fet_start;
    float l_temp_fet_end;
    float l_temp_motor_start;
    float l_temp_motor_end;
    float l_min_duty;
    float l_max_duty;
    // Sensorless
    float sl_min_erpm;
    float sl_min_erpm_cycle_int_limit;
    float sl_max_fullbreak_current_dir_change;
    float sl_cycle_int_limit;
    float sl_phase_advance_at_br;
    float sl_cycle_int_rpm_br;
    float sl_bemf_coupling_k;
    // Hall sensor
    int8_t hall_table[8];
    float hall_sl_erpm;
    // Speed PID
    float s_pid_kp;
    float s_pid_ki;
    float s_pid_kd;
    float s_pid_min_erpm;
    // Pos PID
    float p_pid_kp;
    float p_pid_ki;
    float p_pid_kd;
    // Current controller
    float cc_startup_boost_duty;
    float cc_min_current;
    float cc_gain;
    float cc_ramp_step_max;
    // Misc
    int32_t m_fault_stop_time_ms;
    float m_duty_ramp_step;
    float m_duty_ramp_step_rpm_lim;
    float m_current_backoff_gain;
    // Fields that are only present in Qt
    QString meta_description;
} mc_configuration;

// Applications to use
typedef enum {
    APP_NONE = 0,
    APP_PPM,
    APP_ADC,
    APP_UART,
    APP_PPM_UART,
    APP_ADC_UART,
    APP_NUNCHUK,
    APP_NRF,
    APP_CUSTOM
} app_use;

// PPM control types
typedef enum {
    PPM_CTRL_TYPE_NONE = 0,
    PPM_CTRL_TYPE_CURRENT,
    PPM_CTRL_TYPE_CURRENT_NOREV,
    PPM_CTRL_TYPE_CURRENT_NOREV_BRAKE,
    PPM_CTRL_TYPE_DUTY,
    PPM_CTRL_TYPE_DUTY_NOREV,
    PPM_CTRL_TYPE_PID,
    PPM_CTRL_TYPE_PID_NOREV
} ppm_control_type;

typedef struct {
    ppm_control_type ctrl_type;
    float pid_max_erpm;
    float hyst;
    float pulse_start;
    float pulse_end;
    bool median_filter;
    bool safe_start;
    float rpm_lim_start;
    float rpm_lim_end;
    bool multi_esc;
    bool tc;
    float tc_max_diff;
} ppm_config;

// ADC control types
typedef enum {
    ADC_CTRL_TYPE_NONE = 0,
    ADC_CTRL_TYPE_CURRENT,
    ADC_CTRL_TYPE_CURRENT_REV_CENTER,
    ADC_CTRL_TYPE_CURRENT_REV_BUTTON,
    ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_CENTER,
    ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_BUTTON,
    ADC_CTRL_TYPE_DUTY,
    ADC_CTRL_TYPE_DUTY_REV_CENTER,
    ADC_CTRL_TYPE_DUTY_REV_BUTTON
} adc_control_type;

typedef struct {
    adc_control_type ctrl_type;
    float hyst;
    float voltage_start;
    float voltage_end;
    bool use_filter;
    bool safe_start;
    bool cc_button_inverted;
    bool rev_button_inverted;
    bool voltage_inverted;
    float rpm_lim_start;
    float rpm_lim_end;
    bool multi_esc;
    bool tc;
    float tc_max_diff;
    quint32 update_rate_hz;
} adc_config;

// Nunchuk control types
typedef enum {
    CHUK_CTRL_TYPE_NONE = 0,
    CHUK_CTRL_TYPE_CURRENT,
    CHUK_CTRL_TYPE_CURRENT_NOREV
} chuk_control_type;

typedef struct {
    chuk_control_type ctrl_type;
    float hyst;
    float rpm_lim_start;
    float rpm_lim_end;
    float ramp_time_pos;
    float ramp_time_neg;
    float stick_erpm_per_s_in_cc;
    bool multi_esc;
    bool tc;
    float tc_max_diff;
} chuk_config;

typedef struct {
    // Settings
    quint8 controller_id;
    quint32 timeout_msec;
    float timeout_brake_current;
    bool send_can_status;
    quint32 send_can_status_rate_hz;

    // Application to use
    app_use app_to_use;

    // PPM application settings
    ppm_config app_ppm_conf;

    // ADC application settings
    adc_config app_adc_conf;

    // UART application settings
    quint32 app_uart_baudrate;

    // Nunchuk application settings
    chuk_config app_chuk_conf;
} app_configuration;

#endif // DATATYPES_H
