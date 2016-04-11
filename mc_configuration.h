#ifndef MC_CONFIGURATION_H
#define MC_CONFIGURATION_H

#include "lib-qt-qml-tricks/src/qqmlhelpers.h"
#include "datatypes.h"
#include <QObject>


class MC_Configuration : public QObject
{
    Q_OBJECT

    Q_ENUMS(mc_pwm_mode   )
    Q_ENUMS(mc_comm_mode  )
    Q_ENUMS(mc_motor_type )
    Q_ENUMS(mc_sensor_mode)
    Q_ENUMS(mc_foc_sensor_mode)

    // Switching and drive
    QML_WRITABLE_PROPERTY( mc_pwm_mode 			,pwm_mode)
    QML_WRITABLE_PROPERTY( mc_comm_mode 		,comm_mode)
    QML_WRITABLE_PROPERTY( mc_motor_type 		,motor_type)
    QML_WRITABLE_PROPERTY( mc_sensor_mode 		,sensor_mode)
    // Limits
    QML_WRITABLE_PROPERTY( float 				,l_current_max)
    QML_WRITABLE_PROPERTY( float 				,l_current_min)
    QML_WRITABLE_PROPERTY( float 				,l_in_current_max)
    QML_WRITABLE_PROPERTY( float 				,l_in_current_min)
    QML_WRITABLE_PROPERTY( float 				,l_abs_current_max)
    QML_WRITABLE_PROPERTY( float 				,l_min_erpm)
    QML_WRITABLE_PROPERTY( float 				,l_max_erpm)
    QML_WRITABLE_PROPERTY( float 				,l_max_erpm_fbrake)
    QML_WRITABLE_PROPERTY( float 				,l_max_erpm_fbrake_cc)
    QML_WRITABLE_PROPERTY( float 				,l_min_vin)
    QML_WRITABLE_PROPERTY( float 				,l_max_vin)
    QML_WRITABLE_PROPERTY( float 				,l_battery_cut_start)
    QML_WRITABLE_PROPERTY( float 				,l_battery_cut_end)
    QML_WRITABLE_PROPERTY( bool 				,l_slow_abs_current)
    QML_WRITABLE_PROPERTY( bool 				,l_rpm_lim_neg_torque)
    QML_WRITABLE_PROPERTY( float 				,l_temp_fet_start)
    QML_WRITABLE_PROPERTY( float 				,l_temp_fet_end)
    QML_WRITABLE_PROPERTY( float 				,l_temp_motor_start)
    QML_WRITABLE_PROPERTY( float 				,l_temp_motor_end)
    QML_WRITABLE_PROPERTY( float 				,l_min_duty)
    QML_WRITABLE_PROPERTY( float 				,l_max_duty)
    // Sensorless
    QML_WRITABLE_PROPERTY( float 				,sl_min_erpm)
    QML_WRITABLE_PROPERTY( float 				,sl_min_erpm_cycle_int_limit)
    QML_WRITABLE_PROPERTY( float 				,sl_max_fullbreak_current_dir_change)
    QML_WRITABLE_PROPERTY( float 				,sl_cycle_int_limit)
    QML_WRITABLE_PROPERTY( float 				,sl_phase_advance_at_br)
    QML_WRITABLE_PROPERTY( float 				,sl_cycle_int_rpm_br)
    QML_WRITABLE_PROPERTY( float 				,sl_bemf_coupling_k)
    // Hall sensor
    QML_WRITABLE_PROPERTY( QByteArray			,hall_table)

    QML_WRITABLE_PROPERTY( float 				,hall_sl_erpm)
    // FOC
    QML_WRITABLE_PROPERTY( float 				,foc_current_kp)
    QML_WRITABLE_PROPERTY( float 				,foc_current_ki)
    QML_WRITABLE_PROPERTY( float 				,foc_f_sw)
    QML_WRITABLE_PROPERTY( float 				,foc_dt_us)
    QML_WRITABLE_PROPERTY( float 				,foc_encoder_offset)
    QML_WRITABLE_PROPERTY( bool 				,foc_encoder_inverted)
    QML_WRITABLE_PROPERTY( float 				,foc_encoder_ratio)
    QML_WRITABLE_PROPERTY( float 				,foc_motor_l)
    QML_WRITABLE_PROPERTY( float 				,foc_motor_r)
    QML_WRITABLE_PROPERTY( float 				,foc_motor_flux_linkage)
    QML_WRITABLE_PROPERTY( float 				,foc_observer_gain)
    QML_WRITABLE_PROPERTY( float 				,foc_pll_kp)
    QML_WRITABLE_PROPERTY( float 				,foc_pll_ki)
    QML_WRITABLE_PROPERTY( float 				,foc_duty_dowmramp_kp)
    QML_WRITABLE_PROPERTY( float 				,foc_duty_dowmramp_ki)
    QML_WRITABLE_PROPERTY( float 				,foc_openloop_rpm)
    QML_WRITABLE_PROPERTY( float 				,foc_sl_openloop_hyst)
    QML_WRITABLE_PROPERTY( float 				,foc_sl_openloop_time)
    QML_WRITABLE_PROPERTY( float 				,foc_sl_d_current_duty)
    QML_WRITABLE_PROPERTY( float 				,foc_sl_d_current_factor)
    QML_WRITABLE_PROPERTY( mc_foc_sensor_mode 	,foc_sensor_mode)
    QML_WRITABLE_PROPERTY( QByteArray 			,foc_hall_table)
    QML_WRITABLE_PROPERTY( float 				,foc_hall_sl_erpm)
    // Speed PID
    QML_WRITABLE_PROPERTY( float 				,s_pid_kp)
    QML_WRITABLE_PROPERTY( float 				,s_pid_ki)
    QML_WRITABLE_PROPERTY( float 				,s_pid_kd)
    QML_WRITABLE_PROPERTY( float 				,s_pid_min_erpm)
    // Pos PID
    QML_WRITABLE_PROPERTY( float 				,p_pid_kp)
    QML_WRITABLE_PROPERTY( float 				,p_pid_ki)
    QML_WRITABLE_PROPERTY( float 				,p_pid_kd)
    // Current controller
    QML_WRITABLE_PROPERTY( float 				,cc_startup_boost_duty)
    QML_WRITABLE_PROPERTY( float 				,cc_min_current)
    QML_WRITABLE_PROPERTY( float 				,cc_gain)
    QML_WRITABLE_PROPERTY( float 				,cc_ramp_step_max)
    // Misc
    QML_WRITABLE_PROPERTY( int32_t 				,m_fault_stop_time_ms)
    QML_WRITABLE_PROPERTY( float 				,m_duty_ramp_step)
    QML_WRITABLE_PROPERTY( float 				,m_duty_ramp_step_rpm_lim)
    QML_WRITABLE_PROPERTY( float 				,m_current_backoff_gain)
    QML_WRITABLE_PROPERTY( uint32_t 			,m_encoder_counts)
    // Fields that are only present in Qt
    QML_WRITABLE_PROPERTY( QString 				,meta_description)

public:

    explicit MC_Configuration(QObject *parent = 0);

signals:

public slots:
};

#endif // MC_CONFIGURATION_H
