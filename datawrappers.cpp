#include "datawrappers.h"

mc_configurationWrapper::mc_configurationWrapper(QObject*)
{
}

mc_configurationWrapper::mc_configurationWrapper(const mc_configuration &src)
{
    m_data = src;
    for (int i = 0; i < 8; ++i) {
        m_hall_table.append(src.hall_table[i]);
        m_foc_hall_table.append(src.foc_hall_table[i]);
    }
}

QList<int> mc_configurationWrapper::foc_hall_table() const
{
    return m_foc_hall_table;
}

QList<int> mc_configurationWrapper::hall_table() const
{
    return m_hall_table;
}

const mc_configuration &mc_configurationWrapper::data(){
    return m_data;
}

void mc_configurationWrapper::setFoc_hall_table(QList<int> foc_hall_table)
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

void mc_configurationWrapper::setHall_table(QList<int> hall_table)
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
