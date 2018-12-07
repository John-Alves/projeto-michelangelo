# Michelangelo Drawing Machine
  
O projeto Michelangelo é uma CNC de dois eixos (2D), completamente open source, feita usando o microcontrolador da ST Microeletronics. 

Ficou confuso? Calma, a intenção desse repositório é descomplicar sua vida. Trocando em miudos, o _Projeto Michelangelo_ trata-se de uma máquina desenhista que "receberá uma imagem" e irá desenha-la em uma superficie.

  ![](https://github.com/John-Alves/projeto-michelangelo/blob/master/Imagens/michelangelo-logo.png?raw=true)
  
## Motivação

A _Michelangelo Drawing Machine_ foi desenvolvida como projeto final da disciplina de Sistemas Microprocessados, ministrada pelo professor Jardel na Universidade Federal do Ceará.

Tá, mas e ai?! Atualmente, existem muitos projetos de impressoras 3D e máquinas CNC circulando pela internet. O que há de novo? Fazendo uma análise um pouco mais minunciosa desses projetos, vemos que grande parte deles, utilizam o arduino como placa de prototipagem (o que faz consumir bastante energia) e o código que é executado no Microcontrolador é cheio de bibliotecas e funções que nunca sequer são utilizadas, fazendo com que muita memoria seja usada.

O _Michelangelo_ vem com intuito de sanar esses problemas evitando o uso desnecessário de memória e otimizando o código ao máximo (além de ser mais vantajoso energicamente, uma vez que trocou-se o arduino pelo Cortex M0). 

Outra motivação para o projeto foi: "Deve ser bem legal construir uma máquina que desenha sozinha". :)

