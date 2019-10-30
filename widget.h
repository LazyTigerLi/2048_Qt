#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPair>
#include <QKeyEvent>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QStack>

enum Direction{Up,Down,Left,Right,Back};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    int number[4][4];
    int sizeOfCube;
    bool animationFinished = true;
    bool isUndo = false;
    int score = 0;
    int best = 0;
    QStack<int> history;
    QPushButton *undoButton;
    QMap<int,QString> backgroundColor = {{2,"238,228,218"},{4,"237,224,200"},{8,"242,177,121"},{16,"245,149,99"},
                                         {32,"246,124,95"},{64,"246,94,59"},{128,"237,207,114"},{256,"237,204,97"},
                                         {512,"237,200,80"},{1024,"239,196,66"},{2048,"238,194,46"},{4096,"60,60,50"},
                                         {8192,"62,57,51"},{16384,"62,57,53"},{32768,"59,59,51"},{65536,"59,59,51"}};
    QLabel *cube[4][4];
    QLabel *scoreLabel = nullptr;
    QLabel *bestLabel = nullptr;
    QLabel *scoreValueLabel = nullptr;
    QLabel *bestValueLabel = nullptr;
    QWidget *gameWidget = nullptr;
    QParallelAnimationGroup *group = nullptr;
    void generateCube();

private slots:
    void moveCubes(Direction dir);
    void updateCubes();
    void undo();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void moveTowards(Direction dir);
};

#endif // WIDGET_H
