#ifndef ERROR_H_
#define ERROR_H_

/* generic constants */
#define PATH_BUFSIZE    50
#define ID_BUFSIZE      20

/* success constants */
#define ESUCCESS 0

/* error constants */
#define EFILE    (1 << 1)
#define EVAL     (1 << 2)
#define EMEM     (1 << 3)

#endif /* ERROR_H_ */
