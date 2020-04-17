#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"

void shutdownLogging();
std::shared_ptr <spdlog::logger> getLogger( const std::string & name );

#endif // LOGGER_H