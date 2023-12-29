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
/* includes */

#include "elevador.h"

/* ---------------------------------------------------------------------- */
/* recebe os dados e atribui as variaveis privadas */

int elevador::dados(int quantidade_andares_, float velocidade_elevador_, int viagens_por_hora_, int quantidade_elevadores_, bool andar_prioritario_, bool terreo_permanente_, int altura_pavimento_, int tempo_espera_, unsigned int max_destinos_)
{
    int i;

    quantidade_elevadores = quantidade_elevadores_;
    quantidade_andares = quantidade_andares_;
    velocidade_elevador = velocidade_elevador_;
    viagens_por_hora = viagens_por_hora_;
    andar_prioritario = andar_prioritario_;
    terreo_permanente = terreo_permanente_;
    altura_pavimento = altura_pavimento_;
    tempo_espera = tempo_espera_;
    max_destinos = max_destinos_;

    elevadores = def_cabines();

    remove("elevador_1.txt");
    remove("elevador_2.txt");
    remove("elevador_3.txt");
    remove("elevador_4.txt");
    remove("elevador_5.txt");
    remove("elevador_6.txt");
    remove("elevador_7.txt");
    remove("elevador_8.txt");
    remove("elevador_9.txt");
    remove("elevador_10.txt");
    remove("dados.txt");


    for(i = 0; i < quantidade_elevadores; i++)
    {
        elevadores[i].altura = 1 * altura_pavimento;
        elevadores[i].total_chamadas = 0;
    }
    return 0;
}

/* ---------------------------------------------------------------------- */

int elevador::dados_corretos()
{
    bool sit = true;

    if(quantidade_andares <= 0)
        sit = false;
    if(quantidade_elevadores <= 0 or quantidade_elevadores > 5)
        sit = false;
    if(velocidade_elevador <= 0)
        sit = false;
    if(viagens_por_hora <= 0)
        sit = false;
    if(altura_pavimento <= 0)
        sit = false;
    if(tempo_espera <= 0)
        sit = false;
    if(max_destinos <= 0)
        sit = false;

    return sit;
}

/* ---------------------------------------------------------------------- */
/* dado um certo andar e retornado o elevador disponivel mais proximo */

int elevador::mais_proximo(int andar)
{
    int proximo = -1000;
    int numero_elevador = -1;
    int i;

    if (quantidade_elevadores > 1)
    {
        for(i = 0; i < quantidade_elevadores; i++)
        {
            if((elevadores[i].movimento == subindo and elevadores[i].altura > andar * altura_pavimento) or (elevadores[i].movimento == descendo and elevadores[i].altura  < andar * altura_pavimento))
                /* se o sentido do elevador nao permitir que ele pare no andar == continue (volta ao inicio do loop) */
            {
                continue;
            }
            else
            {
                if(pow(proximo - andar * altura_pavimento, 2) > pow(elevadores[i].altura - andar * altura_pavimento, 2))
                    /* se o elevador for o elegivel mais proximo */
                {
                    proximo = elevadores[i].altura;
                    numero_elevador = i;
                }
            }
        }
    }
    else
    {
        numero_elevador = 0;
    }
    return numero_elevador;
}

/* ---------------------------------------------------------------------- */

int elevador::no_terreo()
{
    int i;
    bool sit = false;

    for(i = 0; i < quantidade_elevadores; i++)
    {
        if(elevadores[i].altura == altura_pavimento)
        {
            sit = true;
            break;
        }
    }
    return sit;
}

/* ---------------------------------------------------------------------- */
/* move a cabine de acordo com sua velocidade e sentido */

int elevador::mover_elevador()
{
    int i;
    definir_movimento();

    for(i = 0; i < quantidade_elevadores; i++)
    {
        if(elevadores[i].espera > 0)  /* delay do elevador ate chegar em um destino */
            elevadores[i].espera--;

        if(elevadores[i].movimento == parado)
            elevadores[i].tempo_parado += 1;

        if((not elevadores[i].andar_destino_subindo.empty() or not elevadores[i].andar_destino_descendo.empty()) and (elevadores[i].espera == 0))
            /* verifica se a cabine te um andar de destino e se o elevador nao estiver parado */
        {
            if(elevadores[i].movimento == subindo)
            {
                if(pow(elevadores[i].altura - elevadores[i].andar_destino_subindo.back() * altura_pavimento, 2) >= pow(velocidade_elevador, 2))
                    /* se a distancia do elevador ate o destino for maior que a velocidade o elevador se movimenta */
                    elevadores[i].altura += velocidade_elevador;
                else
                {
                    elevadores[i].altura = elevadores[i].andar_destino_subindo.back() * altura_pavimento;

                    elevadores[i].andar_destino_subindo.pop_back();
                    elevadores[i].espera = tempo_espera;

                    if(elevadores[i].altura / altura_pavimento == elevadores[i].andar_chamada.back())
                    {
                        elevadores[i].andar_chamada.pop_back();
                        sortear_destino(i);
                    }
                }
                continue;
            }

            if(elevadores[i].movimento == descendo)
            {
                if(pow(elevadores[i].altura - elevadores[i].andar_destino_descendo.back() * altura_pavimento, 2) >= pow(velocidade_elevador, 2))
                    /* se a distancia do elevador ate o destino for maior que a velocidade o elevador se movimenta */
                    elevadores[i].altura -= velocidade_elevador;
                else
                {
                    elevadores[i].altura = elevadores[i].andar_destino_descendo.back() * altura_pavimento;

                    elevadores[i].andar_destino_descendo.pop_back();
                    elevadores[i].espera = tempo_espera;

                    if(elevadores[i].altura / altura_pavimento == elevadores[i].andar_chamada.back())
                    {
                        elevadores[i].andar_chamada.pop_back();
                        sortear_destino(i);
                    }
                }
            }
        }
    }
    return 0;
}

/* ---------------------------------------------------------------------- */
/* define o sentido de movimendo da cabine baseado na sua posicao e seus destinos */

int elevador::definir_movimento()
{
    int i;

    for(i = 0; i < quantidade_elevadores; i++)
    {
        if(elevadores[i].andar_destino_subindo.empty() and elevadores[i].andar_destino_descendo.empty())
        {
            elevadores[i].movimento = parado;
            continue;
        }
        else
        {
            if(elevadores[i].movimento == subindo and not elevadores[i].andar_destino_subindo.empty())
            {
                elevadores[i].movimento = subindo;
                sort(elevadores[i].andar_destino_subindo.begin(), elevadores[i].andar_destino_subindo.end(), greater <int>());
                sort(elevadores[i].andar_chamada.begin(), elevadores[i].andar_chamada.end(), greater <int>());
                continue;
            }
            if(elevadores[i].movimento == descendo and not elevadores[i].andar_destino_descendo.empty())
            {
                elevadores[i].movimento = descendo;
                sort(elevadores[i].andar_destino_descendo.begin(), elevadores[i].andar_destino_descendo.end());
                sort(elevadores[i].andar_chamada.begin(), elevadores[i].andar_chamada.end());
                continue;
            }
            if(not elevadores[i].andar_destino_subindo.empty())
            {
                elevadores[i].movimento = subindo;
                sort(elevadores[i].andar_destino_subindo.begin(), elevadores[i].andar_destino_subindo.end(), greater <int>());
                sort(elevadores[i].andar_chamada.begin(), elevadores[i].andar_chamada.end(), greater <int>());
                continue;
            }
            if(not elevadores[i].andar_destino_descendo.empty())
            {
                elevadores[i].movimento = descendo;
                sort(elevadores[i].andar_destino_descendo.begin(), elevadores[i].andar_destino_descendo.end());
                sort(elevadores[i].andar_chamada.begin(), elevadores[i].andar_chamada.end());
                continue;
            }
        }
    }
    return 0;
}

/* ---------------------------------------------------------------------- */

int elevador::tempo_de_espera(int cabine, int andar)
{
    int delta;
    delta = sqrt(pow(elevadores[cabine].altura - (andar * altura_pavimento), 2));

    if(elevadores[cabine].tempo_espera_medio == 0)
    {
        elevadores[cabine].tempo_espera_medio = (delta / velocidade_elevador);

        if(elevadores[cabine].andar_destino_subindo.size() > 0 and elevadores[cabine].movimento == subindo)
            elevadores[cabine].tempo_espera_medio += (elevadores[cabine].andar_destino_subindo.size()) * tempo_espera;
        if(elevadores[cabine].andar_destino_descendo.size() > 0 and elevadores[cabine].movimento == descendo)
            elevadores[cabine].tempo_espera_medio += (elevadores[cabine].andar_destino_descendo.size()) * tempo_espera;
    }
    else
    {
        elevadores[cabine].tempo_espera_medio += (delta / velocidade_elevador);

        if(elevadores[cabine].andar_destino_subindo.size() > 0 and elevadores[cabine].movimento != descendo)
            elevadores[cabine].tempo_espera_medio += (elevadores[cabine].andar_destino_subindo.size()) * tempo_espera;
        if(elevadores[cabine].andar_destino_descendo.size() > 0 and elevadores[cabine].movimento != subindo)
            elevadores[cabine].tempo_espera_medio += (elevadores[cabine].andar_destino_descendo.size()) * tempo_espera;
        elevadores[cabine].tempo_espera_medio /= 2;
    }
    return 0;
}

/* ---------------------------------------------------------------------- */

int elevador::sortear_chamada()
{
    if(andar_prioritario == true)
        sortear_chamada_1();
    else
        sortear_chamada_0();

    if(terreo_permanente == true and (not no_terreo()) and (not no_vetor(1, elevadores[mais_proximo(1)].andar_destino_descendo)) and mais_proximo(1) != -1)
    {
        elevadores[mais_proximo(1)].andar_destino_descendo.push_back(1);
    }
    return 0;
}

/* ---------------------------------------------------------------------- */
/* sorteia um andar e aciona a cabine elegivel mais proxima */

int elevador::sortear_chamada_0()
{
    int i;
    int rand1 = rand() % (int) round(tempo_simulacao / viagens_por_hora);  /* probabilidade de um elevador ser chamado */
    int rand2 = (rand() % quantidade_andares) + 1;                            /* andar aleatorio */

    if(rand1 == 0)
    {
        elevadores[mais_proximo(rand2)].total_chamadas += 1;

        for(i = 0; i < 5; i++)
        {
            rand2 = (rand() % quantidade_andares) + 1;

            if(elevadores[mais_proximo(rand2)].andar_destino_subindo.size() + elevadores[mais_proximo(rand2)].andar_destino_descendo.size() >= max_destinos)
                break;

            if(mais_proximo(rand2) != -1 and not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_chamada))
            {
                if(rand2 * altura_pavimento > elevadores[mais_proximo(rand2)].altura)
                {
                    if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_chamada))
                        elevadores[mais_proximo(rand2)].andar_chamada.push_back(rand2);
                    if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_destino_subindo))
                        elevadores[mais_proximo(rand2)].andar_destino_subindo.push_back(rand2);
                    tempo_de_espera(mais_proximo(rand2), rand2);
                    break;
                }
                if(rand2 * altura_pavimento < elevadores[mais_proximo(rand2)].altura)
                {
                    if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_chamada))
                        elevadores[mais_proximo(rand2)].andar_chamada.push_back(rand2);
                    if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_destino_descendo))
                        elevadores[mais_proximo(rand2)].andar_destino_descendo.push_back(rand2);
                    tempo_de_espera(mais_proximo(rand2), rand2);
                    break;
                }
            }
        }
    }
    return 0;
}

/* ---------------------------------------------------------------------- */

int elevador::sortear_chamada_1()
{
    int i;
    int rand1 = rand() % (int) round(tempo_simulacao / viagens_por_hora);  /* probabilidade de um elevador ser chamado */
    int rand2;
    int rand3;

    if(rand1 == 0)
    {
        elevadores[mais_proximo(rand2)].total_chamadas += 1;

        for(i = 0; i < 5; i++)
        {
            if(elevadores[mais_proximo(rand2)].andar_destino_subindo.size() + elevadores[mais_proximo(rand2)].andar_destino_descendo.size() >= max_destinos)
            {
                break;
            }

            rand3 = rand() % 2;

            if(rand3 == 1)
            {
                elevadores[mais_proximo(1)].andar_chamada.push_back(1);

                if(not no_vetor(1, elevadores[mais_proximo(1)].andar_destino_descendo))
                {
                    elevadores[mais_proximo(1)].andar_destino_descendo.push_back(1);
                }
                tempo_de_espera(mais_proximo(1), 1);
                break;
            }
            else
            {
                rand2 = (rand() % quantidade_andares) + 1;

                if(mais_proximo(rand2) != -1)
                {
                    if(rand2 * altura_pavimento > elevadores[mais_proximo(rand2)].altura)
                    {
                        if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_chamada))
                            elevadores[mais_proximo(rand2)].andar_chamada.push_back(rand2);

                        if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_destino_subindo))
                        {
                            elevadores[mais_proximo(rand2)].andar_destino_subindo.push_back(rand2);
                        }
                        tempo_de_espera(mais_proximo(rand2), rand2);
                        break;
                    }
                    if(rand2 * altura_pavimento < elevadores[mais_proximo(rand2)].altura)
                    {
                        if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_chamada))
                        {
                            elevadores[mais_proximo(rand2)].andar_chamada.push_back(rand2);
                        }
                        if(not no_vetor(rand2, elevadores[mais_proximo(rand2)].andar_destino_descendo))
                        {
                            elevadores[mais_proximo(rand2)].andar_destino_descendo.push_back(rand2);
                        }
                        tempo_de_espera(mais_proximo(rand2), rand2);
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

/* ---------------------------------------------------------------------- */

int elevador::sortear_destino(int num_elevador)
{
    if(andar_prioritario == false)
    {
        sortear_destino_0(num_elevador);
    }
    else
    {
        sortear_destino_1(num_elevador);
    }
    return 0;
}

/* ---------------------------------------------------------------------- */
/* sorteia o(s) andar(es) de destino da cabine */

int elevador::sortear_destino_0(int num_elevador)
{
    int total_destinos;
    int andar_aleatorio;
    int i;

    if(elevadores[num_elevador].andar_destino_subindo.size() + elevadores[num_elevador].andar_destino_descendo.size() < max_destinos)
    {
        total_destinos = rand() % max_destinos - elevadores[num_elevador].andar_destino_subindo.size() + elevadores[num_elevador].andar_destino_descendo.size() + 1;

        for(i = 0; i < total_destinos; i++)
        {
            andar_aleatorio = (rand() % quantidade_andares) + 1;

            if(andar_aleatorio * altura_pavimento == elevadores[num_elevador].altura)
            {
                i--;
                continue;
            }

            if(andar_aleatorio * altura_pavimento > elevadores[num_elevador].altura and not no_vetor(andar_aleatorio, elevadores[num_elevador].andar_destino_subindo))
                elevadores[num_elevador].andar_destino_subindo.push_back(andar_aleatorio);
            if(andar_aleatorio * altura_pavimento < elevadores[num_elevador].altura and not no_vetor(andar_aleatorio, elevadores[num_elevador].andar_destino_descendo))
                elevadores[num_elevador].andar_destino_descendo.push_back(andar_aleatorio);
        }
    }
    definir_movimento();
    return 0;
}

/* ---------------------------------------------------------------------- */

int elevador::sortear_destino_1(int num_elevador)
{
    int total_destinos;
    int andar_aleatorio;
    int i;

    if(elevadores[num_elevador].andar_destino_subindo.size() + elevadores[num_elevador].andar_destino_descendo.size() < max_destinos)
    {
        if(elevadores[num_elevador].andar_chamada.back() == 1)
        {
            total_destinos = rand() % max_destinos - elevadores[num_elevador].andar_destino_subindo.size() + elevadores[num_elevador].andar_destino_descendo.size() + 1;

            for(i = 0; i < total_destinos; i++)
            {
                andar_aleatorio = (rand() % quantidade_andares) + 1;

                if(andar_aleatorio * altura_pavimento == elevadores[num_elevador].altura or andar_aleatorio == 1)
                {
                    i--;
                    continue;
                }

                if(andar_aleatorio * altura_pavimento > elevadores[num_elevador].altura and not no_vetor(andar_aleatorio, elevadores[num_elevador].andar_destino_subindo))
                    elevadores[num_elevador].andar_destino_subindo.push_back(andar_aleatorio);
                if(andar_aleatorio * altura_pavimento < elevadores[num_elevador].altura and not no_vetor(andar_aleatorio, elevadores[num_elevador].andar_destino_descendo))
                    elevadores[num_elevador].andar_destino_descendo.push_back(andar_aleatorio);
            }
        }
        else
            if(not no_vetor(1, elevadores[num_elevador].andar_destino_descendo))
                elevadores[num_elevador].andar_destino_descendo.push_back(1);
    }

    definir_movimento();
    return 0;
}

/* ---------------------------------------------------------------------- */
/* funcao get */

int elevador::get_tempo_simulacao()
{
    return tempo_simulacao;
}

/* ---------------------------------------------------------------------- */

elevador::elevador() {} /* metodo construtor */
elevador::~elevador() {} /* metodo destrutor */

/* ---------------------------------------------------------------------- */

int elevador::imprimir_estado()
{
    int i;

    for(i = 0; i < quantidade_elevadores; i++)
        elevadores[i].relatorio_altura += to_string(elevadores[i].altura) + "\n";
    return 0;
}

/* ---------------------------------------------------------------------- */

bool elevador::no_vetor(int num, vector <int> vetor)
{
    bool sit = false;
    unsigned int i;

    for(i = 0; i < vetor.size(); i++)
    {
        if(vetor[i] == num)
        {
            sit = true;
            break;
        }
    }
    return sit;
}

/* ---------------------------------------------------------------------- */
/* cria e armazena as cabines */

vector <cabine> elevador::def_cabines()
{
    vector <cabine> elevadores {};

    if(quantidade_elevadores >= 0)
    {
        cabine elevador0;
        elevadores.push_back(elevador0);
    }
    if(quantidade_elevadores >= 1)
    {
        cabine elevador1;
        elevadores.push_back(elevador1);
    }
    if(quantidade_elevadores >= 2)
    {
        cabine elevador2;
        elevadores.push_back(elevador2);
    }
    if(quantidade_elevadores >= 3)
    {
        cabine elevador3;
        elevadores.push_back(elevador3);
    }
    if(quantidade_elevadores >= 4)
    {
        cabine elevador4;
        elevadores.push_back(elevador4);
    }
    if(quantidade_elevadores >= 5)
    {
        cabine elevador5;
        elevadores.push_back(elevador5);
    }
    if(quantidade_elevadores >= 6)
    {
        cabine elevador6;
        elevadores.push_back(elevador6);
    }
    if(quantidade_elevadores >= 7)
    {
        cabine elevador7;
        elevadores.push_back(elevador7);
    }
    if(quantidade_elevadores >= 8)
    {
        cabine elevador8;
        elevadores.push_back(elevador8);
    }
    if(quantidade_elevadores >= 9)
    {
        cabine elevador9;
        elevadores.push_back(elevador9);
    }

    return elevadores;
}

/* ---------------------------------------------------------------------- */
/* metodo destrutor da classe cabine */

cabine::~cabine()
{
    ;
}

/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=4 sw=4 tw=0 wm=0 fo=croql : CPP config - Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20220718.172615      */

