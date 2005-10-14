#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QLabel *speedLine;
    QLabel *statusLine;
    QPushButton *buttonOk;
    QProgressBar *downloadProgress;
    QPushButton *buttonCancel;
    QTableWidget *filesView;

    void setupUi(QDialog *Dialog)
    {
    Dialog->setObjectName(QString::fromUtf8("Dialog"));
    Dialog->resize(QSize(466, 587).expandedTo(Dialog->minimumSizeHint()));
    gridLayout = new QGridLayout(Dialog);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(8);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    speedLine = new QLabel(Dialog);
    speedLine->setObjectName(QString::fromUtf8("speedLine"));
    speedLine->setFrameShape(QFrame::Box);
    speedLine->setAlignment(Qt::AlignLeading);

    gridLayout->addWidget(speedLine, 1, 2, 1, 1);

    statusLine = new QLabel(Dialog);
    statusLine->setObjectName(QString::fromUtf8("statusLine"));
    statusLine->setFrameShape(QFrame::Box);

    gridLayout->addWidget(statusLine, 1, 0, 1, 2);

    buttonOk = new QPushButton(Dialog);
    buttonOk->setObjectName(QString::fromUtf8("buttonOk"));

    gridLayout->addWidget(buttonOk, 2, 2, 1, 1);

    downloadProgress = new QProgressBar(Dialog);
    downloadProgress->setObjectName(QString::fromUtf8("downloadProgress"));
    downloadProgress->setValue(24);

    gridLayout->addWidget(downloadProgress, 2, 0, 1, 1);

    buttonCancel = new QPushButton(Dialog);
    buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

    gridLayout->addWidget(buttonCancel, 2, 1, 1, 1);

    filesView = new QTableWidget(Dialog);
    filesView->setObjectName(QString::fromUtf8("filesView"));
    filesView->setTabKeyNavigation(false);
    filesView->setAlternatingRowColors(true);
    filesView->setSelectionMode(QAbstractItemView::NoSelection);

    gridLayout->addWidget(filesView, 0, 0, 1, 3);

    retranslateUi(Dialog);

    QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
    speedLine->setText(QApplication::translate("Dialog", "---", 0, QApplication::UnicodeUTF8));
    statusLine->setText(QApplication::translate("Dialog", "", 0, QApplication::UnicodeUTF8));
    buttonOk->setText(QApplication::translate("Dialog", "OK", 0, QApplication::UnicodeUTF8));
    buttonCancel->setText(QApplication::translate("Dialog", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Dialog);
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

#endif // MAINDIALOG_H
