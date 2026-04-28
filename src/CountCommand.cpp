#include "CountCommand.h"
#include "SearchCommand.h"
#include "Counter.h"
#include "Translator.h"

#include <fstream>
#include <iostream>

CountCommand::CountCommand(const fs::path& path, const Pattern& pattern)
	: path(path), pattern(pattern), res(0)
{}

void CountCommand::execute()
{
	this->res = 0;

	bool isDir = fs::is_directory(this->path);
	std::vector<fs::path> paths = {};
	if (isDir)
		paths = SearchCommand::pathsToSearchAt(path);
	else
		paths = { this->path };

	for (const auto& filename : paths) {
		std::fstream file;
		file.open(filename, std::ios::in | std::ios_base::binary);
		if (!file.is_open()) {
			std::string name = filename.string();
			throw std::runtime_error("CountCommand::execute<>:: failed to open file \"" + name + "\"");
		}

		try {
			Translator ts(file);
			const auto& bytes = ts.get();

			this->res += Counter::count(bytes, this->pattern);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

int CountCommand::getCount() const
{
	return this->res;
}
