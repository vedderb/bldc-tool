#include "mcconfiguration.h"

McConfiguration::McConfiguration(QObject*)

{
    memset(&m_data, 0, sizeof(mc_configuration));
}

McConfiguration::McConfiguration(const mc_configuration &src)
{
    m_data = src;
    for (int i = 0; i < 8; ++i) {
        m_hall_table.append(src.hall_table[i]);
        m_foc_hall_table.append(src.foc_hall_table[i]);
    }
}

void McConfiguration::setData(const mc_configuration &src)
{
    // set all properties one by one to notify iff changed
    set_pwm_mode(src.pwm_mode);
    set_comm_mode(src.comm_mode);
    set_motor_type(src.motor_type);
    set_sensor_mode(src.sensor_mode);
    set_l_current_max(src.l_current_max);
    set_l_current_min(src.l_current_min);
    set_l_in_current_max(src.l_in_current_max);
    set_l_in_current_min(src.l_in_current_min);
    set_l_abs_current_max(src.l_abs_current_max);
    set_l_min_erpm(src.l_min_erpm);
    set_l_max_erpm(src.l_max_erpm);
    set_l_max_erpm_fbrake(src.l_max_erpm_fbrake);
    set_l_max_erpm_fbrake_cc(src.l_max_erpm_fbrake_cc);
    set_l_min_vin(src.l_min_vin);
    set_l_max_vin(src.l_max_vin);
    set_l_battery_cut_start(src.l_battery_cut_start);
    set_l_battery_cut_end(src.l_battery_cut_end);
    set_l_slow_abs_current(src.l_slow_abs_current);
    set_l_rpm_lim_neg_torque(src.l_rpm_lim_neg_torque);
    set_l_temp_fet_start(src.l_temp_fet_start);
    set_l_temp_fet_end(src.l_temp_fet_end);
    set_l_temp_motor_start(src.l_temp_motor_start);
    set_l_temp_motor_end(src.l_temp_motor_end);
    set_l_min_duty(src.l_min_duty);
    set_l_max_duty(src.l_max_duty);
    set_sl_min_erpm(src.sl_min_erpm);
    set_sl_min_erpm_cycle_int_limit(src.sl_min_erpm_cycle_int_limit);
    set_sl_max_fullbreak_current_dir_change(src.sl_max_fullbreak_current_dir_change);
    set_sl_cycle_int_limit(src.sl_cycle_int_limit);
    set_sl_phase_advance_at_br(src.sl_phase_advance_at_br);
    set_sl_cycle_int_rpm_br(src.sl_cycle_int_rpm_br);
    set_sl_bemf_coupling_k(src.sl_bemf_coupling_k);
    set_hall_sl_erpm(src.hall_sl_erpm);
    set_foc_current_kp(src.foc_current_kp);
    set_foc_current_ki(src.foc_current_ki);
    set_foc_f_sw(src.foc_f_sw);
    set_foc_dt_us(src.foc_dt_us);
    set_foc_encoder_offset(src.foc_encoder_offset);
    set_foc_encoder_inverted(src.foc_encoder_inverted);
    set_foc_encoder_ratio(src.foc_encoder_ratio);
    set_foc_motor_l(src.foc_motor_l);
    set_foc_motor_r(src.foc_motor_r);
    set_foc_motor_flux_linkage(src.foc_motor_flux_linkage);
    set_foc_observer_gain(src.foc_observer_gain);
    set_foc_pll_kp(src.foc_pll_kp);
    set_foc_pll_ki(src.foc_pll_ki);
    set_foc_duty_dowmramp_kp(src.foc_duty_dowmramp_kp);
    set_foc_duty_dowmramp_ki(src.foc_duty_dowmramp_ki);
    set_foc_openloop_rpm(src.foc_openloop_rpm);
    set_foc_sl_openloop_hyst(src.foc_sl_openloop_hyst);
    set_foc_sl_openloop_time(src.foc_sl_openloop_time);
    set_foc_sl_d_current_duty(src.foc_sl_d_current_duty);
    set_foc_sl_d_current_factor(src.foc_sl_d_current_factor);
    set_foc_sensor_mode(src.foc_sensor_mode);
    set_foc_hall_sl_erpm(src.foc_hall_sl_erpm);
    set_s_pid_kp(src.s_pid_kp);
    set_s_pid_ki(src.s_pid_ki);
    set_s_pid_kd(src.s_pid_kd);
    set_s_pid_min_erpm(src.s_pid_min_erpm);
    set_p_pid_kp(src.p_pid_kp);
    set_p_pid_ki(src.p_pid_ki);
    set_p_pid_kd(src.p_pid_kd);
    set_cc_startup_boost_duty(src.cc_startup_boost_duty);
    set_cc_min_current(src.cc_min_current);
    set_cc_gain(src.cc_gain);
    set_cc_ramp_step_max(src.cc_ramp_step_max);
    set_m_fault_stop_time_ms(src.m_fault_stop_time_ms);
    set_m_duty_ramp_step(src.m_duty_ramp_step);
    set_m_duty_ramp_step_rpm_lim(src.m_duty_ramp_step_rpm_lim);
    set_m_current_backoff_gain(src.m_current_backoff_gain);
    set_m_encoder_counts(src.m_encoder_counts);
    set_meta_description(src.meta_description);

    // special handling for tables
    QList<int> foc_hall_table;
    QList<int> hall_table;
    for (int i = 0; i < 8; ++i) {
        hall_table.append(src.hall_table[i]);
        foc_hall_table.append(src.foc_hall_table[i]);
    }
    if(hall_table != m_hall_table){
        m_hall_table = hall_table;
        emit hall_tableChanged(m_hall_table);
    }
    if(foc_hall_table != m_foc_hall_table){
        m_foc_hall_table = foc_hall_table;
        emit foc_hall_tableChanged(m_foc_hall_table);
    }

}

QList<int> McConfiguration::foc_hall_table() const
{
    return m_foc_hall_table;
}

QList<int> McConfiguration::hall_table() const
{
    return m_hall_table;
}

const mc_configuration &McConfiguration::data(){
    return m_data;
}

void McConfiguration::setFoc_hall_table(QList<int> foc_hall_table)
{
    if (m_foc_hall_table == foc_hall_table)
        return;

    Q_ASSERT(foc_hall_table.length() <= 8);
    for (int i = 0; i < foc_hall_table.length(); ++i) {
        m_data.foc_hall_table[i] = foc_hall_table.at(i);
    }
    m_foc_hall_table = foc_hall_table;
    emit foc_hall_tableChanged(foc_hall_table);
}

void McConfiguration::setHall_table(QList<int> hall_table)
{
    if (m_hall_table == hall_table)
        return;

    Q_ASSERT(hall_table.length() <= 8);
    for (int i = 0; i < hall_table.length(); ++i) {
        m_data.hall_table[i] = hall_table.at(i);
    }
    m_hall_table = hall_table;
    emit hall_tableChanged(hall_table);
}
