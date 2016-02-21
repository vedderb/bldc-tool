/*
    Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

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

#include "serialization.h"
#include <QFileDialog>
#include <qxmlstream.h>
#include <QFile>
#include <QDebug>
#include <QByteArray>

Serialization::Serialization(QObject *parent) :
    QObject(parent)
{
}

bool Serialization::writeMcconfXml(const MC_Configuration &mcconf, QWidget *parent)
{
    QString filename = QFileDialog::getSaveFileName(parent,
                                            tr("Save Configuration"), ".",
                                            tr("Xml files (*.xml)"));

    if (!filename.toLower().endsWith(".xml")) {
        filename.append(".xml");
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QXmlStreamWriter xmlwriter(&file);
    xmlwriter.setAutoFormatting(true);
    xmlwriter.writeStartDocument();

    xmlwriter.writeStartElement("MCConfiguration");
    xmlwriter.writeTextElement("pwm_mode", QString::number(mcconf.get_pwm_mode()));
    xmlwriter.writeTextElement("comm_mode", QString::number(mcconf.get_comm_mode()));
    xmlwriter.writeTextElement("motor_type", QString::number(mcconf.get_motor_type()));
    xmlwriter.writeTextElement("sensor_mode", QString::number(mcconf.get_sensor_mode()));
    xmlwriter.writeTextElement("l_current_max", QString::number(mcconf.get_l_current_max()));
    xmlwriter.writeTextElement("l_current_min", QString::number(mcconf.get_l_current_min()));
    xmlwriter.writeTextElement("l_in_current_max", QString::number(mcconf.get_l_in_current_max()));
    xmlwriter.writeTextElement("l_in_current_min", QString::number(mcconf.get_l_in_current_min()));
    xmlwriter.writeTextElement("l_abs_current_max", QString::number(mcconf.get_l_abs_current_max()));
    xmlwriter.writeTextElement("l_min_erpm", QString::number(mcconf.get_l_min_erpm()));
    xmlwriter.writeTextElement("l_max_erpm", QString::number(mcconf.get_l_max_erpm()));
    xmlwriter.writeTextElement("l_max_erpm_fbrake", QString::number(mcconf.get_l_max_erpm_fbrake()));
    xmlwriter.writeTextElement("l_max_erpm_fbrake_cc", QString::number(mcconf.get_l_max_erpm_fbrake_cc()));
    xmlwriter.writeTextElement("l_min_vin", QString::number(mcconf.get_l_min_vin()));
    xmlwriter.writeTextElement("l_max_vin", QString::number(mcconf.get_l_max_vin()));
    xmlwriter.writeTextElement("l_battery_cut_start", QString::number(mcconf.get_l_battery_cut_start()));
    xmlwriter.writeTextElement("l_battery_cut_end", QString::number(mcconf.get_l_battery_cut_end()));
    xmlwriter.writeTextElement("l_slow_abs_current", QString::number(mcconf.get_l_slow_abs_current()));
    xmlwriter.writeTextElement("l_rpm_lim_neg_torque", QString::number(mcconf.get_l_rpm_lim_neg_torque()));
    xmlwriter.writeTextElement("l_temp_fet_start", QString::number(mcconf.get_l_temp_fet_start()));
    xmlwriter.writeTextElement("l_temp_fet_end", QString::number(mcconf.get_l_temp_fet_end()));
    xmlwriter.writeTextElement("l_temp_motor_start", QString::number(mcconf.get_l_temp_motor_start()));
    xmlwriter.writeTextElement("l_temp_motor_end", QString::number(mcconf.get_l_temp_motor_end()));
    xmlwriter.writeTextElement("l_min_duty", QString::number(mcconf.get_l_min_duty()));
    xmlwriter.writeTextElement("l_max_duty", QString::number(mcconf.get_l_max_duty()));
    xmlwriter.writeTextElement("sl_min_erpm", QString::number(mcconf.get_sl_min_erpm()));
    xmlwriter.writeTextElement("sl_min_erpm_cycle_int_limit", QString::number(mcconf.get_sl_min_erpm_cycle_int_limit()));
    xmlwriter.writeTextElement("sl_max_fullbreak_current_dir_change", QString::number(mcconf.get_sl_max_fullbreak_current_dir_change()));
    xmlwriter.writeTextElement("sl_cycle_int_limit", QString::number(mcconf.get_sl_cycle_int_limit()));
    xmlwriter.writeTextElement("sl_cycle_int_limit_high_fac", QString::number(mcconf.get_sl_phase_advance_at_br()));
    xmlwriter.writeTextElement("sl_cycle_int_rpm_br", QString::number(mcconf.get_sl_cycle_int_rpm_br()));
    xmlwriter.writeTextElement("sl_bemf_coupling_k", QString::number(mcconf.get_sl_bemf_coupling_k()));
    xmlwriter.writeTextElement("hall_table_0", QString::number(mcconf.get_hall_table()[0]));
    xmlwriter.writeTextElement("hall_table_1", QString::number(mcconf.get_hall_table()[1]));
    xmlwriter.writeTextElement("hall_table_2", QString::number(mcconf.get_hall_table()[2]));
    xmlwriter.writeTextElement("hall_table_3", QString::number(mcconf.get_hall_table()[3]));
    xmlwriter.writeTextElement("hall_table_4", QString::number(mcconf.get_hall_table()[4]));
    xmlwriter.writeTextElement("hall_table_5", QString::number(mcconf.get_hall_table()[5]));
    xmlwriter.writeTextElement("hall_table_6", QString::number(mcconf.get_hall_table()[6]));
    xmlwriter.writeTextElement("hall_table_7", QString::number(mcconf.get_hall_table()[7]));
    xmlwriter.writeTextElement("hall_sl_erpm", QString::number(mcconf.get_hall_sl_erpm()));
    xmlwriter.writeTextElement("foc_current_kp", QString::number(mcconf.get_foc_current_kp()));
    xmlwriter.writeTextElement("foc_current_ki", QString::number(mcconf.get_foc_current_ki()));
    xmlwriter.writeTextElement("foc_f_sw", QString::number(mcconf.get_foc_f_sw()));
    xmlwriter.writeTextElement("foc_dt_us", QString::number(mcconf.get_foc_dt_us()));
    xmlwriter.writeTextElement("foc_encoder_inverted", QString::number(mcconf.get_foc_encoder_inverted()));
    xmlwriter.writeTextElement("foc_encoder_offset", QString::number(mcconf.get_foc_encoder_offset()));
    xmlwriter.writeTextElement("foc_encoder_ratio", QString::number(mcconf.get_foc_encoder_ratio()));
    xmlwriter.writeTextElement("foc_sensor_mode", QString::number(mcconf.get_foc_sensor_mode()));
    xmlwriter.writeTextElement("foc_pll_kp", QString::number(mcconf.get_foc_pll_kp()));
    xmlwriter.writeTextElement("foc_pll_ki", QString::number(mcconf.get_foc_pll_ki()));
    xmlwriter.writeTextElement("foc_motor_l", QString::number(mcconf.get_foc_motor_l()));
    xmlwriter.writeTextElement("foc_motor_r", QString::number(mcconf.get_foc_motor_r()));
    xmlwriter.writeTextElement("foc_motor_flux_linkage", QString::number(mcconf.get_foc_motor_flux_linkage()));
    xmlwriter.writeTextElement("foc_observer_gain", QString::number(mcconf.get_foc_observer_gain()));
    xmlwriter.writeTextElement("foc_duty_dowmramp_kp", QString::number(mcconf.get_foc_duty_dowmramp_kp()));
    xmlwriter.writeTextElement("foc_duty_dowmramp_ki", QString::number(mcconf.get_foc_duty_dowmramp_ki()));
    xmlwriter.writeTextElement("foc_openloop_rpm", QString::number(mcconf.get_foc_openloop_rpm()));
    xmlwriter.writeTextElement("foc_sl_openloop_hyst", QString::number(mcconf.get_foc_sl_openloop_hyst()));
    xmlwriter.writeTextElement("foc_sl_openloop_time", QString::number(mcconf.get_foc_sl_openloop_time()));
    xmlwriter.writeTextElement("foc_sl_d_current_duty", QString::number(mcconf.get_foc_sl_d_current_duty()));
    xmlwriter.writeTextElement("foc_sl_d_current_factor", QString::number(mcconf.get_foc_sl_d_current_factor()));
    xmlwriter.writeTextElement("foc_hall_table_0", QString::number(mcconf.get_foc_hall_table()[0]));
    xmlwriter.writeTextElement("foc_hall_table_1", QString::number(mcconf.get_foc_hall_table()[1]));
    xmlwriter.writeTextElement("foc_hall_table_2", QString::number(mcconf.get_foc_hall_table()[2]));
    xmlwriter.writeTextElement("foc_hall_table_3", QString::number(mcconf.get_foc_hall_table()[3]));
    xmlwriter.writeTextElement("foc_hall_table_4", QString::number(mcconf.get_foc_hall_table()[4]));
    xmlwriter.writeTextElement("foc_hall_table_5", QString::number(mcconf.get_foc_hall_table()[5]));
    xmlwriter.writeTextElement("foc_hall_table_6", QString::number(mcconf.get_foc_hall_table()[6]));
    xmlwriter.writeTextElement("foc_hall_table_7", QString::number(mcconf.get_foc_hall_table()[7]));
    xmlwriter.writeTextElement("foc_hall_sl_erpm", QString::number(mcconf.get_foc_hall_sl_erpm()));
    xmlwriter.writeTextElement("s_pid_kp", QString::number(mcconf.get_s_pid_kp()));
    xmlwriter.writeTextElement("s_pid_ki", QString::number(mcconf.get_s_pid_ki()));
    xmlwriter.writeTextElement("s_pid_kd", QString::number(mcconf.get_s_pid_kd()));
    xmlwriter.writeTextElement("s_pid_min_rpm", QString::number(mcconf.get_s_pid_min_erpm()));
    xmlwriter.writeTextElement("p_pid_kp", QString::number(mcconf.get_p_pid_kp()));
    xmlwriter.writeTextElement("p_pid_ki", QString::number(mcconf.get_p_pid_ki()));
    xmlwriter.writeTextElement("p_pid_kd", QString::number(mcconf.get_p_pid_kd()));
    xmlwriter.writeTextElement("cc_startup_boost_duty", QString::number(mcconf.get_cc_startup_boost_duty()));
    xmlwriter.writeTextElement("cc_min_current", QString::number(mcconf.get_cc_min_current()));
    xmlwriter.writeTextElement("cc_gain", QString::number(mcconf.get_cc_gain()));
    xmlwriter.writeTextElement("cc_ramp_step_max", QString::number(mcconf.get_cc_ramp_step_max()));
    xmlwriter.writeTextElement("m_fault_stop_time_ms", QString::number(mcconf.get_m_fault_stop_time_ms()));
    xmlwriter.writeTextElement("m_duty_ramp_step", QString::number(mcconf.get_m_duty_ramp_step()));
    xmlwriter.writeTextElement("m_duty_ramp_step_rpm_lim", QString::number(mcconf.get_m_duty_ramp_step_rpm_lim()));
    xmlwriter.writeTextElement("m_current_backoff_gain", QString::number(mcconf.get_m_current_backoff_gain()));
    xmlwriter.writeTextElement("m_encoder_counts", QString::number(mcconf.get_m_encoder_counts()));
    xmlwriter.writeTextElement("meta_description", mcconf.get_meta_description());

    xmlwriter.writeEndElement();
    xmlwriter.writeEndDocument();
    file.close();

    return true;
}

bool Serialization::readMcconfXml(MC_Configuration &mcconf, QWidget *parent)
{
    QString filename = QFileDialog::getOpenFileName(parent,
                                                    tr("Load Configuration"), ".",
                                                    tr("Xml files (*.xml)"));

    bool retval = true;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QXmlStreamReader xmlreader(&file);
    xmlreader.readNext();
    QByteArray hall_table(8,0);
    QByteArray foc_hall_table(8,0) ;
    while (!xmlreader.atEnd()) {
        if (xmlreader.isStartElement()) {
            if (xmlreader.name() == "MCConfiguration") {
                xmlreader.readNext();

                while (!xmlreader.atEnd()) {
                    if (xmlreader.isEndElement()) {
                        xmlreader.readNext();
                        break;
                    }

                    if(xmlreader.name() == "pwm_mode") {mcconf.set_pwm_mode((mc_pwm_mode)xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "comm_mode") {mcconf.set_comm_mode((mc_comm_mode)xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "motor_type") {mcconf.set_motor_type((mc_motor_type)xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "sensor_mode") {mcconf.set_sensor_mode((mc_sensor_mode)xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "l_current_max") {mcconf.set_l_current_max(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_current_min") {mcconf.set_l_current_min(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_in_current_max") {mcconf.set_l_in_current_max(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_in_current_min") {mcconf.set_l_in_current_min(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_abs_current_max") {mcconf.set_l_abs_current_max(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_min_erpm") {mcconf.set_l_min_erpm(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_max_erpm") {mcconf.set_l_max_erpm(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_max_erpm_fbrake") {mcconf.set_l_max_erpm_fbrake(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_max_erpm_fbrake_cc") {mcconf.set_l_max_erpm_fbrake_cc(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_min_vin") {mcconf.set_l_min_vin(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_max_vin") {mcconf.set_l_max_vin(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_battery_cut_start") {mcconf.set_l_battery_cut_start(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_battery_cut_end") {mcconf.set_l_battery_cut_end(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_slow_abs_current") {mcconf.set_l_slow_abs_current(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "l_rpm_lim_neg_torque") {mcconf.set_l_rpm_lim_neg_torque(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "l_temp_fet_start") {mcconf.set_l_temp_fet_start(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_temp_fet_end") {mcconf.set_l_temp_fet_end(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_temp_motor_start") {mcconf.set_l_temp_motor_start(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_temp_motor_end") {mcconf.set_l_temp_motor_end(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_min_duty") {mcconf.set_l_min_duty(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "l_max_duty") {mcconf.set_l_max_duty(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "sl_min_erpm") {mcconf.set_sl_min_erpm(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "sl_min_erpm_cycle_int_limit") {mcconf.set_sl_min_erpm_cycle_int_limit(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "sl_max_fullbreak_current_dir_change") {mcconf.set_sl_max_fullbreak_current_dir_change(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "sl_cycle_int_limit") {mcconf.set_sl_cycle_int_limit(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "sl_cycle_int_limit_high_fac") {mcconf.set_sl_phase_advance_at_br(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "sl_cycle_int_rpm_br") {mcconf.set_sl_cycle_int_rpm_br(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "sl_bemf_coupling_k") {mcconf.set_sl_bemf_coupling_k(xmlreader.readElementText().toDouble());}

                    else if (xmlreader.name() == "hall_table_0") {hall_table[0]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_table_1") {hall_table[1]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_table_2") {hall_table[2]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_table_3") {hall_table[3]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_table_4") {hall_table[4]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_table_5") {hall_table[5]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_table_6") {hall_table[6]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_table_7") {hall_table[7]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "hall_sl_erpm") {mcconf.set_hall_sl_erpm(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_current_kp") {mcconf.set_foc_current_kp(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_current_ki") {mcconf.set_foc_current_ki(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_f_sw") {mcconf.set_foc_f_sw(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_dt_us") {mcconf.set_foc_dt_us(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_encoder_inverted") {mcconf.set_foc_encoder_inverted(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_encoder_offset") {mcconf.set_foc_encoder_offset(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_encoder_ratio") {mcconf.set_foc_encoder_ratio(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_sensor_mode") {mcconf.set_foc_sensor_mode((mc_foc_sensor_mode)xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_pll_kp") {mcconf.set_foc_pll_kp(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_pll_ki") {mcconf.set_foc_pll_ki(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_motor_l") {mcconf.set_foc_motor_l(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_motor_r") {mcconf.set_foc_motor_r(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_motor_flux_linkage") {mcconf.set_foc_motor_flux_linkage(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_observer_gain") {mcconf.set_foc_observer_gain(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_duty_dowmramp_kp") {mcconf.set_foc_duty_dowmramp_kp(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_duty_dowmramp_ki") {mcconf.set_foc_duty_dowmramp_ki(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_openloop_rpm") {mcconf.set_foc_openloop_rpm(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_sl_openloop_hyst") {mcconf.set_foc_sl_openloop_hyst(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_sl_openloop_time") {mcconf.set_foc_sl_openloop_time(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_sl_d_current_duty") {mcconf.set_foc_sl_d_current_duty(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_sl_d_current_factor") {mcconf.set_foc_sl_d_current_factor(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "foc_hall_table_0") {foc_hall_table[0]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_table_1") {foc_hall_table[1]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_table_2") {foc_hall_table[2]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_table_3") {foc_hall_table[3]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_table_4") {foc_hall_table[4]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_table_5") {foc_hall_table[5]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_table_6") {foc_hall_table[6]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_table_7") {foc_hall_table[7]=(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "foc_hall_sl_erpm") {mcconf.set_foc_hall_sl_erpm(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "s_pid_kp") {mcconf.set_s_pid_kp(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "s_pid_ki") {mcconf.set_s_pid_ki(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "s_pid_kd") {mcconf.set_s_pid_kd(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "s_pid_min_rpm") {mcconf.set_s_pid_min_erpm(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "p_pid_kp") {mcconf.set_p_pid_kp(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "p_pid_ki") {mcconf.set_p_pid_ki(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "p_pid_kd") {mcconf.set_p_pid_kd(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "cc_startup_boost_duty") {mcconf.set_cc_startup_boost_duty(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "cc_min_current") {mcconf.set_cc_min_current(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "cc_gain") {mcconf.set_cc_gain(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "cc_ramp_step_max") {mcconf.set_cc_ramp_step_max(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "m_fault_stop_time_ms") {mcconf.set_m_fault_stop_time_ms(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "m_duty_ramp_step") {mcconf.set_m_duty_ramp_step(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "m_duty_ramp_step_rpm_lim") {mcconf.set_m_duty_ramp_step_rpm_lim(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "m_current_backoff_gain") {mcconf.set_m_current_backoff_gain(xmlreader.readElementText().toDouble());}
                    else if (xmlreader.name() == "m_encoder_counts") {mcconf.set_m_encoder_counts(xmlreader.readElementText().toInt());}
                    else if (xmlreader.name() == "meta_description") {mcconf.set_meta_description(xmlreader.readElementText());}
                    else {
                        if (xmlreader.name().size() > 0) {
                            qDebug() << "Unknown XML element";
                            qDebug() << xmlreader.name();
                            qDebug() << xmlreader.readElementText();
                        }
                    }

                    xmlreader.readNext();
                }
            } else {
                xmlreader.readNext();
            }
        } else {
            xmlreader.readNext();
        }
    }
    mcconf.set_hall_table(hall_table);
    mcconf.set_foc_hall_table(foc_hall_table);

    file.close();

    return retval;
}
