SELECT d.id, d.nome,
       COALESCE(SUM(
           CASE 
               WHEN s.nome IN ("febre","tosse","fadiga","feridas") THEN
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
