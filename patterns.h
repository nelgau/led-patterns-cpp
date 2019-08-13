// Texture example: "dot" image, wandering around the frame.

#pragma once

#include "lib/color.h"
#include <cstdlib>
#include <list>
#include <algorithm>

static double frand_range(double fmin, double fmax)
{
    double f = (double)rand() / RAND_MAX;
    return fmin + f * (fmax - fmin);
}

class PatternsEffect : public Effect
{
    static const int strip_length = 112;
    static const int limited_length = 100;
    static const int max_entities = 40;

    static const std::vector<std::pair<float, float>> hue_ranges;

    struct Entity
    {
        int low, high;
        float hue;
        float lightness;

        float fade_interval;
        float delay_interval;
        float darken_interval;
        float time;

    public:
        Entity() {
            int length = rand() % 12 + 4;
            this->low = rand() % (limited_length - length);
            this->high = low + length;

            int range_index = rand() % 2;
            float hue_low = hue_ranges[range_index].first / 360.0;
            float hue_high = hue_ranges[range_index].second / 360.0;
            this->hue = frand_range(hue_low, hue_high);     
            this->lightness = frand_range(0.5, 1);

            this->fade_interval = frand_range(1, 1.75);
            this->delay_interval = frand_range(0, 8);
            this->darken_interval = 8;
            this->time = 0;
        }

        void step(float dt) {
            this->time += dt;
        }

        bool alive(void) {
            return this->time <= this->fade_interval + this->delay_interval + this->darken_interval;
        }

        void draw(Vec3 buffer[]) {
            float effect_time = std::max<float>(this->time - this->delay_interval, 0);
            float darken_time = std::max<float>(effect_time - this->fade_interval, 0);

            float lmult = this->lightness;            

            if (effect_time <= 0) {
                return;
            }

            if (effect_time < this->fade_interval) {
                float fx = effect_time / this->fade_interval;
                lmult *= fx * fx * (3 - 2 * fx);
            }

            if (darken_time > 0) {
                lmult *= expf(-0.1 * darken_time * darken_time);
            }

            for (int i = this->low; i < this->high; i++) {
                float f = (double)(i - this->low) / (this->high - this->low);

                float h = this->hue + 0.02 * cosf(3.14 * (f - 0.5));
                float l = 0.4 + 0.2 * cosf(3.14 * (f - 0.5));

                l *= lmult;

                float sat = l < .5 ? l : 1 - l;
                float s = 2 * sat / (l + sat);
                float v = l + sat;

                Vec3 fc;
                hsv2rgb(fc, h, s, v);

                float x = f - 0.5;
                float sigma = 5;
                float a = expf(-sigma * x * x);

                for (int j = 0; j < 3; j++) {
                    buffer[i][j] = 1 - (1 - a * fc[j]) * (1 - buffer[i][j]);
                }
            }
        }
    };

    class Controller
    {
    public:
        Controller(Vec3 buffer[]) : m_buffer(buffer) {};

        void step(float dt) {
            for (auto &e : m_entities) {
                e.step(dt);
            }

            m_entities.remove_if([](Entity &e) {
                return !e.alive();
            });

            if (m_entities.size() < max_entities) {
                m_entities.push_back(Entity());
            }
        }

        void render(void) {
            for (auto &e : m_entities) {
                e.draw(m_buffer);
            }            
        }

    private:        
        Vec3 *m_buffer;
        std::list<Entity> m_entities;        
    };

public:
    PatternsEffect(void) :
        m_buffer(8 * strip_length) 
    {
        for (int i = 0; i < 8; i++) {
            m_controllers.push_back(Controller(&m_buffer[i * strip_length]));
        }
    }

    virtual void beginFrame(const FrameInfo &f)
    {
        std::fill(m_buffer.begin(), m_buffer.end(), Vec3(0, 0, 0));

        for (auto &c : m_controllers) {
            c.step(f.timeDelta);
            c.render();
        }        
    }

    virtual void shader(Vec3& rgb, const PixelInfo &p) const
    {
            rgb = m_buffer[p.index]; 
    }

private:
    std::vector<Vec3> m_buffer;;
    std::list<Controller> m_controllers;

};

const std::vector<std::pair<float, float>> PatternsEffect::hue_ranges = {
    std::make_pair(150, 330),
    std::make_pair(20, 40)
};
