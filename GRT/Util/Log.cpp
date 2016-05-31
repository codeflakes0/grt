#include "Log.h"

GRT_BEGIN_NAMESPACE

const Log& Log::operator<<(const StandardEndLine manip) const{

        if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;
        
        #ifdef GRT_CXX11_ENABLED
        std::unique_lock<std::mutex> lock( logMutex );
        #endif

        if( *loggingEnabledPtr && instanceLoggingEnabled ){
            // call the function, but we cannot return it's value
            manip(std::cout);
            *writeProceedingTextPtr = true;
            
            //Trigger any logging callbacks
            triggerCallback( lastMessage );
        }
        
        return *this;
    }

GRT_END_NAMESPACE


