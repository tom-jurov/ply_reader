// MIT License

// Copyright (c) 2024 Tomas Jurov

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef PLY_READER_HPP
#define PLY_READER_HPP

#include <Eigen/Core>
#include <vector>
#include <string>

using Vector3f = Eigen::Vector3f;

class PLYReader {
public:
    PLYReader(const std::string& filename);

    const std::vector<Vector3f>& get_vertices() const;
    const std::vector<float>& get_timestamps() const;

private:
    bool read_ply_file(const std::string& filename, std::vector<Vector3f>& vertices, std::vector<float>& timestamps);

    std::vector<Vector3f> vertices;
    std::vector<float> timestamps;
};

#endif // PLY_READER_HPP