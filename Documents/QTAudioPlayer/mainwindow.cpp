#include "mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    player = new QMediaPlayer(this);
    audio  = new QAudioOutput(this);
    player->setAudioOutput(audio);

    trackList = new QListWidget(this);
    addBtn    = new QPushButton("Add", this);
    playBtn   = new QPushButton("Play/Pause", this);
    saveBtn = new QPushButton("save", this);
    loadBtn = new QPushButton("load playlist", this);
    nextBtn = new QPushButton("next", this);
    prevBtn = new QPushButton("prev", this);
    slider = new QSlider(Qt::Horizontal,this);
    slider->setRange(0 , 100);
    slider->setValue(50);

    QWidget *central = new QWidget(this);
    QGridLayout *layout = new QGridLayout(central);
    layout->addWidget(trackList,0, 0, 1, 4);
    layout->addWidget(prevBtn,  1, 0);
    layout->addWidget(playBtn,  1, 1);
    layout->addWidget(nextBtn,  1, 2);
    layout->addWidget(addBtn,   1, 3);
    layout->addWidget(saveBtn,  2, 0);
    layout->addWidget(loadBtn,  2, 1);
    layout->addWidget(slider,   3, 0, 1, 4);

    setCentralWidget(central);

    connect(addBtn,    &QPushButton::clicked,  this, &MainWindow::addTracks);
    connect(playBtn,   &QPushButton::clicked,  this, &MainWindow::togglePlay);
    connect(trackList, &QListWidget::itemDoubleClicked, this, &MainWindow::playTrack);
    connect(saveBtn,   &QPushButton::clicked,  this,  &MainWindow::SaveTracks);
    connect(loadBtn,   &QPushButton::clicked,  this, &MainWindow::LoadTracks);
    connect(nextBtn,   &QPushButton::clicked, this,   &MainWindow::nextTrack);
    connect(prevBtn,   &QPushButton::clicked, this,   &MainWindow::prevTrack);
    connect(slider,    &QSlider::valueChanged, this, &MainWindow::ChangeVolume);

}

MainWindow::~MainWindow() {}

void MainWindow::addTracks()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this, " ", "", " (*.mp3 *.wav *.flac *.ogg *.m4a)"
        );
    for (const QString &file : files)
        trackList->addItem(file);
}

void MainWindow::playTrack(QListWidgetItem *item)
{
    CurrentTrack = trackList->row(item);
    player->setSource(QUrl::fromLocalFile(item->text()));
    player->play();
}

void MainWindow::SaveTracks()
{
    QString path = QFileDialog::getSaveFileName(
        this, "save playlist" , "" , "playlist (*.m3u)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream out (&file);
    for (int i = 0; i < trackList->count(); i++)
    {
        out << trackList->item(i)->text() <<"\n";
  }
    file.close();
}
void MainWindow::ChangeVolume(int value)
{
    audio->setVolume(value / 100.0);
}

void MainWindow::LoadTracks()
{
    QString path = QFileDialog::getOpenFileName(this , "open playlist" , "", "(*.m3u)");
    if (path.isEmpty()) return;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        trackList->addItem(line);
    }
    file.close();
}

void MainWindow::nextTrack()
{
    if (trackList->count() == 0) return;

    CurrentTrack++;

    if(CurrentTrack >= trackList->count())
    {
        CurrentTrack = 0;
    }
    trackList->setCurrentRow(CurrentTrack);

    player->setSource(QUrl::fromLocalFile(trackList->item(CurrentTrack)->text()));
    player->play();


}
void MainWindow::prevTrack()
{
    if (trackList->count() == 0) return;

    CurrentTrack--;

    if(CurrentTrack < 0)
    {
        CurrentTrack = trackList->count()-1;
    }
    trackList->setCurrentRow(CurrentTrack);

    player->setSource(QUrl::fromLocalFile(trackList->item(CurrentTrack)->text()));
    player->play();


}
void MainWindow::togglePlay()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
        player->pause();
    else
        player->play();
}
