#include <iostream>
#include <file_manager.h>
#include <mesh.h>

int parse_arguments(int argc, char* argv[], std::string& input_file, std::string& output_file) {
	// parse command line arguments
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-i" || arg == "--input") {
			if (i + 1 < argc) {
				input_file = argv[i + 1];
				i++;
			} else {
				std::cerr << "Error: argument " << arg << " requires a value" << std::endl;
				return 1;
			}
		} else if (arg == "-o" || arg == "--output") {
			if (i + 1 < argc) {
				output_file = argv[i + 1];
				i++;
			} else {
				std::cerr << "Error: argument " << arg << " requires a value" << std::endl;
				return 1;
			}
		} else {
			std::cerr << "Error: unknown argument " << arg << std::endl;
			return 1;
		}
	}

	// check that required arguments are present
	if (input_file.empty()) {
		std::cerr << "Error: argument -i or --input is required" << std::endl;
		return 1;
	}
	if (output_file.empty()) {
		std::cerr << "Error: argument -o or --output is required" << std::endl;
		return 1;
	}

	return 0;
}

int main(int argc, char** argv) {
	std::string input_file, output_file;

	int status = parse_arguments(argc, argv, input_file, output_file);
	if (status != 0) {
		return status;
	}

	// Read input file
	FileManager file_manager;
	std::vector<Vec3*> points;
	try {
		points = file_manager.read_input_file(input_file);
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}

	// Create mesh
	Mesh mesh = Mesh(points);
	auto facets = mesh.GetFacets();

	// Write output file
	file_manager.write_output_file(output_file, points, facets);
	//file_manager.write_obj("test.obj", points, facets);

	return 0;
}
