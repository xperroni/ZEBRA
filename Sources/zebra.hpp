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

#ifndef ZEBRA_HPP
#define ZEBRA_HPP

#include "domain.hpp"
#include "parameters.hpp"

#include "assembler.hpp"
#include "dispatcher.hpp"
#include "extractor.hpp"
#include "sampler.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <string>

namespace zebra {
    namespace fs = boost::filesystem;

    class ZEBRA;
}

class zebra::ZEBRA {
    struct Z {
        typedef zebra::X X;

        typedef zebra::Y Y;

        typedef zebra::B B;

        typedef zebra::G G;

        typedef zebra::J J;

        typedef zebra::L L;

        typedef zebra::Parameters P;

        typedef zebra::Sampler alpha;

        typedef zebra::Extractor beta;

        typedef zebra::Dispatcher gamma;

        typedef zebra::Assembler omega;
    };

    vgram::network<Z> w;

    cv::Size images_size;

    cv::Size depths_size;

public:
    ZEBRA(int samples, double sigma, const cv::Size &images, const cv::Size &depths);

    void train(const std::string &path);

    void test(const std::string &path);
};

#endif
