SELECT d.id, d.nome, d.cid, p.nome AS pat_nome, pt.nome AS pat_tipo,
	GROUP_CONCAT(
		DISTINCT np.nome
		ORDER BY np.nome
		SEPARATOR ', '
	) AS nomes_pop,
	GROUP_CONCAT(
		CONCAT(s.nome, ' (', ds.ocorrencia, ')')
		ORDER BY FIELD(ds.ocorrencia,
			'muito comum', 'comum', 'pouco comum', 'raro', 'muito raro'),
			s.nome
		SEPARATOR ', '
	) AS sintomas
FROM doencas AS d
JOIN doenca_sintoma AS ds ON ds.id_doenca = d.id
JOIN sintomas AS s ON s.id = ds.id_sintoma
JOIN patogenos AS p ON p.id = d.id_patogeno
JOIN patogeno_tipo AS pt ON pt.id = p.id_tipo
LEFT JOIN nomes_pop AS np ON np.id_doenca = d.id
GROUP BY d.id, d.nome, d.cid, p.nome, pt.nome
ORDER BY d.nome;
