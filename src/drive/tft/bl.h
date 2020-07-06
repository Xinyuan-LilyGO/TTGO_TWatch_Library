#ifndef __BL_H
#define __BL_H
#include <Arduino.h>
#include <Ticker.h>

class BackLight
{
public:
    BackLight(int pin, int channel = 0)
    {
        _pin = pin;
        _channel = channel;
    };
    ~BackLight()
    {
        ledcDetachPin(_pin);
    };

    void begin()
    {
        ledcAttachPin(_pin, _channel);
        ledcSetup(_channel, 12000, 8);
        ledcWrite(_channel, 0);
    };

    void adjust(uint8_t level)
    {
	_level = level;
        ledcWrite(_channel, level);
        _on = true;
    };

    bool isOn()
    {
        return _on;
    };

    void on()
    {
        _on = true;
        ledcWrite(_channel, _level);
    };

    void off()
    {
        _on = false;
        ledcWrite(_channel, 0);
    };

    bool reverse()
    {
        if (isOn()) {
            off(); return false;
        } else {
            on(); return true;
        }
    };

private:
    bool _on = false;
    int _pin;
    uint8_t _channel;
    uint8_t _level = 128;
};


class Motor
{
public:
    Motor(int pin, int channel = 1, int freq = 1000)
    {
        _pin = pin;
        _channel = channel;
        _freq = freq;
    };
    ~Motor()
    {
        ledcDetachPin(_pin);
        delete tick;
    };

    void begin()
    {
        ledcAttachPin(_pin, _channel);
        ledcSetup(_channel, 12000, 8);
        ledcWrite(_channel, 0);
        tick = new Ticker;
    };

    void adjust(uint8_t level)
    {
        ledcWrite(_channel, level);
    };

    void onec()
    {
        ledcWriteTone(_channel, _freq);
        tick->once_ms(200, []() {
            ledcWriteTone(1, 0);
        });
    };

private:
    int _pin;
    uint8_t _channel;
    double _freq;
    Ticker *tick;
};


class Buzzer
{
public:
    Buzzer(int pin, int channel = 2, int freq = 1000)
    {
        _pin = pin;
        _channel = channel;
        _freq = freq;
    };
    ~Buzzer()
    {
        ledcDetachPin(_pin);
        delete tick;
    };

    void begin()
    {
        ledcAttachPin(_pin, _channel);
        ledcSetup(_channel, 12000, 8);
        ledcWrite(_channel, 0);
        tick = new Ticker;
    };

    void adjust(uint8_t level)
    {
        ledcWrite(_channel, level);
    };

    void onec()
    {
        ledcWriteTone(_channel, _freq);
        tick->once_ms(200, []() {
            ledcWriteTone(2, 0);
        });
    };

private:
    int _pin;
    uint8_t _channel;
    double _freq;
    Ticker *tick;
};

#endif
