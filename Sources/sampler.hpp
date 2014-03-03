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

#ifndef ZEBRA_SAMPLER_HPP
#define ZEBRA_SAMPLER_HPP

#include "parameters.hpp"

#include <stdexcept>

namespace zebra {
    class Sampler;
}

class zebra::Sampler {
    std::map<std::pair<J, L>, B> bitstrings;

public:
    const Parameters &p;

    Sampler(const Parameters &p, const X &x);

    const B &operator () (const J &j, const L &l) const;
};

#endif
