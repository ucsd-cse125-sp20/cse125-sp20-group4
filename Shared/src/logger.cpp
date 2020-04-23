#include <filesystem>
#include <mutex>
#include <stdexcept>
#include <vector>

#include "spdlog/async.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "logger.h"

#define ASYNC_QUEUE_SIZE 8192
#define ASYNC_THREADS 1

#define MAX_LOGFILES 5
#define MAX_LOGFILE_SIZE 1024 * 1024 * 10 // 10MB

static std::vector<spdlog::sink_ptr> sinks;

// Need to be wrapped here because C++ is stupid
static std::mutex & getMutex( void ) {

    static std::mutex mtx;
    return mtx;

}

void initLogging( std::string logfile, spdlog::level::level_enum loglevel ) {

    std::lock_guard<std::mutex> lck( getMutex() );

    static bool init = false;
    if ( init ) {
        throw std::runtime_error( "Logging already initialized." );
    }

    spdlog::set_level( loglevel );
    spdlog::set_pattern( "[%Y-%m-%d %H:%M:%S.%e] [%t] [%n] [%^%l%$] %v" );

    spdlog::info( "Initializing logging configuration." );

    // Console sink
    sinks.push_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
    spdlog::info( "Sending logs to stderr with colors" );

    // Main file sink
    try {
        std::filesystem::path logPath = logfile;
        std::string localPath = logPath.make_preferred().string();
        std::filesystem::create_directories( logPath.parent_path() );
        sinks.push_back( std::make_shared<spdlog::sinks::rotating_file_sink_mt>( localPath, MAX_LOGFILE_SIZE, MAX_LOGFILES, true ) );
        spdlog::info( "Sending logs to file {}", localPath );
    } catch ( spdlog::spdlog_ex & e ) {
        spdlog::error( "Could not open target log file: {}", e.what() );
    }

    // Apply new sinks to any existing loggers
    spdlog::apply_all( []( std::shared_ptr<spdlog::logger> l ) {

        std::vector<spdlog::sink_ptr> & lSinks = l->sinks();
        lSinks.clear();
        for ( spdlog::sink_ptr sink : sinks ) {

            lSinks.push_back( sink );

        }

    } );

    init = true;

}

void shutdownLogging( void ) {

    spdlog::shutdown();

}

/**
 * Initialize settings that need to be set before loggers are constructed.
 */
void initLoggerInternal( void ) {

    static bool init = false;
    if ( init ) {
        return;
    }

    spdlog::info( "Initializing internal logger configuration." );

    spdlog::init_thread_pool( ASYNC_QUEUE_SIZE, ASYNC_THREADS );

    init = true;

}

std::shared_ptr<spdlog::logger> getLogger( const std::string & name ) {

    std::lock_guard<std::mutex> lck( getMutex() );

    auto logger = spdlog::get( name );
    if ( !logger ) {
        spdlog::trace( "Creating logger '{}'", name );

        initLoggerInternal();
        logger = std::make_shared<spdlog::async_logger>( name, begin( sinks ), end( sinks ), spdlog::thread_pool(), spdlog::async_overflow_policy::block );

        spdlog::initialize_logger( logger ); // Initialize with global settings and register
    }
    return logger;

}

std::shared_ptr<spdlog::logger> getLogger( const std::string & name, spdlog::level::level_enum loglevel ) {

    std::shared_ptr<spdlog::logger> logger = getLogger( name );
    logger->set_level( loglevel );
    return logger;

}