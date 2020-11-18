#ifndef ERROR_H_
#define ERROR_H_

/* generic constants */
#define PATH_BUFSIZE    50
#define ID_BUFSIZE      20

/* success constants */
#define SUCCESS 0

/* error constants */

/* error for file open/close errors  */
#define EFILE    (1 << 1)
/* error for an invalid value        */
#define EVAL     (1 << 2)
/* error for memory-related problems */
#define EMEM     (1 << 3)

#endif /* ERROR_H_ */
