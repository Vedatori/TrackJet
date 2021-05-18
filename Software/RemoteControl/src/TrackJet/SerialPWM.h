#pragma once

#include <freertos/FreeRTOS.h>

#include <driver/i2s.h>
#include "i2s_parallel.h"

#define PWM_CHANNELS 32
#define DISP_ROWS 8
#define DISP_COLS 8
#define DISP_PWM_RESOLUTION 12

class SerialPWM {
public:
    typedef int value_type;

    SerialPWM(
        const int data_pin,
        const int latch_pin,
        const int clock_pin,
        const int a_output_enable_pin,
        const int frequency = 20000,
        const int i2s = 1);

    ~SerialPWM();

    void setPWM(uint8_t index, uint8_t width);
    void setDisp(uint8_t state[][DISP_COLS]);
    void setDispAll(uint8_t brightness);
    void setDispSingle(uint8_t row, uint8_t col, uint8_t brightness);

    void update();

    void set_output(bool state);    // 0-disabled, 1-enabled

    static int resolution();

private:
    SerialPWM(const SerialPWM&) = delete;

    static volatile void* i2snum2struct(const int num);

    static constexpr int sc_buffers = 2;
    static constexpr int sc_resolution = DISP_ROWS*DISP_PWM_RESOLUTION;
    static int8_t pwm_index[];
    static uint8_t row_pin_index[];
    static uint8_t col_pin_index[];
    const int c_channels = PWM_CHANNELS;
    int m_output_enable_pin;
    volatile void* m_i2s; // m_i2s is actually i2s_dev_t*, but this is an anonymous struct in the Espressif header i2s_struct.h and that causes a compilation error
    i2s_parallel_buffer_desc_t* m_buffer_descriptors[sc_buffers];
    uint8_t* m_buffer[sc_buffers][sc_resolution];
    int m_active_buffer = 0;
    static uint8_t m_pwm[];
    static uint8_t m_disp[DISP_ROWS][DISP_COLS];
};