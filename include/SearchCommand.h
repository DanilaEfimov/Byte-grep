#pragma once

#include "Command.h"
#include "Byte.h"
#include "Pattern.h"

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class SearchCommand : public Command
{
public:
	using bpos_t = Byte::position;

	enum Mode {
		FirstOf,
		LastOf,
		AllOf
	};

	struct GlobalPosition {
		fs::path filename;
		bpos_t relative;

		GlobalPosition(const fs::path& filename, bpos_t relative)
			: filename(filename), relative(relative)
		{};
		GlobalPosition()
			: filename(""), relative()
		{};
	};

private:
	fs::path path;
	Pattern pattern;
	Mode mode;
	bool recursive;

	std::vector<GlobalPosition> positions = {};

	void invoke_find_first_of(const std::vector<fs::path>& targets);
	void invoke_find_last_of(const std::vector<fs::path>& targets);
	void invoke_find_all_of(const std::vector<fs::path>& targets);

	static GlobalPosition find_first_of(const fs::path& filename, const Pattern& pattern);
	static GlobalPosition find_last_of(const fs::path& filename, const Pattern& pattern);
	static std::vector<GlobalPosition> find_all_of(const fs::path& filename, const Pattern& pattern);

public:

	SearchCommand(const std::string& path, Mode m, Pattern pattern, bool recursive = false);
	virtual ~SearchCommand() = default;

	virtual void execute() override;

	std::vector<fs::path> pathsToSearch() const;
	static std::vector<fs::path> pathsToSearchAt(const fs::path& directory);

	const std::vector<GlobalPosition>& getPositions() const;
};
