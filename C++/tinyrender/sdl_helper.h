#pragma once
#include "context.h"
#include <optional>

std::optional<Context> init(const int width, const int height, const char *title = "SDL-Window");
