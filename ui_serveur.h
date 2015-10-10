/********************************************************************************
** Form generated from reading ui file 'serveur.ui'
**
** Created: Fri 28. Aug 14:59:24 2015
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SERVEUR_H
#define UI_SERVEUR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Serveur
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout;
    QTextEdit *rapport;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *port;
    QSpacerItem *horizontalSpacer;
    QPushButton *connect;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *rap;
    QSpacerItem *verticalSpacer;
    QTreeView *dossier;
    QStatusBar *etat;
    QToolBar *sMenu;
    QDockWidget *boiteRapport;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_3;
    QTextBrowser *visual;

    void setupUi(QMainWindow *Serveur)
    {
        if (Serveur->objectName().isEmpty())
            Serveur->setObjectName(QString::fromUtf8("Serveur"));
        Serveur->resize(680, 445);
        Serveur->setStyleSheet(QString::fromUtf8("background-color: rgb(0,0,0);\n"
"color: rgb(255, 255, 255);"));
        centralwidget = new QWidget(Serveur);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 381, 392));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rapport = new QTextEdit(scrollAreaWidgetContents);
        rapport->setObjectName(QString::fromUtf8("rapport"));
        QFont font;
        font.setPointSize(10);
        rapport->setFont(font);
        rapport->setStyleSheet(QString::fromUtf8("background-color: rgb(40, 40, 40);\n"
"color: rgb(0, 170, 0);"));
        rapport->setFrameShape(QFrame::NoFrame);
        rapport->setReadOnly(true);

        horizontalLayout->addWidget(rapport);

        frame = new QFrame(scrollAreaWidgetContents);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(250, 0));
        frame->setMaximumSize(QSize(250, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        port = new QLineEdit(frame);
        port->setObjectName(QString::fromUtf8("port"));
        port->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));

        horizontalLayout_2->addWidget(port);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        connect = new QPushButton(frame);
        connect->setObjectName(QString::fromUtf8("connect"));
        connect->setMinimumSize(QSize(75, 24));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        connect->setFont(font1);
        connect->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_2->addWidget(connect);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        rap = new QPushButton(frame);
        rap->setObjectName(QString::fromUtf8("rap"));
        rap->setMinimumSize(QSize(136, 24));
        rap->setFont(font1);
        rap->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_3->addWidget(rap);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        dossier = new QTreeView(frame);
        dossier->setObjectName(QString::fromUtf8("dossier"));
        dossier->setFont(font1);
        dossier->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 150);"));
        dossier->setFrameShape(QFrame::NoFrame);
        dossier->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dossier->setAlternatingRowColors(true);
        dossier->setAnimated(true);
        dossier->setHeaderHidden(false);

        verticalLayout_2->addWidget(dossier);


        horizontalLayout->addWidget(frame);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        Serveur->setCentralWidget(centralwidget);
        etat = new QStatusBar(Serveur);
        etat->setObjectName(QString::fromUtf8("etat"));
        Serveur->setStatusBar(etat);
        sMenu = new QToolBar(Serveur);
        sMenu->setObjectName(QString::fromUtf8("sMenu"));
        sMenu->setMovable(false);
        Serveur->addToolBar(Qt::TopToolBarArea, sMenu);
        boiteRapport = new QDockWidget(Serveur);
        boiteRapport->setObjectName(QString::fromUtf8("boiteRapport"));
        boiteRapport->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        boiteRapport->setFloating(true);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        visual = new QTextBrowser(dockWidgetContents_2);
        visual->setObjectName(QString::fromUtf8("visual"));
        visual->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));

        verticalLayout_3->addWidget(visual);

        boiteRapport->setWidget(dockWidgetContents_2);
        Serveur->addDockWidget(static_cast<Qt::DockWidgetArea>(2), boiteRapport);

        retranslateUi(Serveur);

        QMetaObject::connectSlotsByName(Serveur);
    } // setupUi

    void retranslateUi(QMainWindow *Serveur)
    {
        Serveur->setWindowTitle(QApplication::translate("Serveur", "Activix", 0, QApplication::UnicodeUTF8));
        rapport->setHtml(QApplication::translate("Serveur", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        port->setText(QApplication::translate("Serveur", "50885", 0, QApplication::UnicodeUTF8));
        connect->setText(QApplication::translate("Serveur", "Ouvrir", 0, QApplication::UnicodeUTF8));
        rap->setText(QApplication::translate("Serveur", "Rapport jounalier", 0, QApplication::UnicodeUTF8));
        sMenu->setWindowTitle(QApplication::translate("Serveur", "toolBar", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Serveur);
    } // retranslateUi

};

namespace Ui {
    class Serveur: public Ui_Serveur {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVEUR_H
