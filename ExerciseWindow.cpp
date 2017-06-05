#include "ExerciseWindow.h"
#include "ui_ExerciseWindow.h"
#include <glob.h>

ExerciseWindow::ExerciseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExerciseWindow)
{
    ui->setupUi(this);
    setupParametars();

}

ExerciseWindow::~ExerciseWindow()
{
    emit onStop();
    delete ui;
}

void ExerciseWindow::setupParametars()
{
    ui->recData->setStyleSheet(QString("QPushButton {background-color: white;}"));
    ui->stopRec->setStyleSheet(QString("QPushButton {background-color: white;}"));

    ui->xGyroSlider->setMinimum(-450);
    ui->xGyroSlider->setMaximum(450);
    ui->yGyroSlider->setMinimum(-450);
    ui->yGyroSlider->setMaximum(450);

}

void ExerciseWindow::on_start_clicked()
{
    ui->start->setDisabled(true);

    qRegisterMetaType<MainWindow::data>("MainWindow::data");

    connect(&mainwin,SIGNAL(onNumber(MainWindow::data)),this,SLOT(newNumber(MainWindow::data)));
    connect(this,&ExerciseWindow::onStop, &mainwin,&MainWindow::stop);
    //MainWindow::start();
    QFuture<void> test = QtConcurrent::run(&this->mainwin,&MainWindow::start);
}

void ExerciseWindow::rec()
{
    mRec = true;
}
void ExerciseWindow::stopRec()
{
    mRec = false;
    recData.close();
}

void ExerciseWindow::newNumber(MainWindow::data cleanData)
{
    connect(ui->recData, SIGNAL(clicked()), this, SLOT(rec()));
    connect(ui->stopRec,SIGNAL(clicked()), this,SLOT(stopRec()));

    // sum yGyro values so that the angle actualy stays as long
    // as we dont move our hand the proper way wait for
    // 50 samples to pass because of calculating offset
    if(diffAbs(cleanData.xGyroSample,xGyroTmp) > 0.01){
        xGyroSum +=cleanData.xGyroSample;
    }
    xGyroTmp = cleanData.xGyroSample;

    if(diffAbs(cleanData.yAccelSample,yGyroTmp) > 0.01){
        yGyroSum +=cleanData.yGyroSample;
    }
    yGyroTmp = cleanData.yGyroSample;

    //remove the drift with help of accel values
    if(abs(cleanData.xAccelSample) < 0.05 && abs(cleanData.yAccelSample) < 0.05){
        xGyroSum = 0;
        yGyroSum = 0;
    }
    //we record xGyroSum and yGyroSum because we use that values for practice and not xGyro,yGyro
    if (mRec){
    QFile recData(QCoreApplication::applicationDirPath() + "/recData.csv");
        if(recData.open(QFile::Append |QFile::Truncate))
        {
            QTextStream output(&recData);
            output << cleanData.xAccelSample <<" "<< cleanData.yAccelSample <<" "<< cleanData.zAccelSample <<" "<< xGyroSum <<" "<< yGyroSum <<" "<< cleanData.zGyroSample << "\n";
        }
    }

    if (yGyroSum > 2){
        ui->addText->setText("Spusti ruku!");

       // player->setMedia(QUrl("qrc:/spustiRuku.mp3"));
       // player->play();
    }
    else if(yGyroSum < -2 && cleanData.zAccelSample < -0.0005){
        ui->addText->setText("Podigni ruku!");

       // player->setMedia(QUrl("qrc:/podigniRuku.mp3"));
       // player->play();

    }
    else if(xGyroSum < -2 || xGyroSum > 2){
        ui->addText->setText("Ispravi ruku!");

       // player->setMedia(QUrl("qrc:/ispraviRuku.mp3"));
       // player->play();

    }
    else if(yGyroSum < -4 || yGyroSum > 4){
        yGyroSum = 0;
    }
    else if(xGyroSum < -4 || xGyroSum > 4){
        xGyroSum = 0;
    }
    else{
        ui->addText->setText("Bravo!");
    }

    if((zGyroTmp > 0 && cleanData.zGyroSample < 0 )||(zGyroTmp < 0 && cleanData.zGyroSample > 0 ))
    {
    speedInterval();
    }
    zGyroTmp = cleanData.zGyroSample;

    moveSlider();



}


void ExerciseWindow::speedInterval()
{

    int milisec = timer.elapsed();
    //ui->time->setText(QString::number(milisec) + " ");
    int index = ui->speedInterval->currentIndex();
    if(index == 1){
        if(milisec < 600){
            ui->addSpeed->setText("Sloweeer");
        }
        else if(milisec > 900){
            ui->addSpeed->setText("Fasteer");
        }
        else ui->addSpeed->setText("Good");
    }
    if(index == 2){
        if(milisec < 900){
            ui->addSpeed->setText("Sloweeer");
        }else if(milisec > 1200){
            ui->addSpeed->setText("Fasteer");
        }
        else ui->addSpeed->setText("Good");
    }
    if(index == 3){
        if(milisec < 1200){
            ui->addSpeed->setText("Sloweeer");
        }else if(milisec > 1500){
            ui->addSpeed->setText("Fasteer");
        }
    }
    if(index == 4){
        if(milisec < 1500){
            ui->addSpeed->setText("Sloweeer");
        }else if(milisec > 2000){
            ui->addSpeed->setText("Fasteer");
        }
    }
    timer.start();


}

void ExerciseWindow::moveSlider()
{
    double x = xGyroSum * 100;
    double y = yGyroSum * 100;
    int valX = qRound(x);
    int valY = qRound(y);
    ui->xGyroSlider->setValue(valX);
    ui->yGyroSlider->setValue(valY);

}

double ExerciseWindow::diffAbs(double value1,double value2){
return sqrt((value1 - value2)*(value1 - value2));
}

double ExerciseWindow::abs(double value){
return sqrt(value*value);
}

void ExerciseWindow::on_stop_clicked()
{
    ui->start->setEnabled(true);
    emit onStop();

}

void ExerciseWindow::on_recData_clicked()
{
    ui->start->click();
    ui->recData->setStyleSheet(QString("QPushButton {background-color: red;}"));
}

void ExerciseWindow::on_stopRec_clicked()
{
    emit onStop();
    ui->recData->setStyleSheet(QString("QPushButton {background-color: white;}"));
    ui->start->setEnabled(true);

}
