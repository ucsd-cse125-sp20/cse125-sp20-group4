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
#define LOGFILE_NAME "log/client.log"

static bool init = false;
static std::vector<spdlog::sink_ptr> sinks;

void initLogging() {

    if ( init ) {
        throw std::runtime_error( "Logging is already initialized." );
    }

    spdlog::set_level( spdlog::level::trace );
    spdlog::set_pattern( "[%Y-%m-%d %H:%M:%S.%e] [%t] [%n] [%^%l%$] %v" );

    spdlog::init_thread_pool( ASYNC_QUEUE_SIZE, ASYNC_THREADS );

    // Console sink
    sinks.push_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
    spdlog::info( "Sending logs to stderr with colors" );

    try {
        // Main file sink
        std::filesystem::path logPath = LOGFILE_NAME;
        std::string localPath = logPath.make_preferred().string();
        std::filesystem::create_directories( logPath.parent_path() );
        sinks.push_back( std::make_shared<spdlog::sinks::rotating_file_sink_mt>( localPath, MAX_LOGFILE_SIZE, MAX_LOGFILES, true ) );
        spdlog::info( "Sending logs to file {}", localPath );
    } catch ( spdlog::spdlog_ex & e ) {
        spdlog::error( "Could not open target log file: {}", e.what() );
    }

    std::vector<spdlog::sink_ptr> & defaultSinks = spdlog::default_logger()->sinks();
    defaultSinks.clear();
    for ( spdlog::sink_ptr sink : sinks ) {

        defaultSinks.push_back( sink );

    }

    init = true;

}

void shutdownLogging() {

    spdlog::shutdown();

}

std::shared_ptr <spdlog::logger> getLogger( const std::string & name ) {

    static std::mutex mtx;
    std::lock_guard<std::mutex> lck( mtx );

    if ( !init ) {
        initLogging();
    }

    auto logger = spdlog::get( name );
    if ( !logger ) {
        spdlog::trace( "Creating logger '{}'", name );

        logger = std::make_shared<spdlog::async_logger>( name, begin( sinks ), end( sinks ), spdlog::thread_pool(), spdlog::async_overflow_policy::block );

        spdlog::initialize_logger( logger ); // Initialize with global settings and register
    }
    return logger;

}