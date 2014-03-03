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

#include "sampler.hpp"


zebra::Sampler::Sampler(const Parameters &_p, const X &x):
    p(_p)
{

    List<X> bgrl = colors::BGRL(x);

    for (ListIteratorConst<std::pair<J, L> > i(p.indices); i.more();) {
        const std::pair<J, L> &index = i.next();
        const J &j = index.first;
        const L &l = index.second;
        B &bits = bitstrings[index];

        int d = 0;
        cv::Rect roi(j[1] * p.n_P, j[0] * p.m_P, p.n_P, p.m_P);
        for (int k = 0, n = bgrl.size(); k < n; k++) {
            S s(l[0], l[1], k);
            std::map<S, point::sampler>::const_iterator si = p.synapses.find(s);
            if (si == p.synapses.end()) {
                throw std::runtime_error(std::string("No synapses found for index ") + (std::string) s);
            }

            const point::sampler &sampler = si->second;
            cv::Mat c_k(bgrl[k], roi);
            cv::Mat samples = sampler.sample<uint8_t>(c_k);
            d = vgram::minchinton<B, uint8_t>(bits, d, samples);
        }
    }
}

const zebra::B &zebra::Sampler::operator () (const J &j, const L &l) const {
    return bitstrings.find(std::make_pair(j, l))->second;
}
