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

#ifndef ZEBRA_ASSEMBLER_HPP
#define ZEBRA_ASSEMBLER_HPP

#include "sampler.hpp"

namespace zebra {
    class Assembler;
}

class zebra::Assembler {
    const Parameters &p;

    List<cv::Mat> depths;

    List<cv::Mat> errors;

public:
    Assembler(const Sampler &a);

    void add(const J &j, const L &l, const G &g, size_t d);

    vgram::output<cv::Mat> operator () () const;
};

#endif
