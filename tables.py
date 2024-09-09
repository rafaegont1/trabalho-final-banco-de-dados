#!/usr/bin/env python3

# {1: {
# 'nome': 'Tuberculose',
# 'cid': 'A15-A19',
# 'patogeno_nome': 'Mycobacterium tuberculosis',
# 'patogeno_tipo': 'Bactéria',
# 'sintomas': [{'nome': 'tosse', 'ocorrencia': 'muito comum'}, {'nome': 'febre', 'ocorrencia': 'comum'}, {'nome': 'perda de peso', 'ocorrencia': 'comum'}]
# }}

import csv


def get_doencas():
    doencas = {}
    with open('doencas.txt') as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',')
        id = 1
        for row in spamreader:
            doencas[id] = {
                'nome': row[0],
                'cid': row[1],
                'patogeno_nome': row[2],
                'patogeno_tipo': row[3],
                'sintomas': []
            }
            id_sintoma = 1
            for sintoma in row[4:]:
                splited = sintoma.split(' (')
                nome_sintoma = splited[0].lower()
                doencas[id]['sintomas'].append({
                    'nome': nome_sintoma,
                    'ocorrencia': splited[1][:-1]
                })
                id_sintoma += 1
            id += 1
    return doencas

def get_sintomas(doencas):
    sintomas = []
    for id_doenca in doencas:
        for sintoma in doencas[id_doenca]['sintomas']:
            sintomas.append(sintoma['nome'])
    sintomas = list(set(sintomas))
    dct = {}
    id = 1
    for sintoma in sintomas:
        dct[sintoma] = id
        id += 1
    print('<SINTOMAS>', dct)
    return dct


def get_from_doencas(doencas, col):
    lst = sorted(set(doenca[col] for doenca in doencas.values()))
    dct = {}
    id = 1
    for p in lst:
        dct[p] = id
        id += 1
    print('<', col, '>', dct)
    return dct


def get_patogenos(doencas):
    dct = {}
    id = 1
    for d in doencas.values():
        pat_nome = d['patogeno_nome']
        if pat_nome not in dct:
            pat_tipo = d['patogeno_tipo']
            dct[pat_nome] = {'id': id, 'tipo': pat_tipo}
            id += 1
    print('<PATOGENOS>', dct)
    return dct


def main():
    doencas = get_doencas()
    # for d in doencas:
    #     print(d, doencas[d])
    patogeno_tipo = get_from_doencas(doencas, 'patogeno_tipo')
    print(patogeno_tipo)
    print('--------------------')
    print('INSERT INTO patogeno_tipo VALUES')
    for nome, id in patogeno_tipo.items():
        print(f'({id}, \'{nome}\'),')
    print('--------------------')
    patogenos = get_patogenos(doencas)
    print('--------------------')
    print('INSERT INTO patogenos VALUES')
    for nome, values in patogenos.items():
        # print(f'({values['id']}, \'{nome}\', values['tipo']),')
        id = values['id']
        tipo = patogeno_tipo[values['tipo']]
        print(f'({id}, \'{nome}\', {tipo}),')
    print('--------------------')
    print('INSERT INTO doencas VALUES')
    for key, values in doencas.items():
        id = key
        nome = values['nome']
        cid = values['cid']
        id_patogeno = patogenos[values['patogeno_nome']]['id']
        print(f'({id}, \'{nome}\', \'{cid}\', {id_patogeno}),')
    print('--------------------')
    sintomas = get_sintomas(doencas)
    print('INSERT INTO sintomas VALUES')
    for nome, id in sintomas.items():
        print(f'({id}, \'{nome}\'),')
    print('--------------------')
    print('INSERT INTO doenca_sintoma VALUES')
    for id_doenca, values in doencas.items():
        for sintoma in values['sintomas']:
            id_sintoma = sintomas[sintoma['nome']]
            ocorrencia = sintoma['ocorrencia']
            print(f'({id_doenca}, {id_sintoma}, \'{ocorrencia}\'),')
    print('--------------------')


if __name__ == '__main__':
    main()
