#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <tuple>
#include <point.h>

class FileManager {
public:
    /* Reads the file and creates a vector of pointers to Vec3. */
    static std::vector<Vec3*> read_input_file(const std::string& filename);

    /* Writes facets to a file. */
    static void write_output_file(
        const std::string& filename, 
        const std::vector<std::tuple<int, int, int>>& facets);

    /* Writes points and facets to a obj file. */
    static void write_obj(
        const std::string& filename,
        const std::vector<Vec3*>& points,
        const std::vector<std::tuple<int, int, int>>& facets);
};

#endif