#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

#include <boost/log/core.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/support/exception.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/exception/all.hpp>
#include <fstream>

namespace logging = boost::log;
namespace trivial = boost::log::trivial;

/**
 * Initialize logging. Simply call this function without args to have working logging system
 * @param filename log file name prefix
 * @param releaseSeverityMin Minimal severity level for release configuration (No -DDEBUG)
 * @param debugSeverityMin Minimal severity level for release configuration (-DDEBUG)
 * @param rotationSize Log rotation size. New file will be created every rotationSize bytes
 * @param minFreeSpace Min free disk space that logger should not eat
 */
void initLoging(
    const std::string& filename = std::string("log"),
    trivial::severity_level releaseSeverityMin = logging::trivial::info,
    trivial::severity_level debugSeverityMin = logging::trivial::trace,
    size_t rotationSize = 16 * 1024 * 1024,
    size_t minFreeSpace = 3 * 1024 * 1024
);

using LoggerType = boost::log::sources::severity_logger_mt<trivial::severity_level>;
extern LoggerType globalLogger;


/// Attach boost scopes info to any type of exception
#define ATT_SCOPES_STACK(ex)		boost::enable_error_info(ex) << boost::log::current_scope()

/// Extract scopes trace to stream
#define GET_SCOPES_STACK(ex) 		"\n\tException scopes bt:\n\t" \
									<< *boost::get_error_info<boost::log::current_scope_info>(ex)

/// Extract scopes trace into log after message
#define BOOST_LOG_SEV_EXTRACE(logger, severity, ex, message) \
	do { \
		if (boost::get_error_info<boost::log::current_scope_info>(ex) != nullptr) \
			BOOST_LOG_SEV(logger, severity) << message << "\n\tException scopes bt:\n\t" << *boost::get_error_info<boost::log::current_scope_info>(ex); \
		else \
			BOOST_LOG_SEV(logger, severity) << message << "\n\tException scopes bt not avaliable"; \
	} while(0)

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)


using namespace boost::log::trivial;

#endif // LOGGING_H_INCLUDED
