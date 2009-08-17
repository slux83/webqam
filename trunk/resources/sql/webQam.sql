CREATE TABLE folder
(
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    name TEXT NOT NULL,
    parent INTEGER
);

DELETE FROM sqlite_sequence;

INSERT INTO sqlite_sequence VALUES('folder',0);

CREATE TABLE webcam
(
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    folder INTEGER NOT NULL,
    url TEXT NOT NULL,
    name TEXT NOT NULL
);

INSERT INTO webcam VALUES(1,0,'http://www.parislive.net/eiffelwebcam1.jpg','Tour Eiffel');

INSERT INTO webcam VALUES(2,0,'http://www.roccadicambio.it/webcam/test_mod.jpg','Rocca Di Cambio');

INSERT INTO webcam VALUES(3,0,'http://www.avendano.org/cam.jpg','Madrid');

INSERT INTO webcam VALUES(4,0,'http://www.campaniameteo.it/webcam/palinuroconchiglia/current.jpg','Palinuro');

INSERT INTO sqlite_sequence VALUES('webcam',4);

CREATE UNIQUE INDEX folder_buk on folder (name ASC);

CREATE UNIQUE INDEX webcam_buk on webcam (url ASC);
