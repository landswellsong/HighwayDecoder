#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QMap>
#include <QFileDialog>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    qw(new QuizWidget(NULL)),
    xml(NULL),
    indicator(new QLabel(NULL)),
    progress(new QProgressBar(NULL)),
    right(0),wrong(0)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(qw);
    connect(ui->actionLoad,SIGNAL(activated()),this,SLOT(loadXMLQuestions()));
    connect(ui->treeWidget,SIGNAL(itemActivated(QTreeWidgetItem *,int)),this,SLOT(treeHandler(QTreeWidgetItem *,int)));
    connect(qw,SIGNAL(rightAnswer()),this,SLOT(rightAnswer()));
    connect(qw,SIGNAL(wrongAnswer()),this,SLOT(wrongAnswer()));
    ui->statusBar->addWidget(indicator);
    ui->statusBar->addWidget(progress);
    indicator->hide();
    progress->hide();
    progress->setRange(0,100);
}

void MainWindow::unloadXMLQuestions()
{
    // TODO
    indicator->hide();
    progress->hide();
    right=0; wrong=0;
}

void MainWindow::loadXMLQuestions()
{
    // Get the file name
//    QString fileName=QFileDialog::getOpenFileName(NULL,trUtf8("Open File"),"",trUtf8("XML documents (*.xml)"));
    QString fileName=":/questions.xml";
    if (fileName!="")
    {
        // Open XML document and parse it
        xml=new QDomDocument("highwayCode");
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(NULL,trUtf8("Error"),trUtf8("Cannot open XML file for reading."));
            delete xml;
            xml=NULL;
            return;
        }
        QString error;
        int line;
        if (!xml->setContent(&file,false,&error,&line))
        {
            QMessageBox::critical(NULL,trUtf8("Error"),trUtf8("Error parsing XML file, %2 at line %1.").arg(line).arg(error));
            file.close();
            delete xml;
            xml=NULL;
            return;
        }
        file.close();
        
        // Clear out
        unloadXMLQuestions();
        
        // Find the root element and collect the linkage data TODO
        QDomNode root=xml->documentElement();
        QList<StringNInt> topics;
        QMap<int,QList<int> > questions;
        for (QDomNode i=root.firstChild();!i.isNull();i=i.nextSibling())
        {
            if (i.nodeName()=="topic")
            {
                QDomElement j=i.toElement();
                if (!j.isNull())
                {
                    QString id=j.attribute("id","");
                    bool validID=false;
                    int iD=id.toInt(&validID);
                    if (validID)
                        topics<< QPair<QString,int>(j.text(),iD);
                }
            }
            if (i.nodeName()=="question")
            {
                QDomElement j=i.toElement();
                if (!j.isNull())
                {
                    QString id=j.attribute("id","");
                    QString topic=j.attribute("topic","");
                   
                    bool validID=false,validTopic=false;
                    int iD=id.toInt(&validID);
                    int topiC=topic.toInt(&validTopic);
                    if (validID && validTopic)
                    {
                        questions[topiC]<<iD;
                        resolver[iD]=i;
                    }
                }
            }
        }
        
        // Populating the actual tree widget
        foreach (StringNInt p,topics)
        {
            // Populating the topics
            QTreeWidgetItem *item=new QTreeWidgetItem(QStringList(p.first));
            item->setData(0,Qt::UserRole,p.second);
            
            // Filling corresponding questions
            foreach (int i,questions[p.second])
            {
                QTreeWidgetItem *subitem=new QTreeWidgetItem(QStringList(QString("#%1").arg(i)));
                subitem->setData(0,Qt::UserRole,i);
                item->addChild(subitem);
            }
            
            ui->treeWidget->addTopLevelItem(item);
        }
    }
}

void MainWindow::treeHandler(QTreeWidgetItem * item,int)
{
    // Convert data and do the checks
    QVariant dt=item->data(0,Qt::UserRole);
    bool ok=false;
    int id=dt.toInt(&ok);
    
    if (ok && resolver.find(id)!=resolver.end())
        qw->populateQuestion(resolver[id]);
    else
        qw->clearQuestion();

}

void MainWindow::rightAnswer()
{
    right++;
    updateStatus();
}

void MainWindow::wrongAnswer()
{
    wrong++;
    updateStatus();
}

void MainWindow::updateStatus()
{
    // Setting values
    indicator->setText(trUtf8("Right: %1 Wrong: %2 Score: ").arg(right).arg(wrong));
    progress->setRange(0,right+wrong);
    progress->setValue(right);
    
    // Showing statuses
    indicator->show();
    progress->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
