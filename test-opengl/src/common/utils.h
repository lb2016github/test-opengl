#ifndef _UTILS_H
#define _UTILS_H

#define SAFE_DELETE(p) if(p) {delete p; p = NULL;}
#define ELEMENTS_COUNT(p) (sizeof(p) / sizeof(p[0]))

#endif // !_UTILS_H
