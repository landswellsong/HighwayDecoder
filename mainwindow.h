#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPair>
#include <QMainWindow>
#include <QDomDocument>
#include <QMap>
#include <QTreeWidget>
#include <QLabel>
#include <QProgressBar>
#include "quizwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
      * Constructor inherited from QWidget
      *
      * @param parent Handle of parenting widget
      */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    /**
      * Load the questions with giving the file selection dialog
      *
      */
    void loadXMLQuestions();
    /**
      * Clear the selection list and all corresponding widgets
      *
      */
    void unloadXMLQuestions();
    /**
      * Reaction on list item activation, loads the corresponding question
      *
      * @param parent Handle of the activated item
      * @param column Unused parameter
      */
    void treeHandler(QTreeWidgetItem *,int);
    /**
      * Right answer click handler, increments the score and updates the statusbar
      *
      */
    void rightAnswer();
    /**
      * Wrong answer click handler, decrements the score and updates the statusbar
      *
      */
    void wrongAnswer();

private:
    Ui::MainWindow *ui;
    QuizWidget *qw;
    QDomDocument *xml;
    QLabel *indicator;
    QProgressBar *progress;
    int right,wrong;
    QMap<int,QDomNode> resolver;
    typedef QPair<QString,int> StringNInt;
    
    void updateStatus();
};

#endif // MAINWINDOW_H
