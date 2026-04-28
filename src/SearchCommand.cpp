#include "SearchCommand.h"
#include "Selector.h"
#include "Translator.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

std::vector<fs::path> SearchCommand::pathsToSearch() const
{
	std::vector<fs::path> res = {};

	if (this->recursive) {
		try {
			res = this->pathsToSearchAt(this->path);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			res = { };
		}
	}
	else {
		res = { this->path };
	}

	return res;
}

std::vector<fs::path> SearchCommand::pathsToSearchAt(const fs::path& directory)
{
	if (!fs::exists(directory))
		throw std::invalid_argument("SearchCommandAt::pathsToSearch<const path&>:: directory does not exist");

	std::vector<fs::path> res = {};

	if (!fs::is_directory(directory))
		throw std::invalid_argument("SearchCommandAt::pathsToSearchAt<const path&>:: recursive mode can not be applied for a file");
	
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (fs::is_regular_file(entry))
			res.push_back(entry);
		if (fs::is_directory(entry)) {
			auto paths = SearchCommand::pathsToSearchAt(entry);
			res.insert(res.end(), paths.begin(), paths.end());
		}
	}
	
	return res;
}

void SearchCommand::invoke_find_first_of(const std::vector<fs::path>& targets)
{
	for (const auto& target : targets) {
		try {
			GlobalPosition gp = SearchCommand::find_first_of(target, this->pattern);
			this->positions.push_back(gp);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

void SearchCommand::invoke_find_last_of(const std::vector<fs::path>& targets)
{
	for (const auto& target : targets) {
		try {
			GlobalPosition gp = SearchCommand::find_last_of(target, this->pattern);
			this->positions.push_back(gp);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

void SearchCommand::invoke_find_all_of(const std::vector<fs::path>& targets)
{
	for (const auto& target : targets) {
		try {
			auto gp = SearchCommand::find_all_of(target, this->pattern);
			this->positions.insert(this->positions.end(), gp.begin(), gp.end());
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

}

SearchCommand::GlobalPosition SearchCommand::find_first_of(const fs::path& filename, const Pattern& pattern)
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios_base::binary);
	if (!file.is_open()) {
		std::string name = filename.string();
		throw std::runtime_error("SearchCommand::find_first_of<const path&, const Pattern&>:: failed to open file \"" + name + "\"");
	}

	GlobalPosition gp;
	gp.filename = filename;

	Translator ts(file);
	std::vector<Byte> bytes = ts.get();
	auto pos = Selector::first_of(bytes, pattern);
	gp.relative = Byte::position{ pos };

	return gp;
}

SearchCommand::GlobalPosition SearchCommand::find_last_of(const fs::path& filename, const Pattern& pattern)
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios_base::binary);
	if (!file.is_open()) {
		std::string name = filename.string();
		throw std::runtime_error("SearchCommand::find_last_of<const path&, const Pattern&>:: failed to open file \"" + name + "\"");
	}

	GlobalPosition gp;
	gp.filename = filename;

	Translator ts(file);
	std::vector<Byte> bytes = ts.get();
	auto pos = Selector::last_of(bytes, pattern);
	gp.relative = Byte::position{ pos };

	return gp;
}

std::vector<SearchCommand::GlobalPosition> SearchCommand::find_all_of(const fs::path& filename, const Pattern& pattern)
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios_base::binary);
	if (!file.is_open()) {
		std::string name = filename.string();
		throw std::runtime_error("SearchCommand::find_all_of<const path&, const Pattern&>:: failed to open file \"" + name + "\"");
	}

	std::vector<GlobalPosition> gp;

	Translator ts(file);
	std::vector<Byte> bytes = ts.get();

	auto positions = Selector::all_of(bytes, pattern);
	for (const auto& pos : positions) {
		gp.push_back(
			GlobalPosition(filename, Byte::position{ pos })
		);
	}

	return gp;
}

SearchCommand::SearchCommand(const std::string& path, Mode m, Pattern pattern, bool recursive)
	: Command(),
	path(path), 
	mode(m), 
	pattern(pattern), 
	recursive(recursive), 
	positions()
{}

void SearchCommand::execute()
{
	const auto& targets = this->pathsToSearch();

	switch (this->mode) {
	case Mode::FirstOf: 
		this->invoke_find_first_of(targets);
		return;
	case Mode::LastOf: 
		this->invoke_find_last_of(targets);
		return;
	case Mode::AllOf: 
		this->invoke_find_all_of(targets);
		return;
	default:
		throw std::invalid_argument("SearchCommand::execute<>:: undefined searching mode");
	}
}

const std::vector<SearchCommand::GlobalPosition>& SearchCommand::getPositions() const
{
	return this->positions;
}
