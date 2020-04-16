#ifndef LOGGER_H
#define LOGGER_H


#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void initLogging();
std::shared_ptr <spdlog::logger> logger();

#endif // LOGGER_H