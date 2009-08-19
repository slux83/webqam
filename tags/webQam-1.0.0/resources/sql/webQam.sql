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

INSERT INTO webcam VALUES(2,0,'http://roofcam.thestrandoceandrive.com/axis-cgi/jpg/image.cgi?resolution=QCIF&camera=1&clock=1&date=1','Miami Beach');

INSERT INTO webcam VALUES(3,0,'http://www.avendano.org/cam.jpg','Madrid');

INSERT INTO webcam VALUES(4,0,'http://63.229.55.21/ec_metros/ourcams/millenium2.jpg','New York - Ground Zero');

INSERT INTO webcam VALUES(5,0,'http://www.barcello.it/images/meteo/axis.jpg','Rome');

INSERT INTO webcam VALUES(6,0,'http://images.earthcam.com/ec_metros/ourcams/trafalgarsq.jpg','London - Trafalgar Square');

INSERT INTO sqlite_sequence VALUES('webcam',6);

CREATE UNIQUE INDEX folder_buk on folder (name ASC);

CREATE UNIQUE INDEX webcam_buk on webcam (url ASC);
