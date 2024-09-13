# Trabalho final de banco de dados

Este trabalho tem como objetivo implementar um banco de dados projetado para armazenar e organizar informações detalhadas sobre doenças, sintomas, patógenos, e demais fatores relevantes para a saúde pública e medicina.

## Dependências

Para rodar o programa, é necessário primeiro instalar as dependências **gcc**, **cmake**, **pkg-config**, **MariaDB Server**, **MariaDB Client**, **libmariadb3** e **libmariadb-dev**.

```
sudo apt update
sudo apt install gcc cmake pkg-config mariadb-server mariadb-client libmariadb3 libmariadb-dev
```

## Compilando utilizando o CMake

Para manter o diretório do projeto organizado, crie um diretório separado para a compilação, como build:

```
mkdir build
cd build
```

Dentro do diretório build, execute o comando CMake apontando para o diretório raiz do projeto, onde está o arquivo CMakeLists.txt:

```
cmake ..
```

Esse comando irá gerar os arquivos de build necessários. O CMake detectará automaticamente o compilador e as bibliotecas necessárias.

Gerados os arquivos de build, compile o projeto com o comando:

```
cmake --build .
```

Para rodar o programa é necessário executar o arquivo `app`.

```
./app
```
