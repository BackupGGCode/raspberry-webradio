#ifndef _MACROS_H_
#define _MACROS_H_

// enclose function in this to avaid warn_unused_result warning
#define ignore_result(x) ({ typeof(x) z = x; (void)sizeof z; })


#endif

