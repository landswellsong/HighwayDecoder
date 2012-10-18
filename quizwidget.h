#ifndef QUIZWIDGET_H
#define QUIZWIDGET_H

#include <QWidget>
#include <QHash>
#include <QtXml/QDomDocument>

namespace Ui {
    class QuizWidget;
}

class QuizWidget : public QWidget
{
    Q_OBJECT

public:
    /**
      * Constructor inherited from QWidget
      *
      * @param parent Handle of parenting widget
      */
    explicit QuizWidget(QWidget *parent = 0);
    ~QuizWidget();

public slots:
    /**
      * Clear the question currently displayed
      *
      */
    void clearQuestion();
    /**
      * React on answer selection (highlight buttons and show hint)
      *
      */
    void reactAnswer();
    /**
      * Fill the widget with data and controls corresponding to the question given
      *
      * @param node XML node pointing to the question to be displayed
      */
    void populateQuestion(const QDomNode& );
    
signals:
    /**
      * Emited on right answer
      *
      */
    void rightAnswer();
    /**
      * Emited on wrong answer
      *
      */
    void wrongAnswer();

private:
    Ui::QuizWidget *ui;
    QHash<QWidget*,bool> rightBtn;
};

#endif // QUIZWIDGET_H
