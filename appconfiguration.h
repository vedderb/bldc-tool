#ifndef APPCONF_H
#define APPCONF_H

#include <QObject>
#include "datatypes.h"
#include "lib-qt-qml-tricks/src/qqmlhelpers.h"

class AppConfiguration : public QObject
{
    Q_OBJECT
    Q_ENUMS(app_use)
    Q_ENUMS(ppm_control_type)
    Q_ENUMS(adc_control_type)
    Q_ENUMS(chuk_control_type)
    Q_ENUMS(NRF_SPEED 	   )
    Q_ENUMS(NRF_POWER 	   )
    Q_ENUMS(NRF_CRC 	   )
    Q_ENUMS(NRF_RETR_DELAY )
    // Settings
    QML_WRITABLE_PROPERTY_W( int 	,controller_id 			,m_data.controller_id 			)
    QML_WRITABLE_PROPERTY_W( int 	,timeout_msec 			,m_data.timeout_msec 			)
    QML_WRITABLE_PROPERTY_W( float 		,timeout_brake_current 	,m_data.timeout_brake_current 	)
    QML_WRITABLE_PROPERTY_W( bool 		,send_can_status 		,m_data.send_can_status 		)
    QML_WRITABLE_PROPERTY_W( int 	,send_can_status_rate_hz,m_data.send_can_status_rate_hz)
    // Application to use
    QML_WRITABLE_PROPERTY_W( app_use ,app_to_use, m_data.app_to_use )
    // PPM application settings
    QML_WRITABLE_PROPERTY_W( ppm_control_type 	,ppm_ctrl_type 		,m_data.app_ppm_conf.ctrl_type 		)
    QML_WRITABLE_PROPERTY_W( float 				,ppm_pid_max_erpm 	,m_data.app_ppm_conf.pid_max_erpm 	)
    QML_WRITABLE_PROPERTY_W( float 				,ppm_hyst 			,m_data.app_ppm_conf.hyst 			)
    QML_WRITABLE_PROPERTY_W( float 				,ppm_pulse_start 	,m_data.app_ppm_conf.pulse_start 	)
    QML_WRITABLE_PROPERTY_W( float 				,ppm_pulse_end 		,m_data.app_ppm_conf.pulse_end 		)
    QML_WRITABLE_PROPERTY_W( bool 				,ppm_median_filter 	,m_data.app_ppm_conf.median_filter 	)
    QML_WRITABLE_PROPERTY_W( bool 				,ppm_safe_start 	,m_data.app_ppm_conf.safe_start 	)
    QML_WRITABLE_PROPERTY_W( float 				,ppm_rpm_lim_start 	,m_data.app_ppm_conf.rpm_lim_start 	)
    QML_WRITABLE_PROPERTY_W( float 				,ppm_rpm_lim_end 	,m_data.app_ppm_conf.rpm_lim_end 	)
    QML_WRITABLE_PROPERTY_W( bool 				,ppm_multi_esc 		,m_data.app_ppm_conf.multi_esc 		)
    QML_WRITABLE_PROPERTY_W( bool 				,ppm_tc 			,m_data.app_ppm_conf.tc 			)
    QML_WRITABLE_PROPERTY_W( float				,ppm_tc_max_diff 	,m_data.app_ppm_conf.tc_max_diff 	)
    // ADC application settings
    QML_WRITABLE_PROPERTY_W( adc_control_type 	,adc_ctrl_type 			,m_data.app_adc_conf.ctrl_type 			)
    QML_WRITABLE_PROPERTY_W( float				,adc_hyst 				,m_data.app_adc_conf.hyst 				)
    QML_WRITABLE_PROPERTY_W( float				,adc_voltage_start 		,m_data.app_adc_conf.voltage_start 		)
    QML_WRITABLE_PROPERTY_W( float				,adc_voltage_end 		,m_data.app_adc_conf.voltage_end 		)
    QML_WRITABLE_PROPERTY_W( bool 				,adc_use_filter 		,m_data.app_adc_conf.use_filter 		)
    QML_WRITABLE_PROPERTY_W( bool 				,adc_safe_start 		,m_data.app_adc_conf.safe_start 		)
    QML_WRITABLE_PROPERTY_W( bool 				,adc_cc_button_inverted	,m_data.app_adc_conf.cc_button_inverted	)
    QML_WRITABLE_PROPERTY_W( bool 				,adc_rev_button_inverted,m_data.app_adc_conf.rev_button_inverted)
    QML_WRITABLE_PROPERTY_W( bool 				,adc_voltage_inverted 	,m_data.app_adc_conf.voltage_inverted 	)
    QML_WRITABLE_PROPERTY_W( float				,adc_rpm_lim_start 		,m_data.app_adc_conf.rpm_lim_start 		)
    QML_WRITABLE_PROPERTY_W( float				,adc_rpm_lim_end 		,m_data.app_adc_conf.rpm_lim_end 		)
    QML_WRITABLE_PROPERTY_W( bool 				,adc_multi_esc 			,m_data.app_adc_conf.multi_esc 			)
    QML_WRITABLE_PROPERTY_W( bool 				,adc_tc 				,m_data.app_adc_conf.tc 				)
    QML_WRITABLE_PROPERTY_W( float				,adc_tc_max_diff 		,m_data.app_adc_conf.tc_max_diff 		)
    QML_WRITABLE_PROPERTY_W( int 			,adc_update_rate_hz 	,m_data.app_adc_conf.update_rate_hz 	)
    // UART application settings
    QML_WRITABLE_PROPERTY_W( int ,app_uart_baudrate	,m_data.app_uart_baudrate)
    // Nunchuk application settings
    QML_WRITABLE_PROPERTY_W( chuk_control_type 	,chuk_ctrl_type 			,m_data.app_chuk_conf.ctrl_type 			)
    QML_WRITABLE_PROPERTY_W( float				,chuk_hyst 					,m_data.app_chuk_conf.hyst 					)
    QML_WRITABLE_PROPERTY_W( float				,chuk_rpm_lim_start 		,m_data.app_chuk_conf.rpm_lim_start 		)
    QML_WRITABLE_PROPERTY_W( float				,chuk_rpm_lim_end 			,m_data.app_chuk_conf.rpm_lim_end 			)
    QML_WRITABLE_PROPERTY_W( float				,chuk_ramp_time_pos 		,m_data.app_chuk_conf.ramp_time_pos 		)
    QML_WRITABLE_PROPERTY_W( float				,chuk_ramp_time_neg			,m_data.app_chuk_conf.ramp_time_neg			)
    QML_WRITABLE_PROPERTY_W( float				,chuk_stick_erpm_per_s_in_cc,m_data.app_chuk_conf.stick_erpm_per_s_in_cc)
    QML_WRITABLE_PROPERTY_W( bool 				,chuk_multi_esc				,m_data.app_chuk_conf.multi_esc				)
    QML_WRITABLE_PROPERTY_W( bool 				,chuk_tc					,m_data.app_chuk_conf.tc					)
    QML_WRITABLE_PROPERTY_W( float				,chuk_tc_max_diff			,m_data.app_chuk_conf.tc_max_diff			)
    // NRF application settings
    QML_WRITABLE_PROPERTY_W( NRF_SPEED 		,nrf_speed			,m_data.app_nrf_conf.speed		)
    QML_WRITABLE_PROPERTY_W( NRF_POWER 		,nrf_power			,m_data.app_nrf_conf.power		)
    QML_WRITABLE_PROPERTY_W( NRF_CRC 		,nrf_crc_type		,m_data.app_nrf_conf.crc_type	)
    QML_WRITABLE_PROPERTY_W( NRF_RETR_DELAY ,nrf_retry_delay	,m_data.app_nrf_conf.retry_delay)
    QML_WRITABLE_PROPERTY_W( unsigned char 	,nrf_retries		,m_data.app_nrf_conf.retries	)
    QML_WRITABLE_PROPERTY_W( unsigned char 	,nrf_channel		,m_data.app_nrf_conf.channel	)
    QML_WRITABLE_PROPERTY_W( unsigned char 	,nrf_address0		,m_data.app_nrf_conf.address[0]	)
    QML_WRITABLE_PROPERTY_W( unsigned char 	,nrf_address1		,m_data.app_nrf_conf.address[1]	)
    QML_WRITABLE_PROPERTY_W( unsigned char 	,nrf_address2		,m_data.app_nrf_conf.address[2]	)
    QML_WRITABLE_PROPERTY_W( bool 			,nrf_send_crc_ack	,m_data.app_nrf_conf.send_crc_ack)

public:
    explicit AppConfiguration(QObject *parent = 0);
    AppConfiguration(app_configuration &data);
    void setData(app_configuration& src);
    const app_configuration& data();

signals:

public slots:

private:
    app_configuration m_data;
};

#endif // APPCONF_H
