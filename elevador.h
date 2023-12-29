/***************************************************************************
 *   elevador.cpp                             Version 20221005.143035      *
 *                                                                         *
 *   Classe Elevador                                                       *
 *   Copyright (C) 2022         by Matheus Barbosa Lebre Alves             * 
 *                              by Ruben Carlo Benante                     *                                                      
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
/* ---------------------------------------------------------------------- */

#ifndef _ELEVADOR_H
#define _ELEVADOR_H

/* ---------------------------------------------------------------------- */
/* includes */

#include <stdio.h>  /* Standard I/O functions */
#include <stdlib.h>  /* Miscellaneous functions (rand, malloc, srand)*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <time.h>  /* Time and date functions */
#include <math.h>  /* Mathematics functions */
#include <string.h>

using namespace std;

enum direcao {subindo, descendo, parado};

/* ---------------------------------------------------------------------- */
/* classe cabine */

class cabine
{
    public:
        int espera = 0;
        float altura = 0;
        direcao movimento = parado;

        vector <int> andar_destino_subindo;
        vector <int> andar_destino_descendo;
        vector <int> andar_chamada;
        string relatorio_altura;

        int total_chamadas;
        int tempo_parado;
        float tempo_espera_medio;

        ~cabine();
};

/* ---------------------------------------------------------------------- */
/* classe elevador */

class elevador
{
    private:
        int quantidade_elevadores;    /* quantidade de elevadores */
        int quantidade_andares;       /* quantidade de andares */
        int altura_pavimento;         /* distancia em metros entre os pavimentos */
        float velocidade_elevador;    /* velocidade dos elevadores */
        int viagens_por_hora;         /* media de viagens por hora */
        bool andar_prioritario;       /* estabelece se o primeiro andar vai ser prioritario ou nao */
        bool terreo_permanente;       /* estabelece se o elevador fica no terreo ou nao */
        int tempo_espera;             /* tempo em segundos que o elevador deve esperar ate parar em um andar */
        unsigned int max_destinos;    /* maximo de destinos que uma viagem pode ter */
        int tempo_simulacao = 3600;   /* tempo em segundos da simulacao */

    public:
        /* int tempo_medio; */
        int dados(int quantidade_andades_, float velocidade_elevador_, int viagens_por_hora_, int quantidade_elevadores_,
                  bool andar_prioritario_, bool terreo_permanente_, int altura_pavimento_, int tempo_espera_, unsigned int max_destinos_);
        int dados_corretos();
        int mais_proximo(int andar);
        int no_terreo();
        int mover_elevador();
        int definir_movimento();
        int tempo_de_espera(int cabine, int andar);
        int sortear_chamada();
        int sortear_chamada_0();
        int sortear_chamada_1();
        int sortear_destino(int num_elevador);
        int sortear_destino_0(int num_elevador);
        int sortear_destino_1(int num_elevador);
        int imprimir_estado();
        bool no_vetor(int num, vector <int> vetor);
        int get_tempo_simulacao();

        vector <cabine> elevadores {};
        vector <cabine> def_cabines();

        elevador();
        ~elevador();
};

/* ---------------------------------------------------------------------- */

#endif /* NOT def _ELEVADOR_H */

/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=4 sw=4 tw=0 wm=0 fo=croql : C config for Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20160615.020326      */

