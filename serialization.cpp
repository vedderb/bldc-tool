#include "serialization.h"
#include <QFileDialog>
#include <qxmlstream.h>
#include <QFile>

Serialization::Serialization(QObject *parent) :
    QObject(parent)
{
}

bool Serialization::writeMcconfXml(const PacketInterface::mc_configuration &mcconf, QWidget *parent)
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
    xmlwriter.writeTextElement("l_current_max", QString::number(mcconf.l_current_max));
    xmlwriter.writeTextElement("l_current_min", QString::number(mcconf.l_current_min));
    xmlwriter.writeTextElement("l_in_current_max", QString::number(mcconf.l_in_current_max));
    xmlwriter.writeTextElement("l_in_current_min", QString::number(mcconf.l_in_current_min));
    xmlwriter.writeTextElement("l_abs_current_max", QString::number(mcconf.l_abs_current_max));
    xmlwriter.writeTextElement("l_min_erpm", QString::number(mcconf.l_min_erpm));
    xmlwriter.writeTextElement("l_max_erpm", QString::number(mcconf.l_max_erpm));
    xmlwriter.writeTextElement("l_max_erpm_fbrake", QString::number(mcconf.l_max_erpm_fbrake));
    xmlwriter.writeTextElement("l_min_vin", QString::number(mcconf.l_min_vin));
    xmlwriter.writeTextElement("l_max_vin", QString::number(mcconf.l_max_vin));
    xmlwriter.writeTextElement("l_slow_abs_current", QString::number(mcconf.l_slow_abs_current));
    xmlwriter.writeTextElement("l_rpm_lim_neg_torque", QString::number(mcconf.l_rpm_lim_neg_torque));
    xmlwriter.writeTextElement("sl_is_sensorless", QString::number(mcconf.sl_is_sensorless));
    xmlwriter.writeTextElement("sl_min_erpm", QString::number(mcconf.sl_min_erpm));
    xmlwriter.writeTextElement("sl_min_erpm_cycle_int_limit", QString::number(mcconf.sl_min_erpm_cycle_int_limit));
    xmlwriter.writeTextElement("sl_cycle_int_limit", QString::number(mcconf.sl_cycle_int_limit));
    xmlwriter.writeTextElement("sl_cycle_int_limit_high_fac", QString::number(mcconf.sl_cycle_int_limit_high_fac));
    xmlwriter.writeTextElement("sl_cycle_int_rpm_br", QString::number(mcconf.sl_cycle_int_rpm_br));
    xmlwriter.writeTextElement("sl_bemf_coupling_k", QString::number(mcconf.sl_bemf_coupling_k));
    xmlwriter.writeTextElement("hall_dir", QString::number(mcconf.hall_dir));
    xmlwriter.writeTextElement("hall_fwd_add", QString::number(mcconf.hall_fwd_add));
    xmlwriter.writeTextElement("hall_rev_add", QString::number(mcconf.hall_rev_add));
    xmlwriter.writeTextElement("s_pid_kp", QString::number(mcconf.s_pid_kp));
    xmlwriter.writeTextElement("s_pid_ki", QString::number(mcconf.s_pid_ki));
    xmlwriter.writeTextElement("s_pid_kd", QString::number(mcconf.s_pid_kd));
    xmlwriter.writeTextElement("s_pid_min_rpm", QString::number(mcconf.s_pid_min_rpm));
    xmlwriter.writeTextElement("cc_startup_boost_duty", QString::number(mcconf.cc_startup_boost_duty));
    xmlwriter.writeTextElement("cc_min_current", QString::number(mcconf.cc_min_current));
    xmlwriter.writeTextElement("cc_gain", QString::number(mcconf.cc_gain));
    xmlwriter.writeTextElement("m_fault_stop_time_ms", QString::number(mcconf.m_fault_stop_time_ms));
    xmlwriter.writeTextElement("meta_description", mcconf.meta_description);

    xmlwriter.writeEndElement();
    xmlwriter.writeEndDocument();
    file.close();

    return true;
}

bool Serialization::readMcconfXml(PacketInterface::mc_configuration &mcconf, QWidget *parent)
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

                    if(xmlreader.name() == "pwm_mode") {mcconf.pwm_mode = (PacketInterface::mc_pwm_mode)xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "comm_mode") {mcconf.comm_mode = (PacketInterface::mc_comm_mode)xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "l_current_max") {mcconf.l_current_max = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_current_min") {mcconf.l_current_min = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_in_current_max") {mcconf.l_in_current_max = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_in_current_min") {mcconf.l_in_current_min = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_abs_current_max") {mcconf.l_abs_current_max = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_min_erpm") {mcconf.l_min_erpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_erpm") {mcconf.l_max_erpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_erpm_fbrake") {mcconf.l_max_erpm_fbrake = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_min_vin") {mcconf.l_min_vin = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_max_vin") {mcconf.l_max_vin = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "l_slow_abs_current") {mcconf.l_slow_abs_current = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "l_rpm_lim_neg_torque") {mcconf.l_rpm_lim_neg_torque = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "sl_is_sensorless") {mcconf.sl_is_sensorless = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "sl_min_erpm") {mcconf.sl_min_erpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_min_erpm_cycle_int_limit") {mcconf.sl_min_erpm_cycle_int_limit = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_cycle_int_limit") {mcconf.sl_cycle_int_limit = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_cycle_int_limit_high_fac") {mcconf.sl_cycle_int_limit_high_fac = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_cycle_int_rpm_br") {mcconf.sl_cycle_int_rpm_br = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "sl_bemf_coupling_k") {mcconf.sl_bemf_coupling_k = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "hall_dir") {mcconf.hall_dir = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_fwd_add") {mcconf.hall_fwd_add = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "hall_rev_add") {mcconf.hall_rev_add = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "s_pid_kp") {mcconf.s_pid_kp = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "s_pid_ki") {mcconf.s_pid_ki = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "s_pid_kd") {mcconf.s_pid_kd = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "s_pid_min_rpm") {mcconf.s_pid_min_rpm = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "cc_startup_boost_duty") {mcconf.cc_startup_boost_duty = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "cc_min_current") {mcconf.cc_min_current = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "cc_gain") {mcconf.cc_gain = xmlreader.readElementText().toDouble();}
                    else if (xmlreader.name() == "m_fault_stop_time_ms") {mcconf.m_fault_stop_time_ms = xmlreader.readElementText().toInt();}
                    else if (xmlreader.name() == "meta_description") {mcconf.meta_description = xmlreader.readElementText();}

                    xmlreader.readNext();
                }
            } else {
                retval = false;
                break;
            }
        } else {
            xmlreader.readNext();
        }
    }

    file.close();

    return retval;
}
