#include "AndroidBuf.h"
#include <iostream>
#ifdef __ANDROID__
#include <sys/system_properties.h>
#include "Log.h"

using namespace std;

namespace GRT{

AndroidBuf::AndroidBuf() {
    static char prop[PROP_VALUE_MAX] = "NOT_SET";
    __system_property_get("log.tag.GRT_C", prop);

    __android_log_print(ANDROID_LOG_INFO, "GRT", "init AndroidBuf() prop=%s", prop);
    this->setp(buffer, buffer + bufsize - 1);
}

int AndroidBuf::overflow(int c) {
        if (c == traits_type::eof()) {
            *this->pptr() = traits_type::to_char_type(c);
            this->sbumpc();
        }
        return this->sync()? traits_type::eof(): traits_type::not_eof(c);
    }

int AndroidBuf::sync() {
        int rc = 0;
        if (this->pbase() != this->pptr()) {
            __android_log_print(ANDROID_LOG_INFO,
                                "GRT",
                                "%s",
                                std::string(this->pbase(),
                                            this->pptr() - this->pbase()).c_str());
            rc = 0;
            this->setp(buffer, buffer + bufsize - 1);
        }
        return rc;
    }

static bool bAndroidLogEnabled = false;

void enableAndroidLog() {
    if(bAndroidLogEnabled) return;

    bAndroidLogEnabled = true;
    __android_log_print(ANDROID_LOG_INFO, "GRT", "enableAndroidLog");
    std::cout.rdbuf(new GRT::AndroidBuf); // leak AndroidBuf memory if disableAndroidLog is not called
}

void disableAndroidLog() {
    if(bAndroidLogEnabled) {
        delete std::cout.rdbuf(0);
        bAndroidLogEnabled = false;
    }
}

std::ostream& operator<<(std::ostream& os, const Log& l) {
    //if(!isAndroidLogLevelEnabled(androidLogLevel)) return *this;
    os << "test";
    return os;
}
}
#endif
