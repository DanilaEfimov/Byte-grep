#pragma once

#include "Command.h"
#include "Pattern.h"

#include <filesystem>

namespace fs = std::filesystem;

class CountCommand : public Command
{
	const fs::path path;
	const Pattern pattern;

	size_t res = 0;

public:
	CountCommand(const fs::path& path, const Pattern& pattern);
	~CountCommand() = default;

	void execute() override;

	int getCount() const;
};

