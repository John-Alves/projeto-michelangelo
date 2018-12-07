# Michelangelo Drawing Machine
  
O projeto Michelangelo é uma CNC de dois eixos (2D), completamente open source, feita usando o microcontrolador da ST Microeletronics.

Ficou confuso? Calma, a intenção desse repositório é descomplicar sua vida. Trocando em miudos, o _Projeto Michelangelo_ trata-se de uma máquina desenhista que "receberá uma imagem" e irá desenhá-la em uma superfície.

  ![](https://github.com/John-Alves/projeto-michelangelo/blob/master/Imagens/michelangelo-logo.png?raw=true)
  
## Motivação

O _Michelangelo Drawing Machine_ foi desenvolvido como projeto final da disciplina de Sistemas Microprocessados, ministrada pelo professor Jardel na Universidade Federal do Ceará.

Tá, mas e aí?! Atualmente, existem muitos projetos de impressoras 3D e máquinas CNC circulando pela internet. O que há de novo? Fazendo uma análise um pouco mais minunciosa desses projetos, vemos que grande parte deles utilizam o arduino como placa de prototipagem (o que faz consumir bastante energia) e o código que é executado no microcontrolador é cheio de bibliotecas e funções que nunca sequer são utilizadas, fazendo com que muita memória seja usada.

_Michelangelo_ vem com intuito de sanar esses problemas evitando o uso desnecessário de memória e otimizando o código ao máximo (além de ser mais vantajoso energicamente, uma vez que trocou-se o arduino pelo Cortex M0).

Outra motivação para o projeto foi: "Deve ser bem legal construir uma máquina que desenha sozinha". :D

## Detalhes e Limitações

![](https://raw.githubusercontent.com/John-Alves/projeto-michelangelo/master/Diagrama%20de%20blocos%20do%20projeto.png)

Para que se envie um desenho à maquina, é necessário gerá-lo em um arquivo G-Code, carregando-o posteriormente no programa GenerateInstructions para que as instruções de movimento tanto dos motores como da caneta sejam filtradas e traduzidas ao microcontrolador.

As instruções traduzidas terão o seguinte formato: [Identificador do motor][Sentido de rotação] [Número de passos]. Existem três identificadores: P, servo motor; X, motor de passos do eixo x; e Y, motor de passos do eixo Y. Ademais, o sentido de rotação pode assumir os valores 0 (zero), sentido horário/de subida; e 1 (um), sentido anti-horário/de descida. Por fim, para as instruções relacionadas aos motores de passos, um terceiro argumento será utilizado, caracterizando o número de passos a serem realizados, no qual varia de 0 a 260 em virtude do alcance.

De posse delas, o microcontrolador tratará essas instruções, controlando os comportamentos dos dois drivers A4988 e do servo motor de forma simultânea, permitindo que o desenho seja realizado.

Inicialmente, codificamos o _Michelangelo_ para que faça desenhos lineares, ou seja, que contenha apenas linhas, independentemente da inclinação. Outra limitação imposta ao projeto se dá no alcance dos motores dos eixos. Como são utilizados motores de leitores antigos de DVD, estes possuem um alcance máximo, em média, de 4 centímetros, impossibilitando que a máquina faça desenhos mais expressivos.

## Materiais e Ferramentas Utilizadas

Os materiais e ferramentas utilizados nesse projeto estão listados abaixo:
* 1x STM32F030F4 (Cortex M0): Também chamado de "Green Pill", esse é microcontrolador usado no projeto.
* 2x Motor de Passo + eixo: Foi retirado de um DVD antigo. Usou-se o eixo que o próprio aparelho usa para movimentar a lente.  
* 2x Driver p/ Motor de Passo A4988: Usando para estabelecer a comunicação entre o microcontrolador e os motores de passo.
* Jumpers.
* 1x Protoboard 760 pontos.
* STM32CubeMX: Usando para configurar os pinos adequadamente e devido a implementação da Hardware Abstract Language (HAL).
* Atollic TrueSTUDIO: IDE de desemvolvimento.
