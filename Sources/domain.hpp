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

#ifndef ZEBRA_DOMAIN_HPP
#define ZEBRA_DOMAIN_HPP

#include <clarus/clarus.hpp>

#include <opencv2/opencv.hpp>

namespace zebra {
    const int CHANNELS = 4;

    const int LEVELS = 10;

    const int SIDE = 16;

    const int BITS = SIDE * SIDE;

    typedef cv::Mat X;

    typedef cv::Mat Y;

    typedef vgram::bitstring<BITS> B;

    typedef float G;

    typedef Index<2> J;

    typedef Index<2> L;

    typedef Index<3> S;
}

#endif
