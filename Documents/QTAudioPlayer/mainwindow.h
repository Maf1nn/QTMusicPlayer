#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTracks();
    void playTrack(QListWidgetItem *item); //здесь тупо обьявление функций
    void togglePlay();
    void SaveTracks();
    void ChangeVolume(int value);
    void LoadTracks();
    void nextTrack();
    void prevTrack();

private:
    QMediaPlayer *player;                  // здесь все понятно ; zxc demon
    QAudioOutput *audio;
    QListWidget  *trackList;
    QPushButton  *addBtn;
    QPushButton  *playBtn;
    QPushButton *saveBtn;
    QSlider *slider;
    QPushButton *loadBtn;
    int CurrentTrack = 0;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
    QSlider *progressSlider;

};
