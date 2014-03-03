/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of ZEBRA.

ZEBRA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ZEBRA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ZEBRA. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ZEBRA_PARAMETERS_HPP
#define ZEBRA_PARAMETERS_HPP

#include "domain.hpp"

#include <map>

namespace zebra {
    struct Parameters;
}

struct zebra::Parameters {
    const size_t r;

    const int m_N;

    const int n_N;

    const int m_H;

    const int n_H;

    const int m_P;

    const int n_P;

    List<std::pair<J, L> > indices;

    std::map<S, point::sampler> synapses;

    Parameters(size_t length, int samples, double sigma, const cv::Size &images, const cv::Size &depths);
};

#endif
