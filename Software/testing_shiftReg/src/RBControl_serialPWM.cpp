#include <freertos/FreeRTOS.h>

#include <cassert>
#include <cstdint>
#include <cstring>

#include "RBControl_serialPWM.hpp"

namespace rb {

volatile void* SerialPWM::i2snum2struct(const int num) {
    assert(num >= 0 && num < 2);
    switch (num) {
    case 0:
        return static_cast<volatile void*>(&I2S0);
    case 1:
        return static_cast<volatile void*>(&I2S1);
    }
    return nullptr;
}

SerialPWM::SerialPWM(const int channels,
    const int data_pin,
    const int latch_pin,
    const int clock_pin,
    const int frequency,
    const int i2s)
    : c_channels(channels)
    , c_bytes(1)
    , m_i2s(i2snum2struct(i2s))
    , m_buffer_descriptors { nullptr }
    , m_buffer { nullptr }
    , m_active_buffer(0)
    , m_pwm(channels, 0) {
    const int buffer_size = c_channels * c_bytes;
    for (int buffer = 0; buffer != sc_buffers; ++buffer) {
        m_buffer_descriptors[buffer] = static_cast<i2s_parallel_buffer_desc_t*>(heap_caps_malloc((sc_resolution + 1) * sizeof(i2s_parallel_buffer_desc_t), MALLOC_CAP_32BIT)); // +1 for end mark
        for (int i = 0; i != sc_resolution; ++i) {
            uint8_t* p_buffer = static_cast<uint8_t*>(heap_caps_malloc(buffer_size, MALLOC_CAP_DMA));
            m_buffer_descriptors[buffer][i].memory = p_buffer;
            m_buffer_descriptors[buffer][i].size = buffer_size;
            memset(p_buffer, 0, buffer_size);
            p_buffer[c_bytes - 1] = (1 << (1 & 7)); // latch pin
            m_buffer[buffer][i] = p_buffer;
        }
        m_buffer_descriptors[buffer][sc_resolution].memory = nullptr;
    }

    i2s_parallel_config_t cfg;
    cfg.bits = I2S_PARALLEL_BITS_8; // if c_bytes == 1

    int i = 0;
    cfg.gpio_bus[i++] = data_pin;
    cfg.gpio_bus[i++] = latch_pin;
    for (; i != 24; ++i)
        cfg.gpio_bus[i] = -1;
    cfg.gpio_wclk = -1;
    cfg.inv_wclk = false;
    cfg.gpio_bclk = clock_pin;
    cfg.inv_bclk = false;
    cfg.clkspeed = frequency * sc_resolution * c_channels;
    cfg.bufa = m_buffer_descriptors[0];
    cfg.bufb = m_buffer_descriptors[1];

    i2s_parallel_setup(static_cast<i2s_dev_t*>(m_i2s), &cfg);
    update();
}

SerialPWM::~SerialPWM() {
    i2s_driver_uninstall(static_cast<i2s_port_t>(i2snum(static_cast<i2s_dev_t*>(m_i2s))));
    for (int buffer = 0; buffer != sc_buffers; ++buffer) {
        heap_caps_free(m_buffer_descriptors[buffer]);
        m_buffer_descriptors[buffer] = nullptr;
        for (int i = 0; i != sc_resolution; ++i) {
            heap_caps_free(m_buffer[buffer][i]);
            m_buffer[buffer][i] = nullptr;
        }
    }
}

SerialPWM::value_type& SerialPWM::operator[](size_t index) { return m_pwm[index]; }

void SerialPWM::update() {
    m_active_buffer ^= 1;
    for (int sample = 0; sample != sc_resolution; ++sample) {
        for (int channel = 0; channel != m_pwm.size(); ++channel) {
            uint8_t& value = m_buffer[m_active_buffer][sample][(channel % c_channels) * c_bytes + ((channel / c_channels) >> 3)];
            if (sample < m_pwm[channel])
                value |= (1 << ((channel / c_channels) & 7));
            else
                value &= ~(1 << ((channel / c_channels) & 7));
        }
    }
    i2s_parallel_flip_to_buffer(static_cast<i2s_dev_t*>(m_i2s), m_active_buffer);
}

int SerialPWM::resolution() { return sc_resolution; }

} // namespace rb
