#include "Pattern.h"
#include "MaskReader.h"
#include "SearchCommand.h"
#include "CountCommand.h"
#include "common.h"

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

	po::options_description desc("supported options");
	desc.add_options()
		("help,h", "show this message")
		("input,i", po::value<std::string>()->required(), "file or directory to search")
		("pattern,p", po::value<std::string>()->required(), "bytes mask to search matches")
		("count,c", "only count output required trigger")
		("mode,m", po::value<std::string>()->default_value("first-of"), "searching mode (first-of, last-of or all-of)");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	
	po::notify(vm);

	try {
		Pattern pattern = MaskReader::readMask(vm["pattern"].as<std::string>());
		fs::path input = vm["input"].as<std::string>();

		bool recursive = isDirectoryInput(input);

		if (vm.count("count")) {
			CountCommand cc(input, pattern);
			cc.execute();
			size_t res = cc.getCount();

			std::cout << "pattern '" << std::string(pattern) << "' was found:" << std::endl;
			std::cout << res << " times" << std::endl;
			
			return 0;
		}

		SearchCommand::Mode mode = parseMode(vm["mode"].as<std::string>());
		SearchCommand sc(input.string(), mode, pattern, recursive);

		sc.execute();
		const auto& positions = sc.getPositions();

		std::cout << "pattern '" << std::string(pattern) << "' was found:" << std::endl;
		std::cout << positions.size() << " times" << std::endl;
		for (const auto& pos : positions) {
			std::cout << pos.filename << ", " << std::string(pos.relative) << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
