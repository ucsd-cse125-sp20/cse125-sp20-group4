#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"

void initLogging( std::string logfile, spdlog::level::level_enum loglevel = spdlog::level::info );
void shutdownLogging( void );
std::shared_ptr <spdlog::logger> getLogger( const std::string & name );

#endif // LOGGER_H