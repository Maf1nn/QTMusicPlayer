#include "mainwindow.h"
#include <QFileDialog>
#include <QLabel>


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
    delBtn = new QPushButton("delete", this);
    progressSlider = new QSlider(Qt::Horizontal, this);
    slider = new QSlider(Qt::Horizontal,this);
    slider->setRange(0 , 100);
    slider->setValue(50);
    timeLabel = new QLabel("0:00", this);

    QWidget *central = new QWidget(this);
    QGridLayout *layout = new QGridLayout(central);

    layout->addWidget(trackList,      0, 0, 1, 4);
    layout->addWidget(progressSlider, 1, 0, 1, 4);
    layout->addWidget(timeLabel,      2, 0, 1, 4, Qt::AlignCenter);
    layout->addWidget(prevBtn,        3, 0);
    layout->addWidget(playBtn,        3, 1);
    layout->addWidget(nextBtn,        3, 2);
    layout->addWidget(addBtn,         3, 3);
    layout->addWidget(saveBtn,        4, 0);
    layout->addWidget(loadBtn,        4, 1);
    layout->addWidget(delBtn,         4, 2);
    layout->addWidget(slider,         4, 3);

    setCentralWidget(central);

    connect(addBtn,    &QPushButton::clicked,  this, &MainWindow::addTracks);
    connect(playBtn,   &QPushButton::clicked,  this, [=](){
        if(player->playbackState() == QMediaPlayer::PlayingState)
            player->pause();
        else
            player->play();
    });
    connect(trackList, &QListWidget::itemDoubleClicked, this, &MainWindow::playTrack);
    connect(saveBtn,   &QPushButton::clicked,  this,  &MainWindow::SaveTracks);
    connect(loadBtn,   &QPushButton::clicked,  this, &MainWindow::LoadTracks);
    connect(nextBtn,   &QPushButton::clicked, this,   &MainWindow::nextTrack);
    connect(prevBtn,   &QPushButton::clicked, this,   &MainWindow::prevTrack);
    connect(delBtn, &QPushButton::clicked, this, [=] (){
        int row = trackList->currentRow();
            if (row == -1) return;
            if (row == CurrentTrack)
                player->stop();
            trackList->takeItem(row);
    });
    connect(slider,    &QSlider::valueChanged, this, [=](int value){
    audio->setVolume(value / 100.0);
    });
    connect(player,    &QMediaPlayer::positionChanged, this, [=](qint64 pos){
        progressSlider->setValue(pos);
        int s = pos / 1000;
        int minutes = s / 60;
        int seconds = s % 60;
        QString time = QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
        timeLabel->setText(time);
    });
    connect(player,    &QMediaPlayer::durationChanged, this, [=](qint64 dur){
        progressSlider->setRange(0 , dur);
    });
    connect(player, &QMediaPlayer::mediaStatusChanged, this , [=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia)
            nextTrack();
    });
    connect(progressSlider, &QSlider::sliderPressed, this , [=](){
        player->setPosition(progressSlider->value());
    });
    connect(progressSlider, &QSlider::sliderMoved, player , &QMediaPlayer::setPosition);

}

MainWindow::~MainWindow() {}

void MainWindow::addTracks()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this, " ", "", " (*.mp3 *.wav *.flac *.ogg *.m4a)"
        );
    for (const QString &file : files)
    {
        QListWidgetItem *item = new QListWidgetItem(QFileInfo(file).baseName());
         item->setData(Qt::UserRole, file);
        trackList->addItem(item);
    }
}

void MainWindow::playTrack(QListWidgetItem *item)
{
    CurrentTrack = trackList->row(item);
    player->setSource(QUrl::fromLocalFile(item->data(Qt::UserRole).toString()));
    player->play();
    setWindowTitle("AudioPlayer - " + item->text());
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
        QListWidgetItem *item = new QListWidgetItem(QFileInfo(line).baseName());
        item->setData(Qt::UserRole, line);
        trackList->addItem(item);
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

    player->setSource(QUrl::fromLocalFile(trackList->item(CurrentTrack)->data(Qt::UserRole).toString()));
    player->play();
    setWindowTitle("AudioPlayer - " + trackList->item(CurrentTrack)->text());


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

    player->setSource(QUrl::fromLocalFile(trackList->item(CurrentTrack)->data(Qt::UserRole).toString()));
    player->play();
    setWindowTitle("AudioPlayer - " + trackList->item(CurrentTrack)->text());
}


