#include "mainwindow.h"
#include "ui_mainwindow.h"

static QString array[47]={""};
static int array_len=0;
static int timer_val=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int fd = wiringPiI2CSetup(DEVICE_ID);
    qDebug()<<fd<<DEVICE_ID;
       if (fd == -1) {
           qDebug() << "Failed to init I2C communication.\n";
           //return -1;
       }
       //qDebug() << "I2C communication successfully setup.\n";

       timer = new QTimer(this);
       connect(timer, SIGNAL(timeout()), this,SLOT(testing()));
       timer1 = new QTimer(this);
       connect(timer1, SIGNAL(timeout()), this,SLOT(on_pushButton_8_clicked()));
       timer1->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_3_clicked()
{
    array[0]="mhmm";
    array[1]="shm";
    array[2]="bhm";
    //step 1
   array[3]="bmf 1";
   array[4]="smf 1";
    array[5]="mix 0010 1";//60
    array[6]="mmf 1";
    array[7]="idl 0010";//60
    array[8]="msb 1";

    array[9]="W 90 00 90 00 90 00 90 00";
    //step 2
    array[10]="bmb 1";
    array[11]="msf 1";
    array[12]="mhm";
    array[13]="mix 0010 1";//900
    array[14]="mmf 1";
    array[15]="idl 0010";//90
    array[16]="msb 1";

    array[17]="W 00 00 00 00 00 00 00 00";
    //step 3
    array[18]="bmf 2";
    array[19]="msf 1";
    array[20]="mhm";
    array[21]="mix 0010 1";//120
    array[22]="mmf 1";
    array[23]="idl 0010";//90
    array[24]="msb 1";

    //step 4
    array[25]="bmf 1";
    array[26]="msf 1";
    array[27]="mhm";
    array[28]="mmf 1";
    array[29]="idl 0010";//30
    array[30]="msb 1";

    //step5
    array[31]="W 00 85 00 85 00 85 00 85";
    array[32]="bmf 2";
    array[33]="msf 1";
    array[34]="mhm";
    array[35]="mis 0010 1";//300
    array[36]="mmf 1";
    array[37]="idl 0010";//90
    array[38]="W 00 00 00 00 00 00 00 00";
    array[39]="msb 1";
    //step 6
    array[40]="bmb 4";
    array[41]="msf 1";
    array[42]="mhm";
    array[43]="mix 0010 1";//60
    array[44]="shm";
    array[45]="bhm";
    array[46]="buz 0005";

    array_len=sizeof(array)/sizeof(array[0]);

qDebug()<<array_len;

processing();




   /* Pi2c arduino(7); //Create a new object "arduino" using address "0x07"
       char receive[16]="bmf 1"; //Create a buffer of char (single bytes) for the data
arduino.i2cWrite(receive,16);
//testing();
qDebug()<<"done";
qDebug()<<array[1];
timer->start(5000);
//while(timer->isActive());
qDebug()<<"timer done";
*/

//keep the commands in global array
//after executing 1st write, do a left shift of the array and start timer
// then at end timer functionn, call this same function back

}

void MainWindow::processing()
{
    timer_val=1;
    if(array[0][0]=='W')
    {
        Pi2c arduino(8);
        char* ch;
        QByteArray ba=array[0].toLatin1();
        ch=ba.data();
        //timer1->stop();
        QThread::msleep(100);
        arduino.i2cWrite(ch,30);
        QThread::msleep(100);
        //timer1->start(1000);
        qDebug()<<array[0];
        for(int i=0;i<array_len-1;i++)
        {
            array[i]=array[i+1];
        }
        array[array_len-1]='\0';
        array_len=array_len-1;
    }

    else
    {
        Pi2c arduino(7);
        char* ch;
        QByteArray ba=array[0].toLatin1();
        ch=ba.data();
        //timer1->stop();
        QThread::msleep(100);
        arduino.i2cWrite(ch,30);
        QThread::msleep(100);
        char rc[30];
        arduino.i2cRead(rc,30);
        qDebug()<<"sent="<<ch<<" received="<<rc;
        QString str1=ch;
        QString str2=rc;
        qDebug()<<str1<<str2;

        while(str1!=str2)
        {
           qDebug()<<"Command Mis Matched";
           QThread::msleep(100);
           arduino.i2cWrite(ch,30);
           QThread::msleep(100);
           arduino.i2cRead(rc,30);
           QThread::msleep(100);
           str1=ch;
           str2=rc;
           qDebug()<<str1<<str2;

        }


        qDebug()<<array[0];

        //timer val
        if(array[0].left(3)=="idl" || array[0].left(3)== "mix" || array[0].left(3)== "mis")
        {
            timer_val=array[0].mid(4,4).toInt();
        }
        else
        {
            timer_val=1;
        }

        for(int i=0;i<array_len-1;i++)
        {
            array[i]=array[i+1];
        }
        array[array_len-1]='\0';
        array_len=array_len-1;
    }
    qDebug()<<array_len;
    if(array_len>=0)
    {
         timer->start(timer_val*1000);

        //qDebug()<<timer_val*10;
    }
    qDebug()<<timer_val;
}


void MainWindow::on_pushButton_4_clicked()
{
    Pi2c arduino(7); //Create a new object "arduino" using address "0x07"
       char receive[30]; //Create a buffer of char (single bytes) for the data
    //Receive from the Arduino and put the contents into the "receive" char array
           arduino.i2cRead(receive,30);
            //Print out what the Arduino is sending...
           qDebug() << "Arduino Says: " << receive;

           //Send an 16 bit integer
}

void MainWindow::testing()
{
   Pi2c arduino(7);
   char receive[30];
    //char cmp[5]="done";
    //while(strncmp(receive,"done",4)!=0)
    //{
   //timer1->stop();
   QThread::msleep(100);
  arduino.i2cRead(receive,30);
  QThread::msleep(100);
   //timer1->start(1000);

           //QThread::msleep(500);
           qDebug() << "Arduino Says: " << receive;
           //qDebug()<<strncmp(receive,"done",4);

           if(strncmp(receive,"done",4)==0)
           {
               timer->stop();               
               qDebug()<<"timer ended";
               //call the same function back from here
               if(array_len>0)
               {
                   processing();
               }

           }
           else
           {
               timer->start(500);
           }

    //}
    QThread::msleep(500);
//processing();

}



void MainWindow::on_pushButton_5_clicked()
{
    Pi2c arduino(8); //Create a new object "arduino" using address "0x07"
       char receive[30]="W 00 80 00 80 00 80 00 80"; //Create a buffer of char (single bytes) for the data
arduino.i2cWrite(receive,30);

       //arduino.i2cWriteArduinoInt(4356);
}





void MainWindow::on_pushButton_8_clicked()
{
    Pi2c arduino(8); //Create a new object "arduino" using address "0x07"
       char receive[30]; //Create a buffer of char (single bytes) for the data
    //Receive from the Arduino and put the contents into the "receive" char array
       //timer1->stop();
       QThread::msleep(100);
      arduino.i2cRead(receive,30);
      QThread::msleep(100);
       //timer1->start(1000);
           qDebug()<<receive;
           QString str=receive;
            //Print out what the Arduino is sending...
           //qDebug() << "Arduino Says: " << str.mid(0,2);
           ui->label->setText(str.mid(0,2));
           ui->label_2->setText(str.mid(3,2));
           ui->label_3->setText(str.mid(6,2));
           ui->label_4->setText(str.mid(9,2));
           ui->label_5->setText(str.mid(12,2));
           ui->label_6->setText(str.mid(15,2));
           ui->label_7->setText(str.mid(18,2));
           ui->label_8->setText(str.mid(21,2));


           //Send an 16 bit integer
}
