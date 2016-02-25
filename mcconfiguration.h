#ifndef DATAWRAPPERS_H
#define MCCONFIGURATION_H

#include <QObject>
#include <QList>
#include "datatypes.h"
#include "lib-qt-qml-tricks/src/qqmlhelpers.h"

class McConfiguration : public QObject
{
    Q_OBJECT

    Q_ENUMS(mc_pwm_mode   )
    Q_ENUMS(mc_comm_mode  )
    Q_ENUMS(mc_motor_type )
    Q_ENUMS(mc_sensor_mode)
    Q_ENUMS(mc_foc_sensor_mode)

    // Switching and drive
    QML_WRITABLE_PROPERTY_W( mc_pwm_mode    ,pwm_mode    ,m_data.pwm_mode    )
    QML_WRITABLE_PROPERTY_W( mc_comm_mode  	,comm_mode   ,m_data.comm_mode   )
    QML_WRITABLE_PROPERTY_W( mc_motor_type 	,motor_type  ,m_data.motor_type  )
    QML_WRITABLE_PROPERTY_W( mc_sensor_mode	,sensor_mode ,m_data.sensor_mode )
    // Limits
    QML_WRITABLE_PROPERTY_W( float	,l_current_max          ,m_data.l_current_max        )
    QML_WRITABLE_PROPERTY_W( float	,l_current_min          ,m_data.l_current_min        )
    QML_WRITABLE_PROPERTY_W( float	,l_in_current_max       ,m_data.l_in_current_max     )
    QML_WRITABLE_PROPERTY_W( float	,l_in_current_min       ,m_data.l_in_current_min     )
    QML_WRITABLE_PROPERTY_W( float	,l_abs_current_max      ,m_data.l_abs_current_max    )
    QML_WRITABLE_PROPERTY_W( float	,l_min_erpm             ,m_data.l_min_erpm           )
    QML_WRITABLE_PROPERTY_W( float	,l_max_erpm             ,m_data.l_max_erpm           )
    QML_WRITABLE_PROPERTY_W( float	,l_max_erpm_fbrake      ,m_data.l_max_erpm_fbrake    )
    QML_WRITABLE_PROPERTY_W( float	,l_max_erpm_fbrake_cc   ,m_data.l_max_erpm_fbrake_cc )
    QML_WRITABLE_PROPERTY_W( float	,l_min_vin              ,m_data.l_min_vin            )
    QML_WRITABLE_PROPERTY_W( float	,l_max_vin              ,m_data.l_max_vin            )
    QML_WRITABLE_PROPERTY_W( float	,l_battery_cut_start    ,m_data.l_battery_cut_start  )
    QML_WRITABLE_PROPERTY_W( float	,l_battery_cut_end      ,m_data.l_battery_cut_end    )
    QML_WRITABLE_PROPERTY_W( bool 	,l_slow_abs_current     ,m_data.l_slow_abs_current   )
    QML_WRITABLE_PROPERTY_W( bool 	,l_rpm_lim_neg_torque   ,m_data.l_rpm_lim_neg_torque )
    QML_WRITABLE_PROPERTY_W( float	,l_temp_fet_start       ,m_data.l_temp_fet_start     )
    QML_WRITABLE_PROPERTY_W( float	,l_temp_fet_end         ,m_data.l_temp_fet_end       )
    QML_WRITABLE_PROPERTY_W( float	,l_temp_motor_start     ,m_data.l_temp_motor_start   )
    QML_WRITABLE_PROPERTY_W( float	,l_temp_motor_end       ,m_data.l_temp_motor_end     )
    QML_WRITABLE_PROPERTY_W( float	,l_min_duty             ,m_data.l_min_duty           )
    QML_WRITABLE_PROPERTY_W( float	,l_max_duty             ,m_data.l_max_duty           )
    // Sensorless
    QML_WRITABLE_PROPERTY_W( float	,sl_min_erpm                           ,m_data.sl_min_erpm                         )
    QML_WRITABLE_PROPERTY_W( float	,sl_min_erpm_cycle_int_limit           ,m_data.sl_min_erpm_cycle_int_limit         )
    QML_WRITABLE_PROPERTY_W( float	,sl_max_fullbreak_current_dir_change   ,m_data.sl_max_fullbreak_current_dir_change )
    QML_WRITABLE_PROPERTY_W( float	,sl_cycle_int_limit                    ,m_data.sl_cycle_int_limit                  )
    QML_WRITABLE_PROPERTY_W( float	,sl_phase_advance_at_br                ,m_data.sl_phase_advance_at_br              )
    QML_WRITABLE_PROPERTY_W( float	,sl_cycle_int_rpm_br                   ,m_data.sl_cycle_int_rpm_br                 )
    QML_WRITABLE_PROPERTY_W( float	,sl_bemf_coupling_k                    ,m_data.sl_bemf_coupling_k                  )
    // Hall sensor
    Q_PROPERTY(QList<int> hall_table READ hall_table WRITE setHall_table NOTIFY hall_tableChanged)
    QML_WRITABLE_PROPERTY_W( float 		,hall_sl_erpm   ,m_data.hall_sl_erpm)
    // FOC
    QML_WRITABLE_PROPERTY_W( float 				,foc_current_kp 		,m_data.foc_current_kp 		 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_current_ki 		,m_data.foc_current_ki 		 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_f_sw       		,m_data.foc_f_sw       		 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_dt_us				,m_data.foc_dt_us				 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_encoder_offset		,m_data.foc_encoder_offset		 )
    QML_WRITABLE_PROPERTY_W( bool 				,foc_encoder_inverted	,m_data.foc_encoder_inverted	 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_encoder_ratio		,m_data.foc_encoder_ratio		 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_motor_l			,m_data.foc_motor_l			 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_motor_r			,m_data.foc_motor_r			 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_motor_flux_linkage	,m_data.foc_motor_flux_linkage	 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_observer_gain		,m_data.foc_observer_gain		 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_pll_kp				,m_data.foc_pll_kp				 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_pll_ki				,m_data.foc_pll_ki				 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_duty_dowmramp_kp	,m_data.foc_duty_dowmramp_kp	 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_duty_dowmramp_ki	,m_data.foc_duty_dowmramp_ki	 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_openloop_rpm		,m_data.foc_openloop_rpm		 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_sl_openloop_hyst	,m_data.foc_sl_openloop_hyst	 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_sl_openloop_time	,m_data.foc_sl_openloop_time	 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_sl_d_current_duty	,m_data.foc_sl_d_current_duty	 )
    QML_WRITABLE_PROPERTY_W( float 				,foc_sl_d_current_factor,m_data.foc_sl_d_current_factor )
    QML_WRITABLE_PROPERTY_W( mc_foc_sensor_mode ,foc_sensor_mode		,m_data.foc_sensor_mode		 )
    Q_PROPERTY(QList<int> foc_hall_table READ foc_hall_table WRITE setFoc_hall_table NOTIFY foc_hall_tableChanged)
    QML_WRITABLE_PROPERTY_W( float 				,foc_hall_sl_erpm		,m_data.foc_hall_sl_erpm )
    // Speed PID
    QML_WRITABLE_PROPERTY_W( float	,s_pid_kp		,m_data.s_pid_kp		 )
    QML_WRITABLE_PROPERTY_W( float	,s_pid_ki		,m_data.s_pid_ki		 )
    QML_WRITABLE_PROPERTY_W( float	,s_pid_kd		,m_data.s_pid_kd		 )
    QML_WRITABLE_PROPERTY_W( float	,s_pid_min_erpm	,m_data.s_pid_min_erpm	 )
    // Pos PID
    QML_WRITABLE_PROPERTY_W( float	,p_pid_kp	,m_data.p_pid_kp )
    QML_WRITABLE_PROPERTY_W( float	,p_pid_ki	,m_data.p_pid_ki )
    QML_WRITABLE_PROPERTY_W( float	,p_pid_kd	,m_data.p_pid_kd )
    // Current controller
    QML_WRITABLE_PROPERTY_W( float	,cc_startup_boost_duty		,m_data.cc_startup_boost_duty	 )
    QML_WRITABLE_PROPERTY_W( float	,cc_min_current				,m_data.cc_min_current			 )
    QML_WRITABLE_PROPERTY_W( float	,cc_gain					,m_data.cc_gain				 )
    QML_WRITABLE_PROPERTY_W( float	,cc_ramp_step_max			,m_data.cc_ramp_step_max		 )
    // Misc
    QML_WRITABLE_PROPERTY_W( uint32_t 	,m_fault_stop_time_ms       ,m_data.m_fault_stop_time_ms     )
    QML_WRITABLE_PROPERTY_W( float 		,m_duty_ramp_step			,m_data.m_duty_ramp_step		 )
    QML_WRITABLE_PROPERTY_W( float 		,m_duty_ramp_step_rpm_lim	,m_data.m_duty_ramp_step_rpm_lim )
    QML_WRITABLE_PROPERTY_W( float 		,m_current_backoff_gain		,m_data.m_current_backoff_gain	 )
    QML_WRITABLE_PROPERTY_W( uint32_t	,m_encoder_counts           ,m_data.m_encoder_counts         )
    // Fields that are only present in Qt
    QML_WRITABLE_PROPERTY_W( QString	,meta_description   ,m_data.meta_description )

public:
    explicit McConfiguration(QObject *parent = 0);
    McConfiguration(const mc_configuration &src);

    void setData(const mc_configuration &src);

    QList<int> foc_hall_table() const;
    QList<int> hall_table() const;

public slots:
    const mc_configuration& data();

    void setFoc_hall_table(QList<int> foc_hall_table);
    void setHall_table(QList<int> hall_table);

signals:

    void foc_hall_tableChanged(QList<int> foc_hall_table);

    void hall_tableChanged(QList<int> hall_table);

private:
    mc_configuration m_data;

    QList<int> m_foc_hall_table;
    QList<int> m_hall_table;
};

#endif // MCCONFIGURATION_H
