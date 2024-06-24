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
#include <ply_reader.hpp>
#include <iostream>
#include <miniply.h>
#include <unordered_map>
#include <array> // Include array for props_to_look_for

PLYReader::PLYReader(const std::string& filename) {
    if (!read_ply_file(filename, vertices, timestamps)) {
        throw std::runtime_error("Failed to read PLY file.");
    }
}

const std::vector<Vector3f>& PLYReader::get_vertices() const {
    return vertices;
}

const std::vector<float>& PLYReader::get_timestamps() const {
    return timestamps;
}

bool PLYReader::read_ply_file(const std::string& filename, std::vector<Vector3f>& vertices, std::vector<float>& timestamps) {
    miniply::PLYReader reader(filename.c_str());

    if (!reader.valid()) {
        std::cerr << "Failed to open PLY file." << std::endl;
        return false;
    }

    // Find the vertex element
    const miniply::PLYElement* vertex_elem = nullptr;
    for (size_t i = 0; i < reader.num_elements(); ++i) {
        const auto* elem = reader.get_element(i);
        if (std::string(elem->name) == "vertex") {
            vertex_elem = elem;
            break;
        }
    }

    if (!vertex_elem) {
        std::cerr << "Vertex element not found in PLY file." << std::endl;
        return false;
    }

    reader.load_element();

    // Get the count of vertices
    uint32_t vertex_count = vertex_elem->count;

    if (vertex_count == 0) {
        std::cerr << "No vertices found in PLY file." << std::endl;
        return false;
    }

    // Precompute property indices
    std::unordered_map<std::string, int> property_indices;
    for (const auto& prop : vertex_elem->properties) {
        property_indices[prop.name] = static_cast<int>(property_indices.size());
    }

    // Use array for properties to look for
    const std::array<const char*, 3> props_to_look_for = {"timestamp", "t", "time"};

    bool found_timestamps = false;
    std::vector<uint32_t> prop_idxs(4, miniply::kInvalidIndex);

    for (size_t i = 0; i < props_to_look_for.size(); ++i) {
        auto it = property_indices.find(props_to_look_for[i]);
        if (it != property_indices.end()) {
            prop_idxs[3] = static_cast<uint32_t>(it->second);
            found_timestamps = true;
            break;
        }
    }

    prop_idxs[0] = vertex_elem->find_property("x");
    prop_idxs[1] = vertex_elem->find_property("y");
    prop_idxs[2] = vertex_elem->find_property("z");

    if (std::any_of(prop_idxs.begin(), prop_idxs.begin() + 3, [](uint32_t idx) { return idx == miniply::kInvalidIndex; })) {
        std::cerr << "Required properties not found in PLY file." << std::endl;
        return false;
    }

    // Allocate buffer for properties
    std::vector<float> buffer(found_timestamps ? 4 * vertex_count : 3 * vertex_count);
    if (!reader.extract_properties(prop_idxs.data(), found_timestamps ? 4 : 3, miniply::PLYPropertyType::Float, buffer.data())) {
        std::cerr << "Failed to extract properties." << std::endl;
        return false;
    }

    // Allocate memory for vertices and timestamps
    vertices.resize(vertex_count);
    if (found_timestamps) {
        timestamps.resize(vertex_count);
    }

    // Process vertices and timestamps
    for (uint32_t i = 0; i < vertex_count; ++i) {
        vertices[i] = Eigen::Vector3f(buffer[i * (found_timestamps ? 4 : 3)], buffer[i * (found_timestamps ? 4 : 3) + 1], buffer[i * (found_timestamps ? 4 : 3) + 2]);
        if (found_timestamps) {
            timestamps[i] = buffer[i * 4 + 3];
        }
    }

    return true;
}
