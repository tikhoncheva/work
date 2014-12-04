#include "../hrd/additionalfunc.h"

std::string returnFilename (const std::string& str)
{
    unsigned int found = str.find_last_of("/\\");
    return  str.substr(found+1);
}


