#include <cerrno>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "qrencode.h"

#include "widget.hpp"
#include "ui_widget.h"

widget::widget(QWidget* parent) :
    QWidget(parent, Qt::Widget), ui(new Ui::widget) {
    ui->setupUi(this);
    this->setFixedSize(550, 350);
    this->setWindowTitle("二维码生成");
    componant_init();
    slot_init();
}

void widget::componant_init() {
    ui->text_edit->clear();
    ui->QRcode_box->setStyleSheet("border: 1px solid black;");
    ui->checkbox_casesensitive->setChecked(true);
    ui->combobox_correction_lvl->setEditable(false);
    ui->lineedit_version->setText(QString::number(default_version));
}

void widget::slot_init() {
    connect(ui->btn_clear, &QPushButton::clicked, this, &widget::slot_btn_clear);
    connect(ui->btn_QR_gen, &QPushButton::clicked, this, &widget::slot_btn_QR_gen);
    connect(ui->btn_save, &QPushButton::clicked, this, &widget::slot_btn_save);
    connect(ui->btn_color_select1, &QPushButton::clicked, this, &widget::slot_btn_bkg_color);
    connect(ui->btn_color_select2, &QPushButton::clicked, this, &widget::slot_btn_ftd_color);
}

int widget::get_version() {
    QString version_str{ ui->lineedit_version->text() };
    if (version_str.isEmpty()) {
        return default_version;
    }
    int version{ version_str.toInt() };
    if (version < 0 || version > 10) {
        QMessageBox::critical(this, "非法尺寸等级", "已更改为默认尺寸等级");
        version = default_version;
    }
    ui->lineedit_version->setText(QString::number(version));
    return version;
}

QRecLevel widget::get_ec_level() {
    auto cur_level{ ui->combobox_correction_lvl->currentIndex() };
    switch (cur_level) {
        case 0:
            return QR_ECLEVEL_L;
        case 1:
            return QR_ECLEVEL_M;
        case 2:
            return QR_ECLEVEL_Q;
        case 3:
            return QR_ECLEVEL_H;
        default:
            return QR_ECLEVEL_L;
    }
}

QRcode* widget::generate_QRcode() {
    const QString text{ ui->text_edit->toPlainText() };
    if (text.isEmpty()) {
        errno = ERANGE;
        return nullptr;
    }

    m_text = text.toUtf8();
    QRecLevel rec_level{ get_ec_level() };
    bool case_sensitive{ ui->checkbox_casesensitive->isChecked() };
    int version{ get_version() };
    return QRcode_encodeString(m_text.data(), version, rec_level, QR_MODE_8, case_sensitive);
}

void widget::paint_QRcode(const QRcode* qrcode) {
    const auto qrcode_w{ qrcode->width };
    auto qrcode_data{ qrcode->data };

    QImage QRcode_img{ QSize{ qrcode_w, qrcode_w }, QImage::Format_RGB16 };
    for (int x{ }; x < qrcode_w; ++x) {
        for (int y{ }; y < qrcode_w; ++y) {
            if (*qrcode_data & 1) {
                QRcode_img.setPixel(x, y, background_color.rgb());
            } else {
                QRcode_img.setPixel(x, y, frontend_color.rgb());
            }
            ++qrcode_data;
        }
    }
    QRcode_img = QRcode_img.scaled(ui->QRcode_box->width(), ui->QRcode_box->width());
    m_QRcode_map = QPixmap::fromImage(QRcode_img);
    ui->QRcode_box->setPixmap(m_QRcode_map);
}

void widget::slot_btn_clear() {
    ui->text_edit->clear();
}

void widget::slot_btn_bkg_color() {
    background_color = QColorDialog::getColor();
}

void widget::slot_btn_ftd_color() {
    frontend_color = QColorDialog::getColor();
}

void widget::slot_btn_QR_gen() {
    const auto qrcode{ generate_QRcode() };
    if (qrcode == nullptr) {
        switch (errno) {
            case EINVAL:
                QMessageBox::critical(this, "发生了错误", "非法的信息输入");
                break;
            case ENOMEM:
                QMessageBox::critical(this, "发生了错误", "没有足够的内存创建二维码");
                break;
            case ERANGE:
                QMessageBox::critical(this, "发生了错误", "信息的长度过长或为0");
                break;
            default:
                break;
        }
        return;
    }
    paint_QRcode(qrcode);
    QRcode_free(qrcode);
}

void widget::slot_btn_save() {
    auto filename{ QFileDialog::getSaveFileName(this, "选择路径", "./", "图像(*.png)") };
    if (filename.isEmpty()) {
        QMessageBox::critical(this, "错误", "非法路径");
    } else {
        if (const bool state{ m_QRcode_map.save(filename) }; !state) {
            QMessageBox::critical(this, "错误", "文件保存失败");
        } else {
            QMessageBox::about(this, "成功", "文件保存成功");
        }
    }
}

widget::~widget() {
    delete ui;
}
