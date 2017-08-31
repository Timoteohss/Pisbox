# Pisbox

Pisbox é um sandbox para minhas aulas de Processamento de Imagens, utilizando apenas OpenCV para manipulação, inclusive com uma interface gráfica utilizando primitivas de desenho do OpenCV!

# Novas atualizações!

Este é o primeiro push, então não temos muitas novidades...

> Ainda virão grandes coisas...
É o que um dia eu disse

## Tecnologias 

Pisbox utiliza as seguintes tecnologias para se fazer útil:

* [OpenCV](http://opencv.org/)  - Biblioteca para manipulação das imagens

Só!

### Instalação

Pisbox requer que você tenha instalado o [OpenCV](http://opencv.org/) versão 2.4.3 ou mais nova.

Para compilar o projeto inicial, apenas escreva

'''sh
g++ -std=c++11 pi.cpp -o pisdbox `pkg-config --libs opencv`
./pisdbox <nome da imagem>
'''
