#include "widget.h"
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <vector>
#include <QVBoxLayout>
#include <QHBoxLayout>
//#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("QWidget#gamewidget > QLabel{margin:5px;}");
    sizeOfCube = 100;
    group = new QParallelAnimationGroup;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            number[i][j] = 0;
            cube[i][j] = nullptr;
        }
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *upperLayout = new QHBoxLayout;
    QVBoxLayout *lowerLayout = new QVBoxLayout;
    undoButton = new QPushButton(tr("undo"),this);
    scoreLabel = new QLabel(tr("score"),this);
    bestLabel = new QLabel(tr("best"),this);
    scoreValueLabel = new QLabel("0",this);
    bestValueLabel = new QLabel("0",this);
    scoreLabel->setStyleSheet("font:12px Arial Black");
    bestLabel->setStyleSheet("font:12px Arial Black");
    scoreValueLabel->setStyleSheet("font:12px Arial Black");
    bestValueLabel->setStyleSheet("font:12px Arial Black");
    gameWidget = new QWidget(this);
    gameWidget->setObjectName("gamewidget");
    gameWidget->setFixedSize(4 * sizeOfCube,4 * sizeOfCube);
    upperLayout->addWidget(bestLabel);
    upperLayout->addWidget(bestValueLabel);
    upperLayout->addWidget(scoreLabel);
    upperLayout->addWidget(scoreValueLabel);
    upperLayout->addWidget(undoButton);
    lowerLayout->addWidget(gameWidget);
    mainLayout->addLayout(upperLayout);
    mainLayout->addLayout(lowerLayout);
    setLayout(mainLayout);
    connect(this,SIGNAL(moveTowards(Direction)),this,SLOT(moveCubes(Direction)));
    connect(group,SIGNAL(finished()),this,SLOT(updateCubes()));
    connect(undoButton,SIGNAL(clicked(bool)),this,SLOT(undo()));
    generateCube();
    generateCube();
    show();
    setFixedSize(size());
    setFocusPolicy(Qt::ClickFocus);
}

Widget::~Widget()
{
}

void Widget::generateCube()
{
    int x,y;
    while(true)
    {
        int index = QRandomGenerator::global()->bounded(16);
        x = index / 4;
        y = index % 4;
        if(number[x][y] != 0)continue;
        break;
    }
    if(cube[x][y] != nullptr)delete cube[x][y];
    int is2or4 = QRandomGenerator::global()->bounded(10);
    if(is2or4 % 10 == 0)number[x][y] = 4;
    else number[x][y] = 2;
    cube[x][y] = new QLabel(QString::number(number[x][y]),gameWidget);
    cube[x][y]->setGeometry(y * sizeOfCube,x * sizeOfCube,sizeOfCube,sizeOfCube);
    cube[x][y]->setAlignment(Qt::AlignCenter);
    cube[x][y]->setStyleSheet("font:36px Arial Black;color:black;background-color:rgb(" + backgroundColor[number[x][y]] + ");");
    cube[x][y]->show();
}

void Widget::moveCubes(Direction dir)
{
    if(dir == Back)
    {
        undo();
        return;
    }
    int temp[4][4];
    int tempScore = score,tempBest = best;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            temp[i][j] = number[i][j];
    if(dir == Left)
    {
        for(int i = 0; i < 4; i++)
        {
            std::vector<int> initialPos;
            std::vector<int> finalPos;
            bool hasMerged = false;
            for(int j = 0; j < 4; j++)
                if(number[i][j])initialPos.push_back(j);
            if(initialPos.size() == 0)continue;
            finalPos.push_back(0);
            if(initialPos.size() >= 2)
            {
                if(number[i][initialPos[0]] == number[i][initialPos[1]])
                {
                    hasMerged = true;
                    score += 2 * number[i][initialPos[0]];
                    finalPos.push_back(0);
                }
                else finalPos.push_back(1);
            }
            if(initialPos.size() >= 3)
            {
                if(hasMerged)
                {
                    hasMerged = false;
                    finalPos.push_back(1);
                }
                else if(number[i][initialPos[1]] == number[i][initialPos[2]])
                {
                    hasMerged = true;
                    score += 2 * number[i][initialPos[1]];
                    finalPos.push_back(1);
                }
                else finalPos.push_back(2);
            }
            if(initialPos.size() == 4)
            {
                if(hasMerged)finalPos.push_back(2);
                else if(number[i][initialPos[2]] == number[i][initialPos[3]])
                {
                    score += 2 * number[i][initialPos[2]];
                    finalPos.push_back(finalPos[finalPos.size() - 1]);
                }
                else finalPos.push_back(finalPos[finalPos.size() - 1] + 1);
            }
            int temp[4] = {0,0,0,0};
            for(int j = 0; j < initialPos.size(); j++)
            {
                temp[finalPos[j]] += number[i][initialPos[j]];
                if(initialPos[j] != finalPos[j])
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(cube[i][initialPos[j]],"pos");
                    animation->setDuration(100);
                    animation->setStartValue(QPoint(initialPos[j] * sizeOfCube, i * sizeOfCube));
                    animation->setEndValue(QPoint(finalPos[j] * sizeOfCube, i * sizeOfCube));
                    group->addAnimation(animation);
                }
            }
            for(int j = 0; j < 4; j++)number[i][j] = temp[j];
        }
    }
    else if(dir == Right)
    {
        for(int i = 0; i < 4; i++)
        {
            std::vector<int> initialPos;
            std::vector<int> finalPos;
            bool hasMerged = false;
            for(int j = 3; j >= 0; j--)
                if(number[i][j])initialPos.push_back(j);
            if(initialPos.size() == 0)continue;
            finalPos.push_back(3);
            if(initialPos.size() >= 2)
            {
                if(number[i][initialPos[0]] == number[i][initialPos[1]])
                {
                    hasMerged = true;
                    score += 2 * number[i][initialPos[0]];
                    finalPos.push_back(3);
                }
                else finalPos.push_back(2);
            }
            if(initialPos.size() >= 3)
            {
                if(hasMerged)
                {
                    hasMerged = false;
                    finalPos.push_back(2);
                }
                else if(number[i][initialPos[1]] == number[i][initialPos[2]])
                {
                    hasMerged = true;
                    score += 2 * number[i][initialPos[1]];
                    finalPos.push_back(2);
                }
                else finalPos.push_back(1);
            }
            if(initialPos.size() == 4)
            {
                if(hasMerged)finalPos.push_back(1);
                else if(number[i][initialPos[2]] == number[i][initialPos[3]])
                {
                    score += 2 * number[i][initialPos[2]];
                    finalPos.push_back(finalPos[finalPos.size() - 1]);
                }
                else finalPos.push_back(finalPos[finalPos.size() - 1] - 1);
            }
            int temp[4] = {0,0,0,0};
            for(int j = 0; j < initialPos.size(); j++)
            {
                temp[finalPos[j]] += number[i][initialPos[j]];
                if(initialPos[j] != finalPos[j])
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(cube[i][initialPos[j]],"pos");
                    animation->setDuration(100);
                    animation->setStartValue(QPoint(initialPos[j] * sizeOfCube, i * sizeOfCube));
                    animation->setEndValue(QPoint(finalPos[j] * sizeOfCube, i * sizeOfCube));
                    group->addAnimation(animation);
                }
            }
            for(int j = 0; j < 4; j++)number[i][j] = temp[j];
        }
    }
    else if(dir == Up)
    {
        for(int j = 0; j < 4; j++)
        {
            std::vector<int> initialPos;
            std::vector<int> finalPos;
            bool hasMerged = false;
            for(int i = 0; i < 4; i++)
                if(number[i][j])initialPos.push_back(i);
            if(initialPos.size() == 0)continue;
            finalPos.push_back(0);
            if(initialPos.size() >= 2)
            {
                if(number[initialPos[0]][j] == number[initialPos[1]][j])
                {
                    hasMerged = true;
                    score += 2 * number[initialPos[0]][j];
                    finalPos.push_back(0);
                }
                else finalPos.push_back(1);
            }
            if(initialPos.size() >= 3)
            {
                if(hasMerged)
                {
                    hasMerged = false;
                    finalPos.push_back(1);
                }
                else if(number[initialPos[1]][j] == number[initialPos[2]][j])
                {
                    hasMerged = true;
                    score += 2 * number[initialPos[1]][j];
                    finalPos.push_back(1);
                }
                else finalPos.push_back(2);
            }
            if(initialPos.size() == 4)
            {
                if(hasMerged)finalPos.push_back(2);
                else if(number[initialPos[2]][j] == number[initialPos[3]][j])
                {
                    score += 2 * number[initialPos[2]][j];
                    finalPos.push_back(finalPos[finalPos.size() - 1]);
                }
                else finalPos.push_back(finalPos[finalPos.size() - 1] + 1);
            }
            int temp[4] = {0,0,0,0};
            for(int i = 0; i < initialPos.size(); i++)
            {
                temp[finalPos[i]] += number[initialPos[i]][j];
                if(initialPos[i] != finalPos[i])
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(cube[initialPos[i]][j],"pos");
                    animation->setDuration(100);
                    animation->setStartValue(QPoint(j * sizeOfCube, initialPos[i] * sizeOfCube));
                    animation->setEndValue(QPoint(j * sizeOfCube, finalPos[i] * sizeOfCube));
                    group->addAnimation(animation);
                }
            }
            for(int i = 0; i < 4; i++)number[i][j] = temp[i];
        }
    }
    else if(dir == Down)
    {
        for(int j = 0; j < 4; j++)
        {
            std::vector<int> initialPos;
            std::vector<int> finalPos;
            bool hasMerged = false;
            for(int i = 3; i >= 0; i--)
                if(number[i][j])initialPos.push_back(i);
            if(initialPos.size() == 0)continue;
            finalPos.push_back(3);
            if(initialPos.size() >= 2)
            {
                if(number[initialPos[0]][j] == number[initialPos[1]][j])
                {
                    hasMerged = true;
                    score += 2 * number[initialPos[0]][j];
                    finalPos.push_back(3);
                }
                else finalPos.push_back(2);
            }
            if(initialPos.size() >= 3)
            {
                if(hasMerged)
                {
                    hasMerged = false;
                    finalPos.push_back(2);
                }
                else if(number[initialPos[1]][j] == number[initialPos[2]][j])
                {
                    hasMerged = true;
                    score += 2 * number[initialPos[1]][j];
                    finalPos.push_back(2);
                }
                else finalPos.push_back(1);
            }
            if(initialPos.size() == 4)
            {
                if(hasMerged)finalPos.push_back(1);
                else if(number[initialPos[2]][j] == number[initialPos[3]][j])
                {
                    score += 2 * number[initialPos[2]][j];
                    finalPos.push_back(finalPos[finalPos.size() - 1]);
                }
                else finalPos.push_back(finalPos[finalPos.size() - 1] - 1);
            }
            int temp[4] = {0,0,0,0};
            for(int i = 0; i < initialPos.size(); i++)
            {
                temp[finalPos[i]] += number[initialPos[i]][j];
                if(initialPos[i] != finalPos[i])
                {
                    QPropertyAnimation *animation = new QPropertyAnimation(cube[initialPos[i]][j],"pos");
                    animation->setDuration(100);
                    animation->setStartValue(QPoint(j * sizeOfCube, initialPos[i] * sizeOfCube));
                    animation->setEndValue(QPoint(j * sizeOfCube, finalPos[i] * sizeOfCube));
                    group->addAnimation(animation);
                }
            }
            for(int i = 0; i < 4; i++)number[i][j] = temp[i];
        }
    }
    if(group->animationCount() != 0)
    {
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                history.push(temp[i][j]);
        history.push(tempScore);
        history.push(tempBest);
    }
    animationFinished = false;  //this should be placed forward considering the size of group is zero
    group->start();
}

//this could only be implemented after the animation has finished
void Widget::updateCubes()
{
    animationFinished = true;
    if(group->animationCount() == 0 && !isUndo)return;
    group->clear();
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            best = (number[i][j] > best) ? number[i][j] : best;
            if(number[i][j] == 0)
            {
                if(cube[i][j] != nullptr)delete cube[i][j];
                cube[i][j] = nullptr;
            }
            else if(cube[i][j] == nullptr)
            {
                cube[i][j] = new QLabel(QString::number(number[i][j]),gameWidget);
                cube[i][j]->setGeometry(j * sizeOfCube, i * sizeOfCube,sizeOfCube,sizeOfCube);
                cube[i][j]->setAlignment(Qt::AlignCenter);
                cube[i][j]->show();
            }
            else
            {
                cube[i][j]->setText(QString::number(number[i][j]));
                cube[i][j]->setGeometry(j * sizeOfCube, i * sizeOfCube,sizeOfCube,sizeOfCube);  //this must be set
                cube[i][j]->show();
            }
            if(number[i][j] != 0)
            {
                if(number[i][j] >= 1024)cube[i][j]->setStyleSheet("font:24px Arial Black;color:white;background-color:rgb(" + backgroundColor[number[i][j]] + ");");
                else if(number[i][j] >= 8 && number[i][j] < 1024)cube[i][j]->setStyleSheet("font:36px Arial Black;color:white;background-color:rgb(" + backgroundColor[number[i][j]] + ");");
                else cube[i][j]->setStyleSheet("font:36px Arial Black;color:black;background-color:rgb(" + backgroundColor[number[i][j]] + ");");
            }
        }
    scoreValueLabel->setText(QString::number(score));
    bestValueLabel->setText(QString::number(best));
    if(!isUndo)generateCube();         //This should be implemented after addressing the cubes
    /*QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            file.write((QString::number(number[i][j])).toLatin1());
            file.write(QString(" ").toLatin1());
        }
        file.write(QString("\n").toLatin1());
    }
    file.close();*/
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(!animationFinished)return;
    if(event->key() == Qt::Key_Up)emit moveTowards(Up);
    else if(event->key() == Qt::Key_Down)emit moveTowards(Down);
    else if(event->key() == Qt::Key_Left)emit moveTowards(Left);
    else if(event->key() == Qt::Key_Right)emit moveTowards(Right);
    else if(event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier))emit moveTowards(Back);
}

void Widget::undo()
{
    if(!animationFinished)return;
    if(history.isEmpty())return;
    score = history.pop();
    best = history.pop();
    for(int i = 15; i >= 0; i--)
        number[i / 4][i % 4] = history.pop();
    isUndo = true;
    updateCubes();
    isUndo = false;
}
