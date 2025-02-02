/* Title: Database example */

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#include <ksql.h>

int
 main(void)
 {
 	struct ksql	*sql;
 	struct ksqlstmt	*stmt;
 	int64_t		 hits;

 	/* First, initialise, open, read, then close the db. */

 	sql = ksql_alloc(NULL);
 	ksql_open(sql, "db.db");
 	ksql_stmt_alloc(sql, &stmt,
 		"INSERT INTO BCHS(NUM, STR) VALUES (99,'Hello Suckers!!!')", 0);
 	ksql_stmt_step(stmt);
 	ksql_stmt_free(stmt);
 	ksql_stmt_alloc(sql, &stmt,
 		"SELECT NUM FROM BCHS", 0);
 	ksql_stmt_step(stmt);
 	hits = ksql_stmt_int(stmt, 0);
 	ksql_stmt_free(stmt);
 	ksql_free(sql);

 	/* Now print the result. */

 	puts("Status: 200 OK\r");
 	puts("Content-Type: text/html\r");
 	puts("\r");
 	printf("Hello, world: %" PRId64 "\n", hits);

 	return(EXIT_SUCCESS);
 }