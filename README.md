INSTRUÇÕES PARA GERAR OS ARQUIVOS DE DADOS E O INDICE PRIMÁRIO

1. extraia o arquivo artigo.csv.gz com o nome artigo.csv e extraia para a raiz do projeto
2. compile o arquivo.cpp com o g++ com o comando "g++ upload.cpp -o upload"
3. execute o executável upload com o comando "./upload"
4. acompanhe as mensagens de progresso na tela até o fim da execução do programa
5. ao fim vc terá os arquivos bloco.data (representação fisica da b+tree) e students.data (representação fisica da estrutura hash)

DESCRIÇÃO DAS ESTRUTURAS
todos os structs estão definidos em struct.h

Arquivo hashing
É organizado por buckets (struct bucket), cada bucket é formado por 18 blocos
cada bloco é um bloco fisico de 4kb (struct block) e cada bloco possui 2 registros (struct registry)
que efetivamente guardam os dados.

Indice Primário (Árvore B+)
Possui grau 495


