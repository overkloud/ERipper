#ifndef EXCEPTION_UTILS_H
#define EXCEPTION_UTILS_H

#include <sstream>
#include <iostream>
#include <exception>
#include <string>

#define THROW_EXCEPTION(Type, Reason) {\
std::ostringstream oss; \
oss << Reason; \ 
throw Type(__FILE__, __LINE__, oss.str()); \
\}

namespace Exception {
	
class StdException : public std::exception
{




};

}

#endif 