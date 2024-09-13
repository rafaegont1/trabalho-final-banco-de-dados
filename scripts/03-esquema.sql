CREATE TABLE patogeno_tipo (
	id INT auto_increment PRIMARY KEY,
	nome VARCHAR(32) NOT NULL UNIQUE
);

CREATE TABLE patogenos (
	id INT auto_increment PRIMARY KEY,
	nome VARCHAR(32) NOT NULL UNIQUE,
	id_tipo INT,
	CONSTRAINT FOREIGN KEY (id_tipo) REFERENCES patogeno_tipo(id)
);

CREATE TABLE doencas (
	id INT auto_increment PRIMARY KEY,
	nome VARCHAR(32) NOT NULL UNIQUE,
	cid VARCHAR(8) NOT NULL UNIQUE,
	id_patogeno int,
	CONSTRAINT FOREIGN KEY (id_patogeno) REFERENCES patogenos(id)
);

CREATE TABLE sintomas (
	id INT auto_increment PRIMARY KEY,
	nome VARCHAR(32) NOT NULL UNIQUE
);

CREATE TABLE doenca_sintoma (
	id_doenca INT NOT NULL,
	id_sintoma INT NOT NULL,
	ocorrencia ENUM('muito raro', 'raro', 'pouco comum', 'comum', 'muito comum'),
	PRIMARY KEY (id_doenca, id_sintoma),
	CONSTRAINT FOREIGN KEY (id_doenca) REFERENCES doencas(id),
	CONSTRAINT FOREIGN KEY (id_sintoma) REFERENCES sintomas(id)
);

CREATE TABLE nomes_pop (
	id INT auto_increment PRIMARY KEY,
	id_doenca INT NOT NULL,
	nome VARCHAR(32) NOT NULL UNIQUE,
	CONSTRAINT FOREIGN KEY (id_doenca) REFERENCES doencas(id)
);
