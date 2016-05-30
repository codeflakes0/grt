#include "Log.h"

namespace GRT{

const Log& Log::operator<< (const bool val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << proceedingText.c_str() << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const short val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const unsigned short val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const int val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const unsigned int val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const long val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;    }

const Log& Log::operator<< (const unsigned long val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const unsigned long long val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const float val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;    }

const Log& Log::operator<< (const double val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;
}

const Log& Log::operator<< (const long double val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
        *lastMessagePtr += Util::toString(val);
    }
    return *this;    }

const Log& Log::operator<< (const void* val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val;
    }
    return *this;
}

const Log& Log::operator<< (const std::string val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            *lastMessagePtr = "";
            std::cout << proceedingText.c_str();
        }
        std::cout << val.c_str();
        *lastMessagePtr += val;
    }
    return *this;
}

const Log& Log::operator<< (const char* val ) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        if( *writeProceedingTextPtr ){
            *writeProceedingTextPtr = false;
            std::cout << proceedingText.c_str();
            *lastMessagePtr = "";
        }
        std::cout << val;
        *lastMessagePtr += val;
    }
    return *this;
}

// define an operator<< to take in std::endl
const Log& Log::operator<<(const StandardEndLine manip) const{
    if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;

    if( *loggingEnabledPtr && instanceLoggingEnabled ){
        // call the function, but we cannot return it's value
        manip(std::cout);
        *writeProceedingTextPtr = true;

        //Trigger any logging callbacks
        triggerCallback( lastMessage );
    }

    return *this;
}
}

