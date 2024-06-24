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
#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <Eigen/Core>
#include <memory>
#include <vector>
#include <thread>
#include <future>

#include <ply_reader.hpp>

namespace py = pybind11;
using namespace py::literals;

PYBIND11_MODULE(ply_reader, m) {
    py::class_<PLYReader>(m, "PLYReader")
        .def(py::init<const std::string&>())
        .def("get_vertices", [](const PLYReader &self) -> py::array_t<float> {
            const std::vector<Eigen::Vector3f>& vertices = self.get_vertices();
            const float* data_ptr = reinterpret_cast<const float*>(vertices.data());

            py::capsule free_when_done(const_cast<float*>(data_ptr), [](void* f) {
                // No action needed because we are not copying data
            });

            return py::array_t<float>({static_cast<int>(vertices.size()), 3},  // shape
                                      {sizeof(float) * 3, sizeof(float)},    // strides
                                      const_cast<float*>(data_ptr),           // pointer
                                      free_when_done);                        // capsule
        })
        .def("get_timestamps", [](const PLYReader &self) -> py::array_t<float> {
            auto &timestamps = self.get_timestamps();
            int num_timestamps = static_cast<int>(timestamps.size());

            if (num_timestamps == 0) {
                return py::none();  // Return None if timestamps vector is empty
            }

            // Create a numpy array without copying data
            py::array_t<float> timestamps_array({num_timestamps}, timestamps.data());
            return timestamps_array;
        });
}
