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

Serialization::Serialization(QObject *parent) :
    QObject(parent)
{
}

bool Serialization::writeMcconfXml(const mc_configuration &mcconf, QWidget *parent)
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
    xmlwriter.writeTextElement("pwm_mode", QString::number(mcconf.pwm_mode));
    xmlwriter.writeTextElement("comm_mode", QString::number(mcconf.comm_mode));
    xmlwriter.writeTextElement("motor_type", QString::number(mcconf.motor_type));
    xmlwriter.writeTextElement("sensor_mode", QString::number(mcconf.sensor_mode));
    xmlwriter.writeTextElement("l_current_max", QString::number(mcconf.l_current_max));
    xmlwriter.writeTextElement("l_current_min", QString::number(mcconf.l_current_min));
    xmlwriter.writeTextElement("l_in_current_max", QString::number(mcconf.l_in_current_max));
    xmlwriter.writeTextElement("l_in_current_min", QString::number(mcconf.l_in_current_min));
    xmlwriter.writeTextElement("l_abs_current_max", QString::number(mcconf.l_abs_current_max));
    xmlwriter.writeTextElement("l_min_erpm", QString::number(mcconf.l_min_erpm));
    xmlwriter.writeTextElement("l_max_erpm", QString::number(mcconf.l_max_erpm));
    xmlwriter.writeTextElement("l_max_erpm_fbrake", QString::number(mcconf.l_max_erpm_fbrake));
    xmlwriter.writeTextElement("l_max_erpm_fbrake_cc", QString::number(mcconf.l_max_erpm_fbrake_cc));
    xmlwriter.writeTextElement("l_min_vin", QString::number(mcconf.l_min_vin));
    xmlwriter.writeTextElement("l_max_vin", QString::number(mcconf.l_max_vin));
    xmlwriter.writeTextElement("l_slow_abs_current", QString::number(mcconf.l_slow_abs_current));
    xmlwriter.writeTextElement("l_rpm_lim_neg_torque", QString::number(mcconf.l_rpm_lim_neg_torque));
    xmlwriter.writeTextElement("l_temp_fet_start", QString::number(mcconf.l_temp_fet_start));
    xmlwriter.writeTextElement("l_temp_fet_end", QString::number(mcconf.l_temp_fet_end));
    xmlwriter.writeTextElement("l_temp_motor_start", QString::number(mcconf.l_temp_motor_start));
    xmlwriter.writeTextElement("l_temp_motor_end", QString::number(mcconf.l_temp_motor_end));
    xmlwriter.writeTextElement("l_min_duty", QString::number(mcconf.l_min_duty));
    xmlwriter.writeTextElement("l_max_duty", QString::number(mcconf.l_max_duty));
    xmlwriter.writeTextElement("sl_min_erpm", QString::number(mcconf.sl_min_erpm));
    xmlwriter.writeTextElement("sl_min_erpm_cycle_int_limit", QString::number(mcconf.sl_min_erpm_cycle_int_limit));
    xmlwriter.writeTextElement("sl_max_fullbreak_current_dir_change", QString::number(mcconf.sl_max_fullbreak_current_dir_change));
    xmlwriter.writeTextElement("sl_cycle_int_limit", QString::number(mcconf.sl_cycle_int_limit));
    xmlwriter.writeTextElement("sl_cycle_int_limit_high_fac", QString::number(mcconf.sl_phase_advance_at_br));
    xmlwriter.writeTextElement("sl_cycle_int_rpm_br", QString::number(mcconf.sl_cycle_int_rpm_br));
    xmlwriter.writeTextElement("sl_bemf_coupling_k", QString::number(mcconf.sl_bemf_coupling_k));
    xmlwriter.writeTextElement("hall_table_0", QString::number(mcconf.hall_table[0]));
    xmlwriter.writeTextElement("hall_table_1", QString::number(mcconf.hall_table[1]));
    xmlwriter.writeTextElement("hall_table_2", QString::number(mcconf.hall_table[2]));
    xmlwriter.writeTextElement("hall_table_3", QString::number(mcconf.hall_table[3]));
    xmlwriter.writeTextElement("hall_table_4", QString::number(mcconf.hall_table[4]));
    xmlwriter.writeTextElement("hall_table_5", QString::number(mcconf.hall_table[5]));
    xmlwriter.writeTextElement("hall_table_6", QString::number(mcconf.hall_table[6]));
    xmlwriter.writeTextElement("hall_table_7", QString::number(mcconf.hall_table[7]));
    xmlwriter.writeTextElement("hall_sl_erpm", QString::number(mcconf.hall_sl_erpm));
    xmlwriter.writeTextElement("s_pid_kp", QString::number(mcconf.s_pid_kp));
    xmlwriter.writeTextElement("s_pid_ki", QString::number(mcconf.s_pid_ki));
    xmlwriter.writeTextElement("s_pid_kd", QString::number(mcconf.s_pid_kd));
    xmlwriter.writeTextElement("s_pid_min_rpm", QString::number(mcconf.s_pid_min_rpm));
    xmlwriter.writeTextElement("p_pid_kp", QString::number(mcconf.p_pid_kp));
    xmlwriter.writeTextElement("p_pid_ki", QString::number(mcconf.p_pid_ki));
    xmlwriter.writeTextElement("p_pid_kd", QString::number(mcconf.p_pid_kd));
    xmlwriter.writeTextElement("cc_startup_boost_duty", QString::number(mcconf.cc_startup_boost_duty));
    xmlwriter.writeTextElement("cc_min_current", QString::number(mcconf.cc_min_current));
    xmlwriter.writeTextElement("cc_gain", QString::number(mcconf.cc_gain));
    xmlwriter.writeTextElement("cc_ramp_step_max", QString::number(mcconf.cc_ramp_step_max));
    xmlwriter.writeTextElement("m_fault_stop_time_ms", QString::number(mcconf.m_fault_stop_time_ms));
    xmlwriter.writeTextElement("meta_description", mcconf.meta_description);

    xmlwriter.writeEndElement();
    xmlwriter.writeEndDocument();
    file.close();

    return true;
}

bool Serialization::readMcconfXml(mc_configuration &mcconf, QWidget *parent)
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
    while (!xmlreader.atEnd()) {
        if (xmlreader.isStartElement()) {
            if (xmlreader.name() == "MCConfiguration") {
                xmlreader.readNext();

                while (!xmlreader.atEnd()) {
                    if (xmlreader.isEndElement()) {
                        xmlreader.readNext();
                        break;
                    }

                    if(xmlreader.name() == "pwm_mode") {mcconf.pwm_mode = (mc_pwm_mode)xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "comm_mode") {mcconf.comm_mode = (mc_comm_mode)xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "motor_type") {mcconf.motor_type = (mc_motor_type)xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "sensor_mode") {mcconf.sensor_mode = (mc_sensor_mode)xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "l_current_max") {mcconf.l_current_max = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_current_min") {mcconf.l_current_min = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_in_current_max") {mcconf.l_in_current_max = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_in_current_min") {mcconf.l_in_current_min = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_abs_current_max") {mcconf.l_abs_current_max = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_min_erpm") {mcconf.l_min_erpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_erpm") {mcconf.l_max_erpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_erpm_fbrake") {mcconf.l_max_erpm_fbrake = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_erpm_fbrake_cc") {mcconf.l_max_erpm_fbrake_cc = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_min_vin") {mcconf.l_min_vin = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_vin") {mcconf.l_max_vin = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_slow_abs_current") {mcconf.l_slow_abs_current = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "l_rpm_lim_neg_torque") {mcconf.l_rpm_lim_neg_torque = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "l_temp_fet_start") {mcconf.l_temp_fet_start = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_temp_fet_end") {mcconf.l_temp_fet_end = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_temp_motor_start") {mcconf.l_temp_motor_start = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_temp_motor_end") {mcconf.l_temp_motor_end = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_min_duty") {mcconf.l_min_duty = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_duty") {mcconf.l_max_duty = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_min_erpm") {mcconf.sl_min_erpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_min_erpm_cycle_int_limit") {mcconf.sl_min_erpm_cycle_int_limit = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_max_fullbreak_current_dir_change") {mcconf.sl_max_fullbreak_current_dir_change = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_cycle_int_limit") {mcconf.sl_cycle_int_limit = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_cycle_int_limit_high_fac") {mcconf.sl_phase_advance_at_br = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_cycle_int_rpm_br") {mcconf.sl_cycle_int_rpm_br = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_bemf_coupling_k") {mcconf.sl_bemf_coupling_k = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "hall_table_0") {mcconf.hall_table[0] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_table_1") {mcconf.hall_table[1] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_table_2") {mcconf.hall_table[2] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_table_3") {mcconf.hall_table[3] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_table_4") {mcconf.hall_table[4] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_table_5") {mcconf.hall_table[5] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_table_6") {mcconf.hall_table[6] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_table_7") {mcconf.hall_table[7] = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_sl_erpm") {mcconf.hall_sl_erpm = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "s_pid_kp") {mcconf.s_pid_kp = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "s_pid_ki") {mcconf.s_pid_ki = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "s_pid_kd") {mcconf.s_pid_kd = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "s_pid_min_rpm") {mcconf.s_pid_min_rpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "p_pid_kp") {mcconf.p_pid_kp = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "p_pid_ki") {mcconf.p_pid_ki = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "p_pid_kd") {mcconf.p_pid_kd = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "cc_startup_boost_duty") {mcconf.cc_startup_boost_duty = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "cc_min_current") {mcconf.cc_min_current = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "cc_gain") {mcconf.cc_gain = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "cc_ramp_step_max") {mcconf.cc_ramp_step_max = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "m_fault_stop_time_ms") {mcconf.m_fault_stop_time_ms = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "meta_description") {mcconf.meta_description = xmlreader.readElementText();}
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

    file.close();

    return retval;
}
