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

#ifndef ZEBRA_DISPATCHER_HPP
#define ZEBRA_DISPATCHER_HPP

#include "extractor.hpp"
#include "sampler.hpp"

namespace zebra {
    class Dispatcher;
}

class zebra::Dispatcher {
    typedef std::pair<J, L> Indices;

    ListIteratorConst<Indices> i;

public:
    Dispatcher(const Sampler &a);

    Dispatcher(const Sampler &a, const Extractor &b);

    bool more() const;

    void next();

    const J &input() const;

    const L &neuron() const;
};

#endif
