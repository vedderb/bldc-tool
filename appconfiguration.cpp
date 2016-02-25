#include "appconfiguration.h"

AppConfiguration::AppConfiguration(QObject *parent) : QObject(parent)
{

}

AppConfiguration::AppConfiguration(app_configuration &data)
{
    m_data = data;
}

void AppConfiguration::setData(app_configuration &src)
{
    // set all properties one by one to notify iff changed
    set_controller_id 				(src.controller_id 			);
    set_timeout_msec 				(src.timeout_msec 			);
    set_timeout_brake_current 		(src.timeout_brake_current 	);
    set_send_can_status 			(src.send_can_status 		);
    set_send_can_status_rate_hz		(src.send_can_status_rate_hz);
    set_app_to_use					(src.app_to_use );
    set_ppm_ctrl_type 				(src.app_ppm_conf.ctrl_type 		);
    set_ppm_pid_max_erpm 			(src.app_ppm_conf.pid_max_erpm 	);
    set_ppm_hyst 					(src.app_ppm_conf.hyst 			);
    set_ppm_pulse_start 			(src.app_ppm_conf.pulse_start 	);
    set_ppm_pulse_end 				(src.app_ppm_conf.pulse_end 		);
    set_ppm_median_filter 			(src.app_ppm_conf.median_filter 	);
    set_ppm_safe_start 				(src.app_ppm_conf.safe_start 	);
    set_ppm_rpm_lim_start 			(src.app_ppm_conf.rpm_lim_start 	);
    set_ppm_rpm_lim_end 			(src.app_ppm_conf.rpm_lim_end 	);
    set_ppm_multi_esc 				(src.app_ppm_conf.multi_esc 		);
    set_ppm_tc 						(src.app_ppm_conf.tc 			);
    set_ppm_tc_max_diff 			(src.app_ppm_conf.tc_max_diff 	);
    set_adc_ctrl_type 				(src.app_adc_conf.ctrl_type 			);
    set_adc_hyst 					(src.app_adc_conf.hyst 				);
    set_adc_voltage_start 			(src.app_adc_conf.voltage_start 		);
    set_adc_voltage_end 			(src.app_adc_conf.voltage_end 		);
    set_adc_use_filter 				(src.app_adc_conf.use_filter 		);
    set_adc_safe_start 				(src.app_adc_conf.safe_start 		);
    set_adc_cc_button_inverted		(src.app_adc_conf.cc_button_inverted	);
    set_adc_rev_button_inverted		(src.app_adc_conf.rev_button_inverted);
    set_adc_voltage_inverted 		(src.app_adc_conf.voltage_inverted 	);
    set_adc_rpm_lim_start 			(src.app_adc_conf.rpm_lim_start 		);
    set_adc_rpm_lim_end 			(src.app_adc_conf.rpm_lim_end 		);
    set_adc_multi_esc 				(src.app_adc_conf.multi_esc 			);
    set_adc_tc 						(src.app_adc_conf.tc 				);
    set_adc_tc_max_diff 			(src.app_adc_conf.tc_max_diff 		);
    set_adc_update_rate_hz 			(src.app_adc_conf.update_rate_hz 	);
    set_app_uart_baudrate			(src.app_uart_baudrate);
    set_chuk_ctrl_type 				(src.app_chuk_conf.ctrl_type 			);
    set_chuk_hyst 					(src.app_chuk_conf.hyst 					);
    set_chuk_rpm_lim_start 			(src.app_chuk_conf.rpm_lim_start 		);
    set_chuk_rpm_lim_end 			(src.app_chuk_conf.rpm_lim_end 			);
    set_chuk_ramp_time_pos 			(src.app_chuk_conf.ramp_time_pos 		);
    set_chuk_ramp_time_neg			(src.app_chuk_conf.ramp_time_neg			);
    set_chuk_stick_erpm_per_s_in_cc	(src.app_chuk_conf.stick_erpm_per_s_in_cc);
    set_chuk_multi_esc				(src.app_chuk_conf.multi_esc				);
    set_chuk_tc						(src.app_chuk_conf.tc					);
    set_chuk_tc_max_diff			(src.app_chuk_conf.tc_max_diff			);
    set_nrf_speed					(src.app_nrf_conf.speed		);
    set_nrf_power					(src.app_nrf_conf.power		);
    set_nrf_crc_type				(src.app_nrf_conf.crc_type	);
    set_nrf_retry_delay				(src.app_nrf_conf.retry_delay);
    set_nrf_retries					(src.app_nrf_conf.retries	);
    set_nrf_channel					(src.app_nrf_conf.channel	);
    set_nrf_address0				(src.app_nrf_conf.address[0]	);
    set_nrf_address1				(src.app_nrf_conf.address[1]	);
    set_nrf_address2				(src.app_nrf_conf.address[2]	);
    set_nrf_send_crc_ack			(src.app_nrf_conf.send_crc_ack);
}

