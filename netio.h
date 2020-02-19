#ifndef NETIO_H_INCLUDED
#define NETIO_H_INCLUDED 1

#include <stdbool.h>
#include <curl/curl.h>

/* one API fetch. */
struct fetch {
	struct fetch	*next;
	struct query	*query;
	CURL		*easy;
	struct curl_slist  *hdrs;
	char		*url;
	char		*buf;
	size_t		len;
	long		rcode;
};
typedef struct fetch *fetch_t;

/* one query, having one or more API fetches. */
struct query {
	struct query	*next;
	struct fetch	*fetches;
	struct writer	*writer;
	char		*command;
	char		*info_buf;
	size_t		info_len;
	bool		once;
};
typedef struct query *query_t;

/* one output stream, having one or several queries merging into it. */
struct writer {
	struct writer	*next;
	struct query	*queries;
	struct query	*active;
	u_long		after;
	u_long		before;
	FILE		*sort_stdin;
	FILE		*sort_stdout;
	pid_t		sort_pid;
	bool		sort_killed;
	int		count;
	char		*status;
	char		*message;
	bool		once;
};
typedef struct writer *writer_t;

void make_curl(void);
void unmake_curl(void);
void fetch(query_t, char *);
writer_t writer_init(u_long, u_long);
void writer_status(writer_t, const char *, const char *);
size_t writer_func(char *ptr, size_t size, size_t nmemb, void *blob);
void writer_fini(writer_t);
void unmake_writers(void);
void io_engine(int);
void escape(char **);

#endif /*NETIO_H_INCLUDED*/