/* Title: Database example schema */

CREATE TABLE BCHS
(
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    NUM INTEGER NOT NULL,
    STR TEXT    NOT NULL
);

INSERT INTO BCHS(NUM, STR)
VALUES (5, 'foo'),
       (7, 'bar');
