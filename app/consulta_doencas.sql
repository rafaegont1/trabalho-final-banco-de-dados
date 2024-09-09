SELECT d.id, d.nome, d.cid, p.nome AS patogeno, pt.nome AS tipo,
	GROUP_CONCAT(
		CONCAT(s.nome, ' (', ds.ocorrencia, ')')
		ORDER BY FIELD(ds.ocorrencia,
			'muito comum', 'comum', 'pouco comum', 'raro', 'muito raro'),
			s.nome
		SEPARATOR ', '
	) AS sintomas
FROM doencas AS d
JOIN sintomas AS s
JOIN doenca_sintoma AS ds ON ds.id_doenca = d.id AND ds.id_sintoma = s.id
JOIN patogenos AS p ON p.id = d.id_patogeno
JOIN patogeno_tipo AS pt ON pt.id = p.id_tipo
GROUP BY d.id
ORDER BY d.nome;
