#include "quizwidget.h"
#include "ui_quizwidget.h"
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QPushButton>

QuizWidget::QuizWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuizWidget)
{
    ui->setupUi(this);
    ui->hintLabel->hide();
}

void QuizWidget::clearQuestion()
{
    // Hiding and clearing the hint
    ui->hintLabel->hide();
    ui->hintLabel->setText("");
    // Clearing the browser
    ui->textBrowser->setHtml("");
    // Removing the buttons
    foreach (QObject *i,children())
    {
        QWidget *j=dynamic_cast<QPushButton*>(i);
        if (j!=NULL)
        {
            ui->verticalLayout->removeWidget(j);
            delete j;
        }
    }
    rightBtn.clear();
}

void QuizWidget::populateQuestion(const QDomNode& xdoc)
{
    // Clear if there was anything
    clearQuestion();
    // Check if the node is correct
    QDomElement doc=xdoc.toElement();
    if (! doc.isNull())
        if (doc.nodeName()=="question")
        {
            QString text="";
            QString hint="";
            QStringList answers;
            // TODO Read attr(all)
            QString ID=doc.attribute("id","");
            bool ok=false;
            int id=ID.toInt(&ok);
            if (!ok)
                return;
            
            // Browse through childs
            for (unsigned j=0;j<doc.childNodes().length();j++)
            {
                QDomNode n=doc.childNodes().at(j);
                // Well sanity, y'know
                if (n.isNull() || n.nodeType()!=QDomNode::ElementNode)
                    continue;
                QDomElement i=n.toElement();
                
                if (i.tagName()=="text")
                    text=i.text();
                else if (i.tagName()=="highwayCode")
                    hint=i.text();
//                 else if (i.tagName()=="image")
                else if (i.tagName()=="answer")
                {
                    // Detecting whether that was a correct answer
                    QString isCorrect=i.attribute("isCorrect","false");
                    
                    // Creating the button
                    QPushButton *btn=new QPushButton(QChar('A'+answers.length()));
                    rightBtn[btn]= isCorrect=="true";
                    connect(btn,SIGNAL(clicked()),this,rightBtn[btn]? SIGNAL(rightAnswer()) : SIGNAL(wrongAnswer()));
                    connect(btn,SIGNAL(clicked()),this,SLOT(reactAnswer()));
                    btn->show();
                    ui->verticalLayout->addWidget(btn);
                    // Adding an answer to the list
                    answers<<(i.text());
                }
            }
            // Compose the text
            // TODO HTML sanitation
            text="<p>"+text+"</p><ol style='list-style-type:upper-alpha'>";
            foreach (QString i,answers)
                text+="<li>"+i;
            text+="</ol>";
            // Checking for an image
            if (QFile::exists(QString("images/%1.jpg").arg(id)))
                text=QString("<img src='images/%1.jpg'/>").arg(id)+text;
            
            ui->textBrowser->setHtml(text);
            // Preparing the label
            ui->hintLabel->hide();
            ui->hintLabel->setText(hint);
        }
}

void QuizWidget::reactAnswer()
{
    // Painting the buttons
    // Removing the buttons
    foreach (QObject *i,children())
    {
        QWidget *j=dynamic_cast<QPushButton*>(i);
        if (j!=NULL)
            j->setStyleSheet(!rightBtn[j]? "* { background-color: rgb(255,125,100) }" : 
                                           "* { background-color: rgb(0,255,125) }");
    }
    
    // Showing the label
    ui->hintLabel->show();
}

QuizWidget::~QuizWidget()
{
    delete ui;
}
