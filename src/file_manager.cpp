#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <file_manager.h>

std::vector<Point3*> FileManager::read_input_file(const std::string& filename) {
	std::vector<Point3*> points = std::vector<Point3*>();; // RVO works

	std::ifstream infile(filename);
	if (!infile) {
		throw std::runtime_error("Could not open file: " + filename);
	} else {
		std::cout << "Reading input file: " << filename << std::endl;
	}

	std::string line;
	while (std::getline(infile, line)) {
		// Skip comment lines starting with '*'
		if (line[0] == '*') {
			continue;
		}

		// Parse point 
		std::istringstream iss(line);
		std::string token;
		std::getline(iss, token, ',');
		int number = std::stoi(token);

		std::getline(iss, token, ',');
		auto x = static_cast<float>(std::stod(token));

		std::getline(iss, token, ',');
		auto y = static_cast<float>(std::stod(token));

		std::getline(iss, token);
		auto z = static_cast<float>(std::stod(token));

		Point3* point = new Point3(x, y, z);
		points.push_back(point);
	}
	std::cout << "Read " << points.size() << " points" << std::endl;
	return points;
}

void FileManager::write_output_file(const std::string& filename,
	const std::vector<Point3*>& points,
	const std::vector<std::tuple<int, int, int>>& facets) {
	std::ofstream outfile(filename);
	if (!outfile) {
		throw std::runtime_error("Could not open file: " + filename);
	} else {
		std::cout << "Writing into file: " << filename << std::endl;
	}
	outfile << "*   N,            X             Y             Z\n";
	outfile << "* Nodes\n";
	for (std::size_t i = 0; i < points.size(); ++i) {
		const auto& point = points[i];
		outfile << std::setw(5) << i + 1 << ",";
		outfile << std::setw(12) << point->X << ",";
		outfile << std::setw(12) << point->Y << ",";
		outfile << std::setw(12) << point->Z << "\n";
	}

	outfile << "*Elements\n";
	for (std::size_t i = 0; i < facets.size(); ++i) {
		const auto facet = facets[i];
		outfile << std::setw(5) << i + 1 << ",";
		outfile << std::setw(12) << std::get<0>(facet) + 1 << ",";
		outfile << std::setw(12) << std::get<1>(facet) + 1 << ",";
		outfile << std::setw(12) << std::get<2>(facet) + 1 << "\n";
	}
	std::cout << "Wrote " << facets.size() << " triangles to " << filename << std::endl;
}

void FileManager::write_obj(const std::string& filename,
	const std::vector<Point3*>& points,
	const std::vector<std::tuple<int, int, int>>& facets) {
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	// Write vertex information
	for (const auto& point : points) {
		file << "v " << point->X << " " << point->Y << " " << point->Z << std::endl;
	}

	// Write triangle information
	for (const auto& facet : facets) {
		int v0 = std::get<0>(facet) + 1; // OBJ file format indexes vertices starting from 1, not 0
		int v1 = std::get<1>(facet) + 1;
		int v2 = std::get<2>(facet) + 1;
		file << "f " << v0 << " " << v1 << " " << v2 << std::endl;
	}

	file.close();
}