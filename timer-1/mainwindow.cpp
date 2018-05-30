#include "mainwindow.h"
#include "console.h"
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <sstream>      // std::stringstream
#include <math.h>       /* sqrt */
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_console(new Console(this)),
    banimate(new BallAnimation(this))
{
    button[0] = new QPushButton("LED 1",this);
    button[0]->setCheckable(true);
    label[0] = new QLabel("Off",this);
    label[0]->move(button[0]->pos()+QPoint(button[0]->width()+5,0));
    m_console->move(button[0]->pos()+QPoint(0,button[0]->height()+5));
    m_console->setEnabled(true);

    connect(button[0],SIGNAL(toggled(bool)),this,SLOT(onbutton1Press(bool)));
    m_led_state[0] = 0;
    m_led_state[1] = 0;
    m_led_state[2] = 0;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readData()));
    timer->start(100);
    banimate->move(QPoint(200,200));
    //setFixedSize(800,600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onbutton1Press(bool toggled)
{
    toggle_it(toggled,0,3);
}

void MainWindow::processMessage(const QString &b)
{

    m_console->setText(b);
    auto values = b.split(":");
    if(values[0] == "V") {
        float cel = values[1].toInt()/4 * 0.48828125;
        float fahr = cel * 9.0/5.0 + 32;
        std::stringstream ss;
        ss << values[1].toInt() << " " << cel << " " << fahr;
        m_console->setText(ss.str().c_str());
    }
}

void MainWindow::toggle_it(bool toggled,int button,int led_pin)
{
    if(toggled) {
        label[button]->setText("On");
        m_led_state[led_pin] = 255;
    }
    else {
        label[button]->setText("Off");
        m_led_state[led_pin] = 0;
    }

}

void MainWindow::readData()
{
    static int count=0;
    //const QByteArray data = m_serial->readAll();
    //m_buffer = QString("%1" ).arg( QDateTime::currentDateTime().toTime_t() );
    //processMessage(m_buffer);
    banimate->addToOrigin(QPoint(1,1));
    banimate->repaint();
    processMessage(QString("%1 %2").arg(count).arg(QDateTime::currentDateTime().toTime_t() ));
    count++;
    m_buffer = "";
}
int width = 80; //painter.drawRect(QRect(0,0,width()-1,height()-1));
int height = 80; //ballanimation.cpp
int ballX = 40;
int ballY = 40;
int forceX = 1; //Change the direction of ellipse
int forceY = 1;

int distance(int x1, int y1, int x2, int y2){
    int xDelta = x1 - x2;
    int yDelta = y1 - y2;
    return sqrt(xDelta * xDelta + yDelta * yDelta);
}

void updatePhysics()
{
    if(ballX > width - 2 || ballX < 0){
        forceX = -forceX; //Change the direction
    }

        ballX += forceX;
        ballY += forceY;
}

int main()
{
    while (true)
    {
        updatePhysics();
    }

    return 0;
}
