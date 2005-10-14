#ifndef IMAGESYNCGUI_H
#define IMAGESYNCGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QLabel *statusLine;
    QLabel *speedLine;
    QPushButton *buttonGo;
    QPushButton *buttonCancel;
    QProgressBar *downloadProgress;
    QTableWidget *filesView;
    QWidget *tab1;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QLineEdit *textEditSource;
    QLabel *label;
    QLabel *label_2;
    QPushButton *buttonBrowseSource;
    QPushButton *buttonBrowseDestination;
    QLineEdit *textEditDestination;
    QPushButton *buttonSaveConfiguration;

    void setupUi(QMainWindow *MainWindow)
    {
    MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(QSize(556, 614).expandedTo(MainWindow->minimumSizeHint()));
    centralWidget = new QWidget(MainWindow);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    tabWidget = new QTabWidget(centralWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setGeometry(QRect(10, 10, 538, 596));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    gridLayout = new QGridLayout(tab);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(8);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    statusLine = new QLabel(tab);
    statusLine->setObjectName(QString::fromUtf8("statusLine"));
    statusLine->setFrameShape(QFrame::Box);

    gridLayout->addWidget(statusLine, 2, 0, 1, 2);

    speedLine = new QLabel(tab);
    speedLine->setObjectName(QString::fromUtf8("speedLine"));
    speedLine->setFrameShape(QFrame::Box);

    gridLayout->addWidget(speedLine, 2, 2, 1, 1);

    buttonGo = new QPushButton(tab);
    buttonGo->setObjectName(QString::fromUtf8("buttonGo"));

    gridLayout->addWidget(buttonGo, 1, 2, 1, 1);

    buttonCancel = new QPushButton(tab);
    buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

    gridLayout->addWidget(buttonCancel, 1, 1, 1, 1);

    downloadProgress = new QProgressBar(tab);
    downloadProgress->setObjectName(QString::fromUtf8("downloadProgress"));
    downloadProgress->setValue(24);

    gridLayout->addWidget(downloadProgress, 1, 0, 1, 1);

    filesView = new QTableWidget(tab);
    filesView->setObjectName(QString::fromUtf8("filesView"));
    filesView->setColumnCount(2);

    gridLayout->addWidget(filesView, 0, 0, 1, 3);

    tabWidget->addTab(tab, QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
    tab1 = new QWidget();
    tab1->setObjectName(QString::fromUtf8("tab1"));
    groupBox = new QGroupBox(tab1);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(9, 9, 516, 101));
    gridLayout1 = new QGridLayout(groupBox);
    gridLayout1->setSpacing(6);
    gridLayout1->setMargin(8);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    textEditSource = new QLineEdit(groupBox);
    textEditSource->setObjectName(QString::fromUtf8("textEditSource"));

    gridLayout1->addWidget(textEditSource, 0, 1, 1, 1);

    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout1->addWidget(label, 0, 0, 1, 1);

    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout1->addWidget(label_2, 1, 0, 1, 1);

    buttonBrowseSource = new QPushButton(groupBox);
    buttonBrowseSource->setObjectName(QString::fromUtf8("buttonBrowseSource"));

    gridLayout1->addWidget(buttonBrowseSource, 0, 2, 1, 1);

    buttonBrowseDestination = new QPushButton(groupBox);
    buttonBrowseDestination->setObjectName(QString::fromUtf8("buttonBrowseDestination"));

    gridLayout1->addWidget(buttonBrowseDestination, 1, 2, 1, 1);

    textEditDestination = new QLineEdit(groupBox);
    textEditDestination->setObjectName(QString::fromUtf8("textEditDestination"));

    gridLayout1->addWidget(textEditDestination, 1, 1, 1, 1);

    buttonSaveConfiguration = new QPushButton(tab1);
    buttonSaveConfiguration->setObjectName(QString::fromUtf8("buttonSaveConfiguration"));
    buttonSaveConfiguration->setGeometry(QRect(440, 540, 85, 24));
    tabWidget->addTab(tab1, QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
    MainWindow->setCentralWidget(centralWidget);
    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    statusLine->setText(QApplication::translate("MainWindow", "", 0, QApplication::UnicodeUTF8));
    speedLine->setText(QApplication::translate("MainWindow", "---", 0, QApplication::UnicodeUTF8));
    buttonGo->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
    buttonCancel->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("MainWindow", "Directories", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("MainWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\" white-space: pre-wrap; font-family:Sans Serif; font-weight:400; font-style:normal; text-decoration:none;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Source:</p></body></html>", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("MainWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\" white-space: pre-wrap; font-family:Sans Serif; font-weight:400; font-style:normal; text-decoration:none;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Destination:</p></body></html>", 0, QApplication::UnicodeUTF8));
    buttonBrowseSource->setText(QApplication::translate("MainWindow", "Browse", 0, QApplication::UnicodeUTF8));
    buttonBrowseDestination->setText(QApplication::translate("MainWindow", "Browse", 0, QApplication::UnicodeUTF8));
    buttonSaveConfiguration->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // IMAGESYNCGUI_H
