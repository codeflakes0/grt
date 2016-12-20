#include "Log.h"

GRT_BEGIN_NAMESPACE

/*
// CDF. Copied from Log.h
template < class T >
const Log& Log::operator<< (const T &val ) const{

        if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

#ifdef GRT_CXX11_ENABLED
        std::unique_lock<std::mutex> lock( logMutex );
#endif
        if( !baseLoggingEnabled ) return *this; //If the base class global logging is disabled, then there is nothing to do
        
        if( *loggingEnabledPtr && instanceLoggingEnabled ){
            if( *writeKeyPtr ){
                *writeKeyPtr = false;
                std::cout << key.c_str();
                std::cout << " ";
                *lastMessagePtr = ""; //Reset the last message
            }
            std::cout << val;
            std::stringstream stream;
            stream << val;
            *lastMessagePtr += stream.str(); //Update the last message
        }
        return *this;
    }
	*/
	
GRT_END_NAMESPACE

