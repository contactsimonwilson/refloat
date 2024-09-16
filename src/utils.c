// Copyright 2024 Lukas Hrazky
//
// This file is part of the Refloat VESC package.
//
// Refloat VESC package is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// Refloat VESC package is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

#include "utils.h"

#include <math.h>

uint32_t rnd(uint32_t seed) {
    return seed * 1664525u + 1013904223u;
}

void rate_limitf(float *value, float target, float step) {
    if (fabsf(target - *value) < step) {
        *value = target;
    } else if (target - *value > 0) {
        *value += step;
    } else {
        *value -= step;
    }
}

// Smoothen changes in tilt angle by ramping the step size
// smooth_center_window: Sets the angle away from Target that step size begins ramping down
void smooth_rampf(
    float *value,
    float *ramped_step,
    float target,
    float step,
    float smoothing_factor,
    float smooth_center_window
) {
    float tiltback_target_diff = target - *value;

    // Within X degrees of Target Angle, start ramping down step size
    if (fabsf(tiltback_target_diff) < smooth_center_window) {
        // Target step size is reduced the closer to center you are (needed for smoothly
        // transitioning away from center)
        *ramped_step = (smoothing_factor * step * (tiltback_target_diff / 2)) +
            ((1 - smoothing_factor) * *ramped_step);
        // Linearly ramped down step size is provided as minimum to prevent overshoot
        float centering_step = fminf(fabsf(*ramped_step), fabsf(tiltback_target_diff / 2) * step) *
            sign(tiltback_target_diff);
        if (fabsf(tiltback_target_diff) < fabsf(centering_step)) {
            *value = target;
        } else {
            *value += centering_step;
        }
    } else {
        // Ramp up step size until the configured tilt speed is reached
        *ramped_step = (smoothing_factor * step * sign(tiltback_target_diff)) +
            ((1 - smoothing_factor) * *ramped_step);
        *value += *ramped_step;
    }
}

void rampf_simple_step_first(
    float *value, float *v1, float target, float step, float smoothing_factor
) {
    float delta = target - *v1;

    if (fabsf(delta) > step) {
        delta = step * sign(delta);
    }

    *v1 += delta;

    *value += smoothing_factor * (*v1 - *value);
}

void rampf_simple_step_last(float *value, float target, float step, float smoothing_factor) {
    float delta = smoothing_factor * (target - *value);

    if (fabsf(delta) > step) {
        delta = step * sign(delta);
    }

    *value += delta;
}

void rampf_ema_2nd(float *value, float *v1, float target, float step, float smoothing_factor) {
    *v1 += smoothing_factor * (target - *v1);

    float delta = smoothing_factor * (*v1 - *value);

    if (fabsf(delta) > step) {
        delta = step * sign(delta);
    }

    *value += delta;
}

void rampf_ema_3rd(
    float *value, float *v1, float *v2, float target, float step, float smoothing_factor
) {
    *v1 += smoothing_factor * (target - *v1);
    *v2 += smoothing_factor * (*v1 - *v2);

    float delta = smoothing_factor * (*v2 - *value);

    if (fabsf(delta) > step) {
        delta = step * sign(delta);
    }

    *value += delta;
}

// Does EMA on the target, but applies a secondary EMA on the step of the first
// one (since EMA can be understood as adding the difference between target and
// value multiplied by alpha, and that's the ramped_step below). Only applies
// the secondary EMA when the step is increasing, not when it's decreasing. Has
// issues dealing with noisy signal, hence version 2 below.
//
// The secondary EMA is meant to ease the transition into a sharp target
// change. It can have a separate alpha which will determine the smoothness of
// this transition.
void rampf_step_filter(
    float *value, float *ramped_step, float target, float step, float smoothing_factor
) {
    float delta = smoothing_factor * (target - *value);

    if (fabsf(delta) > step) {
        delta = step * sign(delta);
    }

    if (fabsf(delta) > fabsf(*ramped_step)) {
        *ramped_step += smoothing_factor * (delta - *ramped_step);
    } else {
        *ramped_step = delta;
    }

    *value += *ramped_step;
}

// A version of the above, which also filters the target value before applying
// the same filter. This (third) EMA could have another alpha of its own.
void rampf_step_filter2(
    float *value,
    float *smooth_target,
    float *ramped_step,
    float target,
    float step,
    float smoothing_factor
) {
    *smooth_target += smoothing_factor * (target - *smooth_target);

    float delta = smoothing_factor * 0.7f * (*smooth_target - *value);

    if (fabsf(delta) > step) {
        delta = step * sign(delta);
    }

    if (fabsf(delta) > fabsf(*ramped_step)) {
        *ramped_step += smoothing_factor * (delta - *ramped_step);
    } else {
        *ramped_step = delta;
    }

    *value += *ramped_step;
}

float clampf(float value, float min, float max) {
    const float m = value < min ? min : value;
    return m > max ? max : m;
}
