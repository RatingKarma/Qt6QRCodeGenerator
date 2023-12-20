#include <QMessageBox>
#include <QRegularExpression>

#include "widget.hpp"
#include "ui_widget.h"

widget::widget(QWidget* parent) : QWidget(parent, Qt::Widget | Qt::WindowStaysOnTopHint), ui(new Ui::widget) {
    ui->setupUi(this);
    this->setFixedSize(500, 300);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(button_click()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(button2_click()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(button3_click()));
}

void widget::button_click() {
    const auto text_str{ ui->lineEdit->text() };
    if (text_str.isEmpty()) {
        return;
    }
    const QRegularExpression split_expr("\\s+");
    QStringList list{ text_str.split(split_expr) };
    for (QString& item : list) {
        nums.push_back(std::move(item));
    }
    ui->lineEdit->clear();
}

void widget::button2_click() {
    ui->lineEdit->clear();
    nums.clear();
}

void widget::button3_click() {
    QString result{ };
    for (auto&& item : nums) {
        result.append(item + " ");
    }
    QMessageBox box{ this };
    box.setText(result);
    box.exec();
}

widget::~widget() {
    delete ui;
}
