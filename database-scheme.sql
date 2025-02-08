/* Title: Database example schema */

CREATE TABLE chibis
(
    CCID VARCHAR(128) NOT NULL,
    CCINDEX INTEGER NOT NULL,
    HIT INTEGER,
    PING BOOLEAN
);

INSERT INTO chibis VALUES ('FOO',55,0,TRUE),
                          ('BAR',99,5,FALSE)