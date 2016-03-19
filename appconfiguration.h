#ifndef APPCONF_H
#define APPCONF_H

#include <QObject>
#include "datatypes.h"
#include "lib-qt-qml-tricks/src/qqmlhelpers.h"

class AppConfiguration : public QObject
{
    Q_OBJECT

public:
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
    } App_use;
    typedef enum {
        PPM_CTRL_TYPE_NONE = 0,
        PPM_CTRL_TYPE_CURRENT,
        PPM_CTRL_TYPE_CURRENT_NOREV,
        PPM_CTRL_TYPE_CURRENT_NOREV_BRAKE,
        PPM_CTRL_TYPE_DUTY,
        PPM_CTRL_TYPE_DUTY_NOREV,
        PPM_CTRL_TYPE_PID,
        PPM_CTRL_TYPE_PID_NOREV
    } PPM_control_type;
    typedef enum {
        ADC_CTRL_TYPE_NONE = 0,
        ADC_CTRL_TYPE_CURRENT,
        ADC_CTRL_TYPE_CURRENT_REV_CENTER,
        ADC_CTRL_TYPE_CURRENT_REV_BUTTON,
        ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_CENTER,
        ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_BUTTON,
        ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_ADC,
        ADC_CTRL_TYPE_DUTY,
        ADC_CTRL_TYPE_DUTY_REV_CENTER,
        ADC_CTRL_TYPE_DUTY_REV_BUTTON
    } ADC_control_type;
    typedef enum {
        CHUK_CTRL_TYPE_NONE = 0,
        CHUK_CTRL_TYPE_CURRENT,
        CHUK_CTRL_TYPE_CURRENT_NOREV
    } Chuk_control_type;
    typedef enum {
        NRF_SPEED_250K = 0,
        NRF_SPEED_1M,
        NRF_SPEED_2M
    } NRFSPEED;
    typedef enum {
        NRF_POWER_M18DBM = 0,
        NRF_POWER_M12DBM,
        NRF_POWER_M6DBM,
        NRF_POWER_0DBM
    } NRFPOWER;

    typedef enum {
        NRF_AW_3 = 0,
        NRF_AW_4,
        NRF_AW_5
    } NRFAW;
    typedef enum {
        NRF_CRC_DISABLED = 0,
        NRF_CRC_1B,
        NRF_CRC_2B
    } NRFCRC;typedef enum {
        NRF_RETR_DELAY_250US = 0,
        NRF_RETR_DELAY_500US,
        NRF_RETR_DELAY_750US,
        NRF_RETR_DELAY_1000US,
        NRF_RETR_DELAY_1250US,
        NRF_RETR_DELAY_1500US,
        NRF_RETR_DELAY_1750US,
        NRF_RETR_DELAY_2000US,
        NRF_RETR_DELAY_2250US,
        NRF_RETR_DELAY_2500US,
        NRF_RETR_DELAY_2750US,
        NRF_RETR_DELAY_3000US,
        NRF_RETR_DELAY_3250US,
        NRF_RETR_DELAY_3500US,
        NRF_RETR_DELAY_3750US,
        NRF_RETR_DELAY_4000US
    } NRFRETRDELAY;

    // Settings
    QML_WRITABLE_PROPERTY_WRAPPER( int 	,controller_id 			,m_data.controller_id, quint8)
    QML_WRITABLE_PROPERTY_WRAPPER( int 	,timeout_msec 			,m_data.timeout_msec, quint32)
    QML_WRITABLE_PROPERTY_W( float 		,timeout_brake_current 	,m_data.timeout_brake_current 	)
    QML_WRITABLE_PROPERTY_W( bool 		,send_can_status 		,m_data.send_can_status 		)
    QML_WRITABLE_PROPERTY_WRAPPER( int 	,send_can_status_rate_hz,m_data.send_can_status_rate_hz, quint32)
    // Application to use
    QML_WRITABLE_ENUM_PROPERTY_W( App_use ,app_to_use, app_use, m_data.app_to_use )
    // PPM application settings
    QML_WRITABLE_ENUM_PROPERTY_W( PPM_control_type, ppm_ctrl_type, ppm_control_type, m_data.app_ppm_conf.ctrl_type)
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
    QML_WRITABLE_ENUM_PROPERTY_W( ADC_control_type ,adc_ctrl_type,adc_control_type, m_data.app_adc_conf.ctrl_type)
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
    QML_WRITABLE_PROPERTY_WRAPPER( int 			,adc_update_rate_hz 	,m_data.app_adc_conf.update_rate_hz, quint32)
    // UART application settings
    QML_WRITABLE_PROPERTY_WRAPPER( int ,app_uart_baudrate	,m_data.app_uart_baudrate, quint32)
    // Nunchuk application settings
    QML_WRITABLE_ENUM_PROPERTY_W( Chuk_control_type 	,chuk_ctrl_type, chuk_control_type,m_data.app_chuk_conf.ctrl_type 			)
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
    QML_WRITABLE_ENUM_PROPERTY_W( NRFSPEED 		,nrf_speed		, NRF_SPEED 	,m_data.app_nrf_conf.speed		)
    QML_WRITABLE_ENUM_PROPERTY_W( NRFPOWER 		,nrf_power		, NRF_POWER 	,m_data.app_nrf_conf.power		)
    QML_WRITABLE_ENUM_PROPERTY_W( NRFCRC        ,nrf_crc_type	, NRF_CRC       ,m_data.app_nrf_conf.crc_type	)
    QML_WRITABLE_ENUM_PROPERTY_W( NRFRETRDELAY  ,nrf_retry_delay, NRF_RETR_DELAY,m_data.app_nrf_conf.retry_delay)
    QML_WRITABLE_PROPERTY_WRAPPER( char ,nrf_retries		,m_data.app_nrf_conf.retries   , unsigned char )
    QML_WRITABLE_PROPERTY_WRAPPER( char ,nrf_channel		,m_data.app_nrf_conf.channel   , unsigned char )
    QML_WRITABLE_PROPERTY_WRAPPER( char ,nrf_address0		,m_data.app_nrf_conf.address[0], unsigned char	)
    QML_WRITABLE_PROPERTY_WRAPPER( char ,nrf_address1		,m_data.app_nrf_conf.address[1], unsigned char	)
    QML_WRITABLE_PROPERTY_WRAPPER( char 	,nrf_address2		,m_data.app_nrf_conf.address[2], unsigned char	)
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
