#ifndef ERROR_H_
#define ERROR_H_

/*
 * generic constants
 */

/* buffer size for path-related things. Commonly use 2*BUFSIZE */
#define PATH_BUFSIZE    50
/* The extra-large buffer used for ID keys */
#define ID_BUFSIZE      20
/* The format string for reading/writing ID keys */
#define ID_FORMAT       "%06x"

/*
 * success constants
 */

/* Generic success return */
#define SUCCESS 0

/*
 * error constants
 */

/* error prefix string for failure in main */
#define ERRMAIN  "[ERROR] in "

/* error for file open/close errors          */
#define EFILE    (1 << 1)
/* error for an invalid value                */
#define EVAL     (1 << 2)
/* error for memory-related problems         */
#define EMEM     (1 << 3)
/* error for bad database env                */
#define EDBENV   (1 << 4)
/* error for bad database cursors            */
#define EDBCUR   (1 << 5)
/* error for bad database transactions       */
#define EDBTRX   (1 << 6)

/*
 * Path constants. For use with home_prefix()
 */

/* config directory */
#define HOME_DIR "/.ze"
/* config file */
#define CONFIG_FILE HOME_DIR"/ze.ini"
/* ID file */
#define CONFIG_ID HOME_DIR"/current.id"

#endif /* ERROR_H_ */
