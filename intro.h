#ifndef INTRO_H
#define INTRO_H

#include <QWidget>
#include "ui_intro.h"

namespace Ui {
class Intro;
}

class Intro : public QWidget , public Ui::Intro
{
    Q_OBJECT
    
public:
    explicit Intro(QWidget *parent = 0);
    
};

#endif // INTRO_H
