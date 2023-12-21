#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QByteArray>
#include <QPixmap>
#include <QWidget>

#include "qrencode.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class widget;
}

QT_END_NAMESPACE

class widget final : public QWidget {
        Q_OBJECT

    public:
        explicit widget(QWidget* parent = nullptr);

        ~widget() override;

    private:
        inline static constexpr int default_version{ 7 };

        Ui::widget* ui;
        QByteArray m_text;
        QPixmap m_QRcode_map;
        QColor background_color{ Qt::black };
        QColor frontend_color{ Qt::white };

        void componant_init();

        void slot_init();

        int get_version();

        QRecLevel get_ec_level();

        QRcode* generate_QRcode();

        void paint_QRcode(const QRcode*);

    private slots:
        void slot_btn_clear();

        void slot_btn_QR_gen();

        void slot_btn_save();

        void slot_btn_bkg_color();

        void slot_btn_ftd_color();
};

#endif
