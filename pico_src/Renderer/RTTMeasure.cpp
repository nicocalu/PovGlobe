#include <stdio.h>
#include <cmath>

#include "RTTMeasure.hpp"

uint32_t RTTMeasure::m_measured_intervals[N_MAGNETS] = {};
int8_t RTTMeasure::m_curr_segment_index = 0;
critical_section_t RTTMeasure::critical_section = {};
absolute_time_t RTTMeasure::m_last_hall_sensor_event = nil_time;
absolute_time_t RTTMeasure::m_last_print = nil_time;

RTTMeasure::RTTMeasure()
{
    init();
    gpio_set_irq_enabled_with_callback(PIN_HALL_SENSOR, GPIO_IRQ_EDGE_RISE, true, &RTTMeasure::gpio_hall_sensor_callback);
    //set interrupt to callback func gpio_hall_sensor_callback
}

RTTMeasure::~RTTMeasure()
{
}

void RTTMeasure::init(){
    for (size_t i = 0; i < N_MAGNETS; i++)
    {
        m_measured_intervals[i] = get_absolute_time();
    }
    m_curr_segment_index = 0;
    m_last_hall_sensor_event = nil_time;

    critical_section_init(&critical_section); //set up critical section 
}

RTTMeasure& RTTMeasure::getInstance() {
    static RTTMeasure instance;
    return instance;
}

void RTTMeasure::gpio_hall_sensor_callback(uint gpio, uint32_t events) {
    const absolute_time_t curr_time = get_absolute_time();

    if (gpio != PIN_HALL_SENSOR)
        return;
    
    const uint32_t old_dt = m_measured_intervals[m_curr_segment_index];

    critical_section_enter_blocking(&critical_section);

    if (!is_nil_time(m_last_hall_sensor_event)){ //set up dts for the first time
        const uint32_t new_dt = absolute_time_diff_us(m_last_hall_sensor_event, curr_time);
        m_measured_intervals[m_curr_segment_index] = 0.5* new_dt + 0.5*old_dt;
    }
    m_last_hall_sensor_event = curr_time;
    m_curr_segment_index = (m_curr_segment_index + 1U) % N_MAGNETS; //change segment index
    critical_section_exit(&critical_section);
}