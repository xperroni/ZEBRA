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

#include "assembler.hpp"

zebra::Assembler::Assembler(const Sampler &a):
    p(a.p)
{
    for (int z = 0; z < p.n_N; z++) {
        cv::Mat d(p.m_H, p.n_H, CV_32F, cv::Scalar::all(0));
        cv::Mat e(p.m_H, p.n_H, CV_32S, cv::Scalar::all(p.r));

        depths.append(d);
        errors.append(e);
    }
}

void zebra::Assembler::add(const J &j, const L &l, const G &g, size_t d) {
    size_t a = j[0];
    size_t b = j[1];
    size_t z = l[1];

    depths[z].at<float>(a, b) = g;
    errors[z].at<int>(a, b) = d;
}

List<float> options(const List<cv::Mat> &depths, int i, int j) {
    List<float> values;
    for (ListIteratorConst<cv::Mat> k(depths); k.more();) {
        const cv::Mat &estimate = k.next();
        values.append(estimate.at<float>(i, j));
    }

    return values;
}

List<float> neighbors(const cv::Mat &depths, int i, int j) {
    int u0 = std::max(0, i - 1);
    int un = std::min(depths.rows, i + 2);

    int v0 = std::max(0, j - 1);
    int vn = std::min(depths.cols, j + 2);

    List<float> values;
    for (int u = u0; u < un; u++) {
        for (int v = v0; v < vn; v++) {
            if (u != i || v != j) {
                values.append(depths.at<float>(u, v));
            }
        }
    }

    return values;
}

float closest(const List<float> &s1, const List<float> &s2) {
    float winner = 0.0;
    float best = std::numeric_limits<float>::max();
    for (ListIteratorConst<float> i(s1); i.more();) {
        float candidate = i.next();

        float error = 0.0;
        for (ListIteratorConst<float> j(s2); j.more();) {
            float neighbor = j.next();
            error += fabs(candidate - neighbor);
        }

        if (error < best) {
            winner = candidate;
            best = error;
        }
    }

    return winner;
}

cv::Mat winner_take_all(cv::Mat o1, const List<cv::Mat> &depths) {
    int rows = o1.rows;
    int cols = o1.cols;
    for (float e1 = std::numeric_limits<float>::max(), e2 = e1 - 1.0; e1 - e2 > 0;) {
        cv::Mat o2 = o1.clone();
        e1 = e2;
        e2 = 0.0;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                List<float> s1 = options(depths, i, j);
                List<float> s2 = neighbors(o1, i, j);

                float &v = o2.at<float>(i, j);
                float v2 = closest(s1, s2);

                if (v != v2) {
                    e2 += fabs(v - v2);
                    v = v2;
                }

            }
        }

        o1 = o2;
    }

    return o1;
}

vgram::output<cv::Mat> zebra::Assembler::operator () () const {
    cv::Mat y(p.m_H, p.n_H, CV_32F, cv::Scalar(0));
    for (int i = 0; i < p.m_H; i++) {
        for (int j = 0; j < p.n_H; j++) {
            float winner = 0.0;
            int error = p.r;
            for (size_t z = 0; z < LEVELS; z++) {
                int d = errors[z].at<int>(i, j);
                if (d < error) {
                    winner = depths[z].at<float>(i, j);
                    error = d;
                }
            }

            y.at<float>(i, j) = winner;
        }
    }

    y = winner_take_all(y, depths);

    return vgram::output<cv::Mat>(y, cv::mean(errors[0])[0]);
}
