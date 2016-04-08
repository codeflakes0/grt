#ifndef TESTAPP_ANDROIDBUF_H
#define TESTAPP_ANDROIDBUF_H

#ifdef __ANDROID__
// __ANDROID__ must be defined with a -D option when calling swig

#include <iostream>
#include <android/log.h>

using namespace std;


namespace GRT{

class AndroidBuf: public std::streambuf {
public:
    enum { bufsize = 1024 }; // ... or some other suitable buffer size
    AndroidBuf();
private:
    int overflow(int c);
    int sync();
    char buffer[bufsize];
};

} // end of namespace GRT

#endif //__ANDROID__
#endif
