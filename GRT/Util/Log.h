/*
GRT MIT License
Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef GRT_LOG_HEADER
#define GRT_LOG_HEADER

#include "GRTTypedefs.h"
//Only include the C++ 11 code if C++11 support it is enabled
#ifdef GRT_CXX11_ENABLED
#include <atomic>
#include <thread>
#include <mutex>
#endif //GRT_CXX11_ENABLED
// TBE
#ifdef __ANDROID__
#include <android/log.h>
#include <sys/system_properties.h>
#endif


GRT_BEGIN_NAMESPACE

class GRT_API Log{
public:
    Log(std::string proceedingText = ""){
        setProceedingText(proceedingText);
        loggingEnabledPtr = NULL;
        instanceLoggingEnabled = true;
        writeProceedingText = true;
        writeProceedingTextPtr = &writeProceedingText;
        lastMessagePtr = &lastMessage;
        #ifdef __ANDROID__
        androidLogLevel = ANDROID_LOG_VERBOSE;
        #endif
    }

    virtual ~Log(){}

    template < class T >
	    const Log& operator<< (const T val ) const{

#ifdef GRT_CXX11_ENABLED
        std::unique_lock<std::mutex> lock( logMutex );
#endif
        
        if( *loggingEnabledPtr && instanceLoggingEnabled ){
            if( *writeProceedingTextPtr ){
                *writeProceedingTextPtr = false;
                std::cout << proceedingText.c_str();
                *lastMessagePtr = "";
            }
            std::cout << val;
            std::stringstream stream;
            stream << val;
            *lastMessagePtr += stream.str();
        }
        return *this;
    }

    // this is the type of std::cout
    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
    
    // this is the function signature of std::endl
    typedef CoutType& (*StandardEndLine)(CoutType&);
    
    // define an operator<< to take in std::endl
    const Log& operator<<(const StandardEndLine manip) const;

    //Getters
    virtual bool getLoggingEnabled() const{ return false; }
    
    bool getInstanceLoggingEnabled() const { return instanceLoggingEnabled; };

    std::string getProceedingText() const{ return proceedingText; }

    virtual std::string getLastMessage() const{ return lastMessage; }
    
    //Setters
    void setProceedingText(const std::string &proceedingText){
        if( proceedingText.length() == 0 ) this->proceedingText = "";
        else this->proceedingText = proceedingText + " ";
    }
    
    bool setEnableInstanceLogging(bool loggingEnabled){
        this->instanceLoggingEnabled = loggingEnabled;
        return true;
    }

private:
    // TBE
    static int _strproperty_to_prio(char* str) {
        #ifdef __ANDROID__
        if(str == NULL) return ANDROID_LOG_DEFAULT;   // 1
        if(str[0] == 'V') return ANDROID_LOG_VERBOSE; // 2
        if(str[0] == 'D') return ANDROID_LOG_DEBUG;   // 3
        if(str[0] == 'I') return ANDROID_LOG_INFO;    // 4
        if(str[0] == 'W') return ANDROID_LOG_WARN;
        if(str[0] == 'E') return ANDROID_LOG_ERROR;   // 6
        return ANDROID_LOG_DEFAULT;
        #else
        return 0;
        #endif
    }

protected:

    // TBE
   bool isAndroidLogLevelEnabled(int logLevel) const {
        #ifdef __ANDROID__
        static char prop[PROP_VALUE_MAX] = "FALSE";
        __system_property_get("log.tag.GRT_C", prop);
        return logLevel>=_strproperty_to_prio(prop);
        #else
        return true;
        #endif
   }

    virtual void triggerCallback( const std::string &message ) const{
        return;
    }

    template< class T >
    std::string to_str( const T &val ) const {
        std::stringstream s;
        s << val;
        return s.str();
    }
    
    std::string proceedingText;
    std::string lastMessage;
    bool instanceLoggingEnabled;
    bool *loggingEnabledPtr;
    bool *writeProceedingTextPtr;
    std::string *lastMessagePtr;
    bool writeProceedingText;

	// TBE
    int androidLogLevel; // default ANDROID_LOG_VERBOSE
    //friend std::ostream& operator<<(std::ostream& os, const Log& l);

#ifdef GRT_CXX11_ENABLED
	static std::mutex logMutex;
#endif
};

GRT_END_NAMESPACE

#endif //GRT_LOG_HEADER
