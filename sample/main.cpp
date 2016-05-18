#include "logging.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;

void func1(int x)
{
    BOOST_LOG_FUNCTION();
    switch(x)
    {
    case 0: 
        {
            BOOST_LOG_NAMED_SCOPE("case 0");
            BOOST_LOG_SEV(globalLogger, warning) << "Parameter x is zero";
            break;
        }
    case 1:
        {
            BOOST_LOG_NAMED_SCOPE("case 1");
            BOOST_LOG_SEV(globalLogger, info) << "Parameter x is one";
            break;
        }
    default:
        {
            BOOST_LOG_NAMED_SCOPE("default case");
            BOOST_LOG_SEV(globalLogger, error) << "Parameter x = " << x << " that is not allowed";
            // Here we attaching scopes backtrace to exception to get it in catch() block
            throw ATT_SCOPES_STACK(std::runtime_error("Exception here!!!111"));
            break;
        }
    }
}

int main(int argc, char** argv)
{
    
    initLoging(
        "test-log-file",
        info,
        trace
    );
    
    BOOST_LOG_FUNCTION();
    BOOST_LOG_SEV(globalLogger, info) << "Program started";
    
    BOOST_LOG_SEV(globalLogger, trace) << "This is trace message";
    BOOST_LOG_SEV(globalLogger, debug) << "This is debug message";
    BOOST_LOG_SEV(globalLogger, info) << "This is info message";
    BOOST_LOG_SEV(globalLogger, warning) << "This is warning message";
    BOOST_LOG_SEV(globalLogger, error) << "This is error message";
    BOOST_LOG_SEV(globalLogger, fatal) << "This is fatal message";
    
    try
    {
        func1(1);
        func1(0);
        func1(89);
    }
    catch(const std::exception& ex)
    {
        BOOST_LOG_SEV_EXTRACE(globalLogger, fatal, ex, "Exception during function call: " << ex.what());
    }
    return 0;
}
