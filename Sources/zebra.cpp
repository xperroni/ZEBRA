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

#include "zebra.hpp"

cv::Mat smoothness(const cv::Mat &gray, size_t w) {
    size_t rows = gray.rows / w;
    size_t cols = gray.cols / w;
    cv::Mat out(rows, cols, CV_64F, cv::Scalar(0));

    cv::Mat image;
    cv::resize(gray, image, cv::Size(cols * w, rows * w));

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            cv::Rect roi(j * w, i * w, w, w);
            cv::Mat patch(gray, roi);

            cv::Mat mean, stdev;
            cv::meanStdDev(patch, mean, stdev);
            out.at<double>(i, j) = stdev.at<double>(0);
        }
    }

    return out;
}

cv::Mat edge_mask(const List<cv::Mat> &channels) {
    cv::Mat edges(channels[0].size(), CV_8U, cv::Scalar(0));
    for (ListIteratorConst<cv::Mat> i(channels); i.more();) {
        const cv::Mat &channel = i.next();
        cv::Mat e = filter::sobel(channel);
        cv::threshold(e, e, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
        edges = edges | e;
    }

    return edges;
}

cv::Mat edge_mask(const cv::Mat &bgr) {
    return edge_mask(colors::BGRL(bgr));
}

cv::Mat otsu_filter(const cv::Mat &bgr) {
    List<cv::Mat> channels = colors::BGRL(bgr);

    cv::Mat filtered(bgr.rows, bgr.cols, CV_8U, cv::Scalar(127));
    for (ListIteratorConst<cv::Mat> i(channels); i.more();) {
        cv::Mat channel = i.next();
        cv::threshold(channel, channel, 0, 32, cv::THRESH_BINARY + cv::THRESH_OTSU);
        filtered += channel;
    }

    return filtered;
}

cv::Mat grates(const cv::Mat &bgr) {
    static cv::Vec3b GREEN(255, 255, 255);
    static cv::Vec3b RED(0, 0, 0);

    cv::Mat l = colors::lightness(bgr);
    int rows = l.rows - l.rows % 2;
    int cols = l.cols - l.cols % 2;

    cv::Mat grated(rows, cols, CV_8UC3, cv::Scalar::all(0));
    for (int i = 0; i < rows; i++) {
        int p = (i % 2);
        int sign = 1 - 2 * p;

        for (int j = 0; j < cols; j++) {
            int q = (j % 2);
            int u = i + sign * (p ^ q);
            int v = j + sign * (p ^ (q ^ 1));

            uint8_t a = l.at<uint8_t>(i, j);
            uint8_t b = l.at<uint8_t>(u, v);

            grated.at<cv::Vec3b>(i, j) = (a > b ? GREEN : RED);
        }
    }

    return grated;
}

cv::Mat redgreen(const cv::Mat &bgr) {
    static cv::Vec3b GREEN(0, 255, 0);
    static cv::Vec3b RED(0, 0, 255);

    int rows = bgr.rows;
    int cols = bgr.cols;

    cv::Mat out(rows, cols, CV_8UC3, cv::Scalar::all(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            const cv::Vec3b &pixel = bgr.at<cv::Vec3b>(i, j);
            out.at<cv::Vec3b>(i, j) = (pixel[1] > pixel[2] ? GREEN : RED);
        }
    }

    return out;
}

cv::Mat blueblack(const cv::Mat &bgr) {
    static cv::Vec3b BLUE(255, 0, 0);
    static cv::Vec3b BLACK(0, 0, 0);

    int rows = bgr.rows;
    int cols = bgr.cols;

    cv::Mat out(rows, cols, CV_8UC3, cv::Scalar::all(0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            const cv::Vec3b &pixel = bgr.at<cv::Vec3b>(i, j);
            out.at<cv::Vec3b>(i, j) = (pixel[0] > std::max(pixel[1], pixel[2]) ? BLUE : BLACK);
        }
    }

    return out;
}

cv::Mat otsu(const cv::Mat &bgr) {
    cv::Mat bw;
    cv::threshold(colors::lightness(bgr), bw, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    return bw;
}

/*
int main() {
    cv::Mat bgr = images::load(
        "/home/helio/Roboken/Data/Saxena/combined1-p-108t0.jpg"
    );

    cv::Mat depthmap = depths::load("/home/helio/Roboken/Data/Saxena/combined1-p-108t0.dat", cv::Size(86, 107));

    cv::Mat values = depths::bgr(depthmap);
    cv::Mat labels = depths::bgr(blob_colouring(depthmap, 1.0));

    cv::Size size(bgr.cols / 4, bgr.rows / 4);
    cv::resize(bgr.clone(), bgr, size);
    cv::resize(values.clone(), values, size);
    cv::resize(labels.clone(), labels, size);

    viewer::show("Viewer 1", bgr);
    viewer::show("Viewer 2", values);
    viewer::show("Viewer 3", labels);

    cv::waitKey();
}
*/

zebra::ZEBRA::ZEBRA(
    int samples,
    double sigma,
    const cv::Size &images,
    const cv::Size &depths
):
    w(Parameters(BITS, samples, sigma, images, depths)),
    images_size(images),
    depths_size(depths)
{
    // Nothing to do.
}

void zebra::ZEBRA::train(const std::string &path) {
    std::cout << "Training..." << std::endl;

    fs::path cases(path);
    fs::path folder(cases.parent_path());
    fs::ifstream file(cases);
    for (std::string name; std::getline(file, name) && name != "";) {
        std::cout << "    " << name << std::endl;
        std::string image_path = (folder / (name + ".jpg")).native();
        std::string depths_path = (folder / (name + ".dat")).native();

        cv::Mat image = cv::imread(image_path);
        cv::Mat depths = depths::load(depths_path, depths_size);
        w.set(image, depths);
    }
}

void zebra::ZEBRA::test(const std::string &path) {
    std::cout << "Testing..." << std::endl;

    fs::path cases(path);
    fs::path folder(cases.parent_path());
    fs::ifstream file(cases);
    for (std::string name; std::getline(file, name) && name != "";) {
        std::cout << "    " << name << std::endl;
        std::string image_path = (folder / (name + ".jpg")).native();

        cv::Mat image = cv::imread(image_path);
        cv::Mat depths = w.get(image).value();

        std::string depths_path = (folder / (name + ".test.dat")).native();
        depths::save(depths_path, depths);

        std::string depths_jpg_path = (folder / (name + ".test.jpg")).native();
        cv::imwrite(depths_jpg_path, depths::bgr(depths, images_size));

        std::string refer_path = (folder / (name + ".dat")).native();
        cv::Mat refer = depths::load(refer_path, depths_size);

        std::string refer_jpg_path = (folder / (name + ".dat.jpg")).native();
        cv::imwrite(refer_jpg_path, depths::bgr(refer, images_size));
    }
}

void train_and_test(const std::string &train, const std::string &test) {
    zebra::ZEBRA z(zebra::LEVELS, 4.0, cv::Size(1704, 2272), cv::Size(86, 107));
    z.train(train);
    z.test(test);
}

int main(int argc, char *argv[]) {
    train_and_test(argv[1], argv[2]);

    return 0;
}
