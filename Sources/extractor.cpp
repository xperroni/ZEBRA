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

#include "extractor.hpp"

zebra::Extractor::Extractor(const Sampler &a, const Y &_y):
    y(_y)
{
    // Nothing to do.
}

zebra::G zebra::Extractor::operator () (const J &j) const {
    return y.at<G>(j[0], j[1]);
}
