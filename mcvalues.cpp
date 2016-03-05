#include "mcvalues.h"

McValues::McValues(QObject *parent) : QObject(parent),
    m_power           (0),
    m_batteryVoltage  (0),
    m_MOSFET_Temp     (0),
    m_motorCurrent    (0),
    m_batteryCurrent  (0),
    m_electricalSpeed (0),
    m_dutyCycle       (0),
    m_drawnCap        (0),
    m_chargedCap      (0),
    m_drawnEnergy     (0),
    m_chargedEnergy   (0),
    m_tacho           (0),
    m_tachoABS        (0)
{
}

McValues::McValues(MC_VALUES values)
{
    m_power           = values.v_in * values.current_in	;
    m_batteryVoltage  = values.v_in               		;
    m_MOSFET_Temp     = values.temp_mos1          		;
    m_motorCurrent    = values.current_motor      		;
    m_batteryCurrent  = values.current_in         		;
    m_electricalSpeed = values.rpm                		;
    m_dutyCycle       = values.duty_now *100.0   		;
    m_drawnCap        = values.amp_hours *1000.0 		;
    m_chargedCap      = values.amp_hours_charged *1000.0;
    m_drawnEnergy     = values.watt_hours         		;
    m_chargedEnergy   = values.watt_hours_charged 		;
    m_tacho           = values.tachometer         		;
    m_tachoABS        = values.tachometer_abs     		;
    m_faultString     = values.fault_str          		;
}

void McValues::setValues(MC_VALUES values)
{
    update_power          ( values.v_in * values.current_in	);
    update_batteryVoltage ( values.v_in               		);
    update_MOSFET_Temp    ( values.temp_mos1          		);
    update_motorCurrent   ( values.current_motor      		);
    update_batteryCurrent ( values.current_in         		);
    update_electricalSpeed( values.rpm                		);
    update_dutyCycle      ( values.duty_now *100.0   		);
    update_drawnCap       ( values.amp_hours *1000.0 		);
    update_chargedCap     ( values.amp_hours_charged *1000.0);
    update_drawnEnergy    ( values.watt_hours         		);
    update_chargedEnergy  ( values.watt_hours_charged 		);
    update_tacho          ( values.tachometer         		);
    update_tachoABS       ( values.tachometer_abs     		);
    update_faultString    ( values.fault_str          		);
}


