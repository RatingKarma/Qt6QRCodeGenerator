#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class widget; }
QT_END_NAMESPACE

class widget : public QWidget {
    Q_OBJECT

    public:
        explicit widget(QWidget* parent = nullptr);

        ~widget() override;

    private:
        Ui::widget* ui;
        QVector<QString> nums;

    private slots:
        void button_click();
        void button2_click();
        void button3_click();
};

#endif
