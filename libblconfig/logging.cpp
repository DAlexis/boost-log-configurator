#include "logging.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/expressions/formatters/if.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/record_ostream.hpp>

#define UNUSED_ARG(x) (void)(x)

boost::log::sources::severity_logger_mt< boost::log::trivial::severity_level > globalLogger;

void initLoging(
    const std::string& filename,
    trivial::severity_level releaseSeverityMin,
    trivial::severity_level debugSeverityMin,
    size_t rotationSize,
    size_t minFreeSpace
)
{
    namespace expressions = boost::log::expressions;
    namespace sinks = boost::log::sinks;
    namespace keywords = boost::log::keywords;
    namespace trivial = boost::log::trivial;
    namespace attributes = boost::log::attributes;
    boost::shared_ptr<boost::log::core> log_core = boost::log::core::get();

    using TextSink = sinks::synchronous_sink<sinks::text_file_backend>;

    boost::shared_ptr<sinks::text_file_backend> fileBackend =
        boost::make_shared<sinks::text_file_backend>(
            // log filename pattern
            keywords::file_name = filename +
                                "_%Y-%m-%d_%H-%M-%S.%N.log",
            // rotate log when reaching 16MiB
            keywords::rotation_size = rotationSize,
            // or at midnight
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            // keep at least 3GiB free space
            keywords::min_free_space = minFreeSpace
        );
    fileBackend->auto_flush(true);

    boost::shared_ptr<TextSink> fileSink(new TextSink(fileBackend));

    
    using StreamSink = sinks::synchronous_sink<sinks::text_ostream_backend>;

    boost::shared_ptr<StreamSink> stdoutSink = boost::make_shared<StreamSink>();

    auto format = (
        expressions::stream //<< optionalFields
             << "# " << expressions::attr< unsigned int >("LineID") << " |"
            << std::setw(7) << logging::trivial::severity
            << " | "
            << expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f ")
            << expressions::if_(expressions::has_attr<unsigned int>("Worker"))
                [
                    expressions::stream << "[Worker: " << expressions::attr<unsigned int>("Worker") << "] "
                ]
            << expressions::if_(expressions::has_attr<std::string>("Job"))
                [
                    expressions::stream << "[Job: " << expressions::attr<std::string>("Job") << "] "
                ]
            << "> " << expressions::smessage
            << expressions::if_(trivial::severity >= trivial::warning)
                [
                      expressions::stream << "\n\tThis message scopes backtrace: \n\t" << expressions::format_named_scope("Scopes",
                         boost::log::keywords::format = "%n (%f:%l)\n\t",
                        boost::log::keywords::iteration = expressions::forward,
                        boost::log::keywords::depth = 0)
                ]
    );

    boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());

    stdoutSink->set_formatter(format);
    stdoutSink->locked_backend()->add_stream(stream);

    fileSink->set_formatter(format);

    log_core->add_sink(stdoutSink);
    log_core->add_sink(fileSink);

    boost::log::add_common_attributes();
    log_core->add_global_attribute("Scopes", attributes::named_scope());


#ifdef DEBUG
    logging::core::get()->set_filter
    (
        logging::trivial::severity >= debugSeverityMin
    );
    UNUSED_ARG(releaseSeverityMin);
#else
    logging::core::get()->set_filter
    (
        logging::trivial::severity >= releaseSeverityMin
    );
    UNUSED_ARG(debugSeverityMin);
#endif
}
