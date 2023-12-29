#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString q_elevadores;
    QString q_andares;
    QString d_pavimentos;
    QString velocidade;
    QString viagens_ph;
    QString max_destinos;
    QString tempo_espera;

    bool modo_aleatorio;
    bool modo_terreo_permanente;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_toolButton_chamadas_aleatorias_clicked();

    void on_toolButton_quantidade_elevadores_clicked();

    void on_toolButton_quantidade_andares_clicked();

    void on_toolButton_velocidade_clicked();

    void on_toolButton_viagens_por_hora_clicked();

    void on_toolButton_altura_pavimentos_clicked();

    void on_toolButton_tempo_espera_clicked();

    void on_toolButton_limite_pessoas_clicked();

    void on_toolButton_elevador_terreo_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
