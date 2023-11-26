#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // WINDOW SETTINGS
    setFixedSize(400, 400);

    timer = new QTimer(this);
    menu = new QMenu("File");
    settings_action = new QAction(tr("Settings"));
    credits_action = new QAction(tr("Credits"));
    menu->addAction(settings_action);
    menu->addAction(credits_action);
    menuBar()->addMenu(menu);



    // SETTINGS
    Settings::pomodoro_time = 5;
    Settings::current_sound.setSource(QUrl::fromLocalFile("../sounds/sound.wav"));
    timeout_counter = 0;
    pause_counter = 0;



    // WIDGETS
    dial = new QDial;
    dial->setRange(0, Settings::pomodoro_time);

    time_left = new QLabel;
    time_left->setAlignment(Qt::AlignCenter);
    time_left->setFont(QFont("Arial", 20));
    time_left->setMaximumHeight(25);

    start_btn = new QPushButton(tr("Start"));
    stop_btn = new QPushButton(tr("Stop"));



    // LAYOUTS
    central_widget = new QWidget;
    main_layout = new QVBoxLayout;
    setCentralWidget(central_widget);
    central_widget->setLayout(main_layout);

    main_layout->addWidget(time_left);
    main_layout->addWidget(dial);
    main_layout->addWidget(start_btn);
    main_layout->addWidget(stop_btn);


    // CONNECTIONS
    connect(settings_action, SIGNAL(triggered()), this, SLOT(openSettings()));
    connect(credits_action, SIGNAL(triggered()), this, SLOT(openCredits()));

    connect(start_btn, SIGNAL(clicked()), this, SLOT(startTimer()));
    connect(stop_btn, SIGNAL(clicked()), this, SLOT(stopTimer()));
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    connect(dial, SIGNAL(valueChanged(int)), this, SLOT(onDialChange(int)));
}

QString MainWindow::convertTime(int total_seconds)
{
    QString minutes = QString::number(total_seconds / 60);
    QString seconds = QString::number(total_seconds % 60);

    if (minutes.size() == 1)
        minutes.push_front("0");

    if (seconds.size() == 1)
        seconds.push_front("0");

    return minutes + ":" + seconds;
}

void MainWindow::startTimer()
{
    Settings::is_round = true;
    if (start_btn->text() == tr("Start"))
    {
        dial->setRange(0, Settings::pomodoro_time);
        timer->start(1000);
        start_btn->setText(tr("Pause"));
    }
    else
    {
        if (++pause_counter % 2 == 0)
        {
            timer->start(1000);
            start_btn->setText(tr("Pause"));
        }
        else
        {
            timer->stop();
            start_btn->setText(tr("Continue"));
        }
    }
}

void MainWindow::stopTimer()
{
    timer->stop();
    timeout_counter	= 0;
    time_left->setText(convertTime(Settings::pomodoro_time - timeout_counter));
    dial->setValue(0);
    start_btn->setText(tr("Start"));
    Settings::is_round = false;
}

void MainWindow::onTimeout()
{
    Settings::is_round = true;
    dial->setValue(++timeout_counter);
    time_left->setText(convertTime(Settings::pomodoro_time - timeout_counter));

    if (timeout_counter > Settings::pomodoro_time)
    {
        timeout_counter	= 0;
        time_left->setText(convertTime(Settings::pomodoro_time - timeout_counter));
        dial->setValue(0);
        start_btn->setText(tr("Start"));
        Settings::current_sound.play();
        Settings::is_round = false;
        timer->stop();
    }
}

void MainWindow::onDialChange(int value)
{
    timeout_counter = dial->value();
    time_left->setText(convertTime(Settings::pomodoro_time - value));
}

void MainWindow::openSettings()
{
    Settings::current_sound.stop();
    SettingsForm *sf = new SettingsForm;
    sf->setModal(true);
    sf->show();
}

void MainWindow::openCredits()
{
    QMessageBox::information(this, tr("Credits"), tr("Version: 0.0.1\nCreated by: Oleksii Paziura"));
}
