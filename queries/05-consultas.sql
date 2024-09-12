-- Após a análise das funções que deverão ser implementadas pelo sistema
-- especialista, foi identificado a necessidade de realizar as seguintes
-- consultas no banco de dados:

-- (a) Consulta para listar todas as doenças e seus respectivos dados. A
-- consulta deve retornar id da doença, seu nome, número CID, e o tipo do
-- patógeno. A sequência em que as colunas serão apresentadas deve manter essa
-- ordem e as linhas organizadas em ordem alfabética em relação ao nome da
-- doença.

SELECT d.id, d.nome, d.cid, t.nome
FROM doencas AS d
JOIN patogenos AS p ON d.id_patogeno = p.id
JOIN patogeno_tipo as t ON p.id_tipo = t.id
GROUP BY d.id
ORDER BY d.nome;

-- (b) Consulta para listar os sintomas de uma doença específica. A consulta
-- deve retornar o nome do sintoma e sua taxa de ocorrência, nessa ordem de
-- colunas, e de forma que as linhas sejam ordenadas pela taxa de ocorrência em
-- que sintomas mais frequentes devem ser posicionados acima dos sintomas menos
-- frequentes (em caso de dois ou mais sintomas com a mesma taxa de ocorrência,
-- deve-se seguir com a ordenação pela ordem alfabética em relação ao nome do
-- sintoma).

SELECT s.nome, ds.ocorrencia
FROM sintomas AS s
JOIN doenca_sintoma AS ds ON s.id = ds.id_sintoma
WHERE ds.id_doenca = 20
ORDER BY
	FIELD(ds.ocorrencia,
		'muito comum', 'comum', 'pouco comum', 'raro', 'muito raro'),
	nome ASC;

-- (c) Consulta para listar todas as doenças e seus respectivos sintomas. A
-- consulta deve retornar id da doença, seu nome, e os seus sintomas (juntamente
-- com a taxa de ocorrência). A sequência em que as colunas serão apresentadas
-- deve manter essa ordem. As linhas devem ser organizadas em ordem alfabética
-- em relação ao nome da doença. Cada doença deve ser apresentada em uma única
-- linha e, para doenças com múltiplos sintomas, eles devem ser disponibilizados
-- em uma única coluna separados por vírgula. Os sintomas devem ser ordenados do
-- muito comum ao muito raro. Para cada sintoma, a sua taxa de ocorrência deve
-- vir entre parênteses, logo em seguida ao nome do sintoma (por exemplo, “Febre
-- (muito comum), Diarreia (raro), Dor no corpo (muito raro)”).

SELECT d.id, d.nome,
	GROUP_CONCAT(
		CONCAT(s.nome, ' (', ds.ocorrencia, ')')
		ORDER BY FIELD(ds.ocorrencia,
			'muito comum', 'comum', 'pouco comum', 'raro', 'muito raro'),
			s.nome
		SEPARATOR ', '
	) AS sintomas
FROM doencas AS d
JOIN doenca_sintoma AS ds ON d.id = ds.id_doenca
JOIN sintomas AS s ON ds.id_sintoma = s.id
GROUP BY d.id
ORDER BY d.nome;

-- (d) Consulta para calcular o número de doenças cadastradas para cada tipo de
-- patógeno. Devem ser apresentados o tipo do patógeno (vírus, bactéria, fungo,
-- ...) e a quantidade de doenças cadastradas no sistema que são causadas pelo
-- respectivo tipo de patógeno. As colunas devem seguir a ordem especificada e
-- as linhas devem ser organizadas em ordem decrescente em relação à quantidade
-- de doenças, seguida pela ordem alfabética em relação ao tipo do patógeno.

SELECT pt.nome AS patogeno, COUNT(*) AS qtd_doencas
FROM doencas AS d
JOIN patogenos AS p ON d.id_patogeno = p.id
JOIN patogeno_tipo AS pt ON p.id_tipo = pt.id
GROUP BY pt.nome
ORDER BY qtd_doencas DESC, patogeno ASC;

-- (e) Consulta para obter algumas estatísticas sobre os dados armazenados no
-- sistema. A consulta deverá apresentar o número de doenças cadastradas, o
-- número de sintomas cadastrados, o número médio de sintomas por doença, o
-- menor número de sintomas de uma doença, o maior número de sintomas de uma
-- doença. As colunas devem ser apresentadas nessa ordem e as linhas devem ser
-- organizadas em ordem crescente considerando a mesma ordem das colunas.

SELECT
	(SELECT COUNT(*) FROM doencas) AS qtd_doencas,
	(SELECT COUNT(DISTINCT nome) FROM sintomas) AS qtd_sintomas,
	(SELECT AVG(contagem_sint)
	 FROM (
		SELECT COUNT(*) AS contagem_sint
		FROM doenca_sintoma
		GROUP BY id_doenca
	 ) AS contagem_por_doenca) AS media_sintomas,
	(SELECT MIN(contagem_sint)
	 FROM (
		SELECT COUNT(*) AS contagem_sint
		FROM doenca_sintoma
		GROUP BY id_doenca
	 ) AS contagem_por_doenca) AS min_sintomas,
	(SELECT MAX(contagem_sint)
	 FROM (
		SELECT COUNT(*) AS contagem_sint
		FROM doenca_sintoma
		GROUP BY id_doenca
	 ) AS contagem_por_doenca) AS max_sintomas
ORDER BY
	qtd_doencas,
	qtd_sintomas,
	media_sintomas,
	min_sintomas,
	max_sintomas;
	
-- (f) Consulta com estatísticas sobre os sintomas. A consulta deve apresentar o
-- nome do sintoma, o número total de doenças que apresenta o sintoma, o número
-- de doenças em que o sintoma é muito comum, comum, pouco comum, raro e muito
-- raro. As colunas devem ser apresentadas nesta ordem e as linhas devem ser
-- organizadas, em ordem decrescente, em relação ao número total de doenças, em
-- seguida pela taxa de ocorrência (do muito comum ao muito raro) e, por fim,
-- por ordem alfabética em relação ao nome do sintoma.

SELECT s.nome, 
	COUNT(ds.id_doenca) AS total_doencas,
	SUM(CASE WHEN ds.ocorrencia = 'muito comum' THEN 1 ELSE 0 END) AS muito_comum,
	SUM(CASE WHEN ds.ocorrencia = 'comum' THEN 1 ELSE 0 END) AS comum,
	SUM(CASE WHEN ds.ocorrencia = 'pouco comum' THEN 1 ELSE 0 END) AS pouco_comum,
	SUM(CASE WHEN ds.ocorrencia = 'raro' THEN 1 ELSE 0 END) AS raro,
	SUM(CASE WHEN ds.ocorrencia = 'muito raro' THEN 1 ELSE 0 END) AS muito_raro
FROM sintomas AS s
JOIN doenca_sintoma AS ds ON s.id = ds.id_sintoma
GROUP BY s.nome
ORDER BY
	total_doencas DESC,
	muito_comum DESC,
	comum DESC,
	pouco_comum DESC,
	raro DESC,
	muito_raro DESC,
	s.nome ASC;

-- (g) Consulta para listar todas as doenças que possuem um determinado conjunto
-- de sintomas. Devem ser apresentados o id da doença e o seu nome (mantendo as
-- colunas nesta ordem e as linhas organizadas em ordem alfabética em relação ao
-- nome da doença). Para essa questão, considere o seguinte conjunto de sintomas
-- “Febre” e “Diarreia”.

(SELECT d.id, d.nome
	FROM doencas AS d
	JOIN doenca_sintoma AS ds ON d.id = ds.id_doenca
	JOIN sintomas AS s ON ds.id_sintoma = s.id
	WHERE s.nome = 'Febre')
INTERSECT
(SELECT d.id, d.nome
	FROM doencas AS d
	JOIN doenca_sintoma AS ds ON d.id = ds.id_doenca
	JOIN sintomas AS s ON ds.id_sintoma = s.id
	WHERE s.nome = 'Diarreia')
ORDER BY nome ASC;

-- (h) Consulta para listar as doenças mais prováveis para uma lista de sintomas
-- analisada. A consulta deve retornar o id da doença e o seu nome. Para essa
-- consulta, deve-se considerar um esquema de pontuações baseados nos sintomas,
-- calculado da seguinte forma:
--	i. Cada sintoma é atribuído a uma taxa de ocorrência. Essas taxas de
--	ocorrência são convertidas em pesos numéricos: muito comum = 5; comum = 4;
--	pouco comum = 3; raro = 2; muito raro = 1.
--	ii. Cada doença inicia com uma pontuação igual a 0 (zero). Para cada sintoma
--	que uma doença tem em comum em relação à lista de sintomas avaliada, a
--	pontuação da doença é incrementada pelo peso correspondente à taxa de
--	ocorrência do sintoma.
--	iii. Para cada sintoma presente na lista e que uma doença não tenha em sua
--	relação de sintomas, a pontuação da doença é decrementada em 1 ponto.
--	iv. As doenças são ordenadas em ordem decrescente em relação ao total de
--	pontos obtidos.

SELECT d.id, d.nome,
       COALESCE(SUM(
           CASE 
               WHEN s.nome IN ("febre","tosse") THEN
                   CASE ds.ocorrencia
                       WHEN 'muito comum' THEN 5
                       WHEN 'comum' THEN 4
                       WHEN 'pouco comum' THEN 3
                       WHEN 'raro' THEN 2
                       WHEN 'muito raro' THEN 1
                   END
               ELSE 0
           END
       ), 0) - (
           SELECT COUNT(*) 
           FROM sintomas s2
           WHERE s2.nome IN ("febre","tosse")
           AND s2.id NOT IN (
               SELECT ds.id_sintoma
               FROM doenca_sintoma ds
               WHERE ds.id_doenca = d.id
           )
       ) AS pontuacao_final
FROM doencas d
LEFT JOIN doenca_sintoma ds ON d.id = ds.id_doenca
LEFT JOIN sintomas s ON ds.id_sintoma = s.id
GROUP BY d.id, d.nome
ORDER BY pontuacao_final DESC;
