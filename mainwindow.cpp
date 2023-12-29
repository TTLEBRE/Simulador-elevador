#include "mainwindow.h"
#include "QProgressBar"
#include "elevador.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QCoreApplication>
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->barra->setValue(0);

    ui->customPlot->xAxis->setLabel("segundos");
    ui->customPlot->yAxis->setLabel("metros");

    ui->customPlot_3->xAxis->setLabel("segundos");
    ui->customPlot_3->yAxis->setLabel("metros");

    ui->customPlot_4->xAxis->setLabel("segundos");
    ui->customPlot_4->yAxis->setLabel("metros");

    ui->customPlot_5->xAxis->setLabel("segundos");
    ui->customPlot_5->yAxis->setLabel("metros");

    ui->customPlot_6->xAxis->setLabel("segundos");
    ui->customPlot_6->yAxis->setLabel("metros");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    vector <QString> arquivo = {"elevador_1.txt","elevador_2.txt","elevador_3.txt","elevador_4.txt","elevador_5.txt","elevador_6.txt","elevador_7.txt","elevador_8.txt","elevador_9.txt","elevador_10.txt"};
    QString nome;

    q_elevadores = ui->line_quantidade_elevadores->text();
    q_andares = ui->line_quantidade_andares->text();
    d_pavimentos = ui->line_altura_pavimento->text();
    velocidade = ui->line_velocidade_elevador->text();
    viagens_ph = ui->line_viagens_por_hora->text();
    max_destinos = ui->line_max_destinos->text();
    tempo_espera = ui->line_tempo_espera->text();

    QVector <double> x1(3600), y1(3600);
    QVector <double> x2(3600), y2(3600);
    QVector <double> x3(3600), y3(3600);
    QVector <double> x4(3600), y4(3600);
    QVector <double> x5(3600), y5(3600);
    /*QVector <double> x6(3600), y6(3600);
    QVector <double> x7(3600), y7(3600);
    QVector <double> x8(3600), y8(3600);
    QVector <double> x9(3600), y9(3600);
    QVector <double> x10(3600), y10(3600);*/

    modo_aleatorio = ui->checkBox->checkState();
    modo_terreo_permanente = ui->checkBox_2->checkState();

    ui->barra->setRange(0,100);
    int perc = 0;

    srand(time(NULL));
    elevador sistema;
    sistema.dados(q_andares.toInt(), velocidade.toFloat(), viagens_ph.toInt(), q_elevadores.toInt(), modo_aleatorio, modo_terreo_permanente, d_pavimentos.toInt(), tempo_espera.toInt(), max_destinos.toInt());

    if (sistema.dados_corretos())
    {
        for (int seg = 0; seg < 3600; seg++)
        {
            if (seg % 36 == 0)
            {
                perc++;
            }
            ui->barra->setValue(perc);

            sistema.sortear_chamada();
            sistema.mover_elevador();
            sistema.imprimir_estado();

            y1[seg] = sistema.elevadores[0].altura;
            x1[seg] = seg;

            y2[seg] = sistema.elevadores[1].altura;
            x2[seg] = seg;

            y3[seg] = sistema.elevadores[2].altura;
            x3[seg] = seg;

            y4[seg] = sistema.elevadores[3].altura;
            x4[seg] = seg;

            y5[seg] = sistema.elevadores[4].altura;
            x5[seg] = seg;
        }

        for (int i = 0; i < q_elevadores.toInt(); ++i)
        {
            nome = arquivo[i];
            QFile arquivo(nome);
            QTextStream saida(&arquivo);

            if(!arquivo.open(QFile::WriteOnly|QFile::Text))
            {
                QMessageBox::warning(this,"ERRO", "");
            }

            saida << QString::fromStdString(sistema.elevadores[i].relatorio_altura);

            arquivo.flush();
            arquivo.close();
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(x1, y1);
        ui->customPlot->xAxis->setRange(0, 3600);
        ui->customPlot->yAxis->setRange(0, q_andares.toInt() * d_pavimentos.toInt());
        ui->customPlot->replot();

        ui->customPlot_3->addGraph();
        ui->customPlot_3->graph(0)->setData(x2, y2);
        ui->customPlot_3->xAxis->setRange(0, 3600);
        ui->customPlot_3->yAxis->setRange(0, q_andares.toInt() * d_pavimentos.toInt());
        ui->customPlot_3->replot();


        ui->customPlot_4->addGraph();
        ui->customPlot_4->graph(0)->setData(x3, y3);
        ui->customPlot_4->xAxis->setRange(0, 3600);
        ui->customPlot_4->yAxis->setRange(0, q_andares.toInt() * d_pavimentos.toInt());
        ui->customPlot_4->replot();

        ui->customPlot_5->addGraph();
        ui->customPlot_5->graph(0)->setData(x4, y4);
        ui->customPlot_5->xAxis->setRange(0, 3600);
        ui->customPlot_5->yAxis->setRange(0, q_andares.toInt() * d_pavimentos.toInt());
        ui->customPlot_5->replot();

        ui->customPlot_6->addGraph();
        ui->customPlot_6->graph(0)->setData(x5, y5);
        ui->customPlot_6->xAxis->setRange(0, 3600);
        ui->customPlot_6->yAxis->setRange(0, q_andares.toInt() * d_pavimentos.toInt());
        ui->customPlot_6->replot();


        QFile arquivo("dados.txt");
        QTextStream saida(&arquivo);


        if(!arquivo.open(QFile::WriteOnly|QFile::Text))
        {
            QMessageBox::warning(this,"ERRO", "");
        }
        saida << "elevador; total chamadas; tempo parado; tempo em movimento; distancia percorrida; tempo de espera medio\n";

        for (int i2 = 0; i2 < q_elevadores.toInt(); ++i2)
        {
            saida << i2 + 1 << "; " << sistema.elevadores[i2].total_chamadas << "; " << sistema.elevadores[i2].tempo_parado << "; ";
            saida << 3600 - sistema.elevadores[i2].tempo_parado << "; " <<(3600 - sistema.elevadores[i2].tempo_parado) * d_pavimentos.toInt();
            saida << "; " << sistema.elevadores[i2].tempo_espera_medio <<"\n";
        }

    }
    else
    {
        QMessageBox::warning(this,"ERRO", "insira os dados corretamente");
    }
}

void MainWindow::on_toolButton_chamadas_aleatorias_clicked()
{
    QMessageBox::about(this,"Ajuda", "Durante a simulação, o elevador será chamado em momentos e andares aleatórios para destinos aleatórios");
}

void MainWindow::on_toolButton_quantidade_elevadores_clicked()
{
    QMessageBox::about(this,"Ajuda", "Inserir a quantidade de elevadores a serem considerados no cálculo para o sistema");
}


void MainWindow::on_toolButton_quantidade_andares_clicked()
{
    QMessageBox::about(this,"Ajuda", "Inserir a quantidade de andares a que o sistema de elevadores terá acesso");
}


void MainWindow::on_toolButton_velocidade_clicked()
{
    QMessageBox::about(this,"Ajuda", "Inserir a velocidade de funcionamento do elevador em m/s");
}


void MainWindow::on_toolButton_viagens_por_hora_clicked()
{
    QMessageBox::about(this,"Ajuda", "Inserir a quantidade de viagens que o sistema de elevadores irá efetuar no período de uma hora");
}


void MainWindow::on_toolButton_altura_pavimentos_clicked()
{
    QMessageBox::about(this,"Ajuda", "Inserir a altura média, em metros, de cada andar do edifício");
}


void MainWindow::on_toolButton_tempo_espera_clicked()
{
    QMessageBox::about(this,"Ajuda", "Inserir o período de espera do sistema de elevadores, em segundos");
}


void MainWindow::on_toolButton_limite_pessoas_clicked()
{
    QMessageBox::about(this,"Ajuda", "Inserir a capacidade máxima de pessoas a serem transportadas no elevador em uma única viagem");
}


void MainWindow::on_toolButton_elevador_terreo_clicked()
{
    QMessageBox::about(this,"Ajuda", "Andar térreo a ser levado em conta para a simulação");
}

