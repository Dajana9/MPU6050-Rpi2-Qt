#include "ExerciseWindow.h"
#include "ui_ExerciseWindow.h"
#include <glob.h>
#include "ShowRecData.h"

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

    ui->xGyroSlider->setMinimum(-90);
    ui->xGyroSlider->setMaximum(90);
    ui->yGyroSlider->setMinimum(-90);
    ui->yGyroSlider->setMaximum(90);

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

    // sum yGyro values so that the angle actualy stays in position as long
    // as we dont move the hand

    double xGyro = cleanData.xGyroSample * 9; // so that sumated value at 90° is 90° :)
    double yGyro = cleanData.yGyroSample * 9;
    double zGyro = cleanData.zGyroSample * 36;


    if(diffAbs(xGyro,xGyroTmp) > 0.01){
        xGyroSum +=xGyro;
    }
    xGyroTmp = xGyro;

    if(diffAbs(yGyro,yGyroTmp) > 0.01){
        yGyroSum +=yGyro;
    }
    yGyroTmp = yGyro;

    //remove the drift with help of accel values
    if(abs(cleanData.xAccelSample) < 0.05 && abs(cleanData.yAccelSample) < 0.05){
        xGyroSum = 0;
        yGyroSum = 0;
    }
    double xAxisMove = 0.3 * xGyroSum + 0.7 * cleanData.yAccelSample * 90; //xGyro axis rotation
    double yAxisMove = 0.3 * yGyroSum - 0.7 * cleanData.xAccelSample * 90; //yGyr0 - xAccel is different orientation so it must be - before

    //we record xGyroSum and yGyroSum because we use that values for practice and not xGyro,yGyro
    if (mRec){
    QFile recData(QCoreApplication::applicationDirPath() + "/recData.csv");
        if(recData.open(QFile::Append |QFile::Truncate))
        {
            QTextStream output(&recData);
            output << cleanData.xAccelSample * -90 <<" "<< cleanData.yAccelSample * 90 <<" "<< cleanData.zAccelSample * 90 <<" "<< xGyroSum <<" "<< yGyroSum <<" "<< zGyro << "\n";
        }
    }
    else
        recData.close();

    int valX = qRound(xAxisMove);
    int valY = qRound(yAxisMove);
    ui->xGyroSlider->setValue(valX); // correct your hand movemant
    ui->yGyroSlider->setValue(valY);// raise your hand / lower your arm


    int flag = 0;
    if (xAxisMove > 30){
        ui->addText->setText("Spusti ruku!");
       // player->setMedia(QUrl("qrc:/spustiRuku.mp3"));
       // player->play();
        flag = 1;
    }
    else if(xAxisMove < -30){
        ui->addText->setText("Podigni ruku!");
       // player->setMedia(QUrl("qrc:/podigniRuku.mp3"));
       // player->play();
        flag = 1;

    }
    else if(yAxisMove < -30 || yAxisMove > 30){
        ui->addText->setText("Ispravi ruku!");
       // player->setMedia(QUrl("qrc:/ispraviRuku.mp3"));
       // player->play();
        flag = 1;

    }
    // drift can cause values to grow to much so we need to secure the possibility
    else if(yAxisMove < -60 || yAxisMove > 60){
        yGyroSum = 0;
    }
    else if(xAxisMove < -60 || xAxisMove > 60){
        xGyroSum = 0;
    }
    else{
        ui->addText->setText("Bravo!");
    }

    if((zGyroTmp > 0 && cleanData.zGyroSample < 0 )||(zGyroTmp < 0 && cleanData.zGyroSample > 0 ))
    {
    speedInterval();
    if (flag == 0){
        countExercise++;
        ui->counter->setText(QString::number(countExercise));
    }

    }
    zGyroTmp = zGyro;
}


void ExerciseWindow::speedInterval()
{

    int milisec = timer.elapsed();
    int index = ui->speedInterval->currentIndex();
    if(index == 1){
        if(milisec < 200){
            ui->addSpeed->setText("Sporije");
        }
        else if(milisec > 800){
            ui->addSpeed->setText("Brze");
        }
        else ui->addSpeed->setText("Bravo");
    }
    if(index == 2){
        if(milisec < 800){
            ui->addSpeed->setText("Sporije");
        }else if(milisec > 1300){
            ui->addSpeed->setText("Brze");
        }
        else ui->addSpeed->setText("Bravo");
    }
    if(index == 3){
        if(milisec < 1300){
            ui->addSpeed->setText("Sporije");
        }else if(milisec > 2000){
            ui->addSpeed->setText("Brze");
        }else
            ui->addSpeed->setText("Bravo");
    }
    timer.start();
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
    countExercise = 0;
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

void ExerciseWindow::on_showRec_clicked()
{
    emit onStop();
    ShowRecData* showrecdata = new ShowRecData(this);
    showrecdata->show();
}
