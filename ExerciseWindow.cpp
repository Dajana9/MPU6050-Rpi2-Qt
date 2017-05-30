#include "ExerciseWindow.h"
#include "ui_ExerciseWindow.h"

ExerciseWindow::ExerciseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExerciseWindow)
{
    ui->setupUi(this);
    ui->recData->setStyleSheet(QString("QPushButton {background-color: white;}"));
    ui->stopRec->setStyleSheet(QString("QPushButton {background-color: white;}"));

}

ExerciseWindow::~ExerciseWindow()
{
    emit onStop();
    delete ui;
}

void ExerciseWindow::on_start_clicked()
{
    ui->start->setDisabled(true);

    qRegisterMetaType<MainWindow::data>("MainWindow::data");

    connect(&mainwin,SIGNAL(onNumber(MainWindow::data)),this,SLOT(newNumber(MainWindow::data)));
    connect(this,&ExerciseWindow::onStop, &mainwin,&MainWindow::stop);

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
    QFile recData("recData.csv");
        if(recData.open(QFile::Append |QFile::Truncate))
        {
            QTextStream output(&recData);
            output << cleanData.xAccelSample <<" "<< cleanData.yAccelSample <<" "<< cleanData.zAccelSample <<" "<< xGyroSum <<" "<< yGyroSum <<" "<< cleanData.zGyroSample << "\n";
        }
    }
    if (yGyroSum > 2){
        ui->toDo->setText("Spusti ruku!");
       // player->setMedia(QUrl("qrc:/spustiRuku.mp3"));
       // player->play();
    }
    else if(yGyroSum < -2 && cleanData.zAccelSample < -0.0005){
        ui->toDo->setText("Podigni ruku!");
       // player->setMedia(QUrl("qrc:/podigniRuku.mp3"));
       // player->play();

    }
    else if(xGyroSum < -2 || xGyroSum > 2){
        ui->toDo->setText("Ispravi ruku!");
       // player->setMedia(QUrl("qrc:/ispraviRuku.mp3"));
       // player->play();

    }
    else{
        ui->toDo->setText("Bravo!"); // wait a bit because other messages wont be seen!
    }

    //losije od zGyro ali vidi se rezultat
    cleanData.xAccelSample = cos(cleanData.zGyroSample)*cleanData.xAccelSample + sin(cleanData.zGyroSample)*cleanData.yAccelSample; //akceleracija u X smjeru
    cleanData.yAccelSample = -sin(cleanData.zGyroSample)*cleanData.xAccelSample + cos(cleanData.zGyroSample)*cleanData.yAccelSample;
    // YAcc[i]=-sin(kut[i])*XAcc1[i]+cos(kut[i])*YAcc1[i]; //akceleracija u Y smjeru
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
