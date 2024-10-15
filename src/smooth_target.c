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

#include "smooth_target.h"

#include "utils.h"

#include <math.h>

void smooth_target_configure(SmoothTarget *st, CfgSmoothTarget *cfg) {
    st->cfg = *cfg;
}

void smooth_target_reset(SmoothTarget *st, float value) {
    st->target = 0;
    st->step = 0;
    st->value = value;
}

void smooth_target_update(SmoothTarget *st, float target) {
    st->target += st->cfg.smooth_alpha * (target - st->target);

    float delta = st->cfg.alpha * (st->target - st->value);

    if (fabsf(delta) > fabsf(st->step) || sign(delta) != sign(st->step)) {
        if (fabsf(st->target) > fabsf(st->value)) {
            st->step += st->cfg.in_alpha_away * (delta - st->step);
        } else {
            st->step += st->cfg.in_alpha_back * (delta - st->step);
        }
    } else {
        st->step = delta;
    }

    st->value += st->step;
}
