/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

// Local
#include "firstrunwizard.h"

FirstRunWizard::FirstRunWizard(QWidget *parent) : QWizard(parent)
{
    this->setWindowTitle(tr("First Run Wizard"));
    this->setWindowIcon(QIcon::fromTheme("quickwizard",
                                         QIcon(QPixmap(":/images/icons/breeze/32x32/window-close.svg"))));
    this->setPage(Page_QEMUBinaries, new QEMUBinariesPage(this));
    this->setStartId(Page_QEMUBinaries);

#ifndef Q_OS_MAC
    this->setWizardStyle(NStyles);
#endif
#ifdef Q_OS_MAC
        this->setWizardStyle(MacStyle);
#endif

    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    this->setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));
    this->setMinimumSize(700, 400);

    m_warningFinishMessageBox = new QMessageBox();
    m_warningFinishMessageBox->setWindowTitle(tr("QEMU - configuration"));
    m_warningFinishMessageBox->setIcon(QMessageBox::Warning);
    m_warningFinishMessageBox->setText("<p>If you don't finish the Wizard QtEmu will not work correctly.</p>"
                                       "<p>You can make the configuration later from the QtEmu menu</p>");

    qDebug() << "FirstRunWizard created";
}

FirstRunWizard::~FirstRunWizard()
{
    qDebug() << "FirstRunWizard destroyed";
}

void FirstRunWizard::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.beginGroup("Configuration");
    settings.setValue("firstrunwizard", false);
    settings.endGroup();
    settings.sync();

    this->m_warningFinishMessageBox->exec();
    event->accept();
}

QEMUBinariesPage::QEMUBinariesPage(QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("QEMU options"));

    m_descriptionLabel = new QLabel(tr("Select QEMU binaries folder and qemu-img folder"), this);
    m_descriptionLabel->setWordWrap(true);

    m_qemuBinariesPathLabel = new QLabel(tr("QEMU binaries path") + ":", this);
    m_qemuBinariesPathLineEdit = new QLineEdit(this);
    m_qemuBinariesPushButton = new QPushButton(this);
    m_qemuBinariesPushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                       QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));

    m_qemuImgPathLabel = new QLabel(tr("QEMU img path") + ":", this);
    m_qemuImgPathLineEdit = new QLineEdit(this);
    m_qemuImgPathPushButton = new QPushButton(this);
    m_qemuImgPathPushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));

    m_vmsPathLabel = new QLabel(tr("QEMU machines path") + ":", this);
    m_qemuMachinesPathLineEdit = new QLineEdit(this);
    m_qemuMachinesPathPushButton = new QPushButton(this);
    m_qemuMachinesPathPushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));

    m_descriptionHostPortLabel = new QLabel(tr("Select the port to connect to QEMU monitor"), this);
    m_descriptionHostPortLabel->setWordWrap(true);

    m_monitorHostnameLabel = new QLabel(tr("Hostname") + ":", this);
    m_monitorHostnameLabel->setWordWrap(true);

    m_monitorHostnameComboBox = new QComboBox(this);
    m_monitorHostnameComboBox->addItem("localhost");
    m_monitorHostnameComboBox->addItem("127.0.0.1");

    m_monitorSocketPathLabel = new QLabel(tr("Port") + ":", this);
    m_monitorSocketSpinBox = new QSpinBox(this);
    m_monitorSocketSpinBox->setMinimum(1);
    m_monitorSocketSpinBox->setMaximum(65535);
    m_monitorSocketSpinBox->setValue(6000);

    this->registerField("qemuBinaries*", m_qemuBinariesPathLineEdit);
    this->registerField("qemuImg*", m_qemuImgPathLineEdit);

    m_mainLayout = new QGridLayout();
    m_mainLayout->setColumnStretch(1, 10);
    m_mainLayout->addWidget(m_descriptionLabel,           0, 0, 1, 4);
    m_mainLayout->addWidget(m_qemuBinariesPathLabel,      1, 0, 1, 1);
    m_mainLayout->addWidget(m_qemuBinariesPathLineEdit,   1, 1, 1, 2);
    m_mainLayout->addWidget(m_qemuBinariesPushButton,     1, 2, 1, 1);
    m_mainLayout->addWidget(m_qemuImgPathLabel,           2, 0, 1, 1);
    m_mainLayout->addWidget(m_qemuImgPathLineEdit,        2, 1, 1, 2);
    m_mainLayout->addWidget(m_qemuImgPathPushButton,      2, 2, 1, 1);
    m_mainLayout->addWidget(m_vmsPathLabel,               3, 0, 1, 1);
    m_mainLayout->addWidget(m_qemuMachinesPathLineEdit,   3, 1, 1, 2);
    m_mainLayout->addWidget(m_qemuMachinesPathPushButton, 3, 2, 1, 1);
    m_mainLayout->addWidget(m_descriptionHostPortLabel,   4, 0, 1, 4);
    m_mainLayout->addWidget(m_monitorHostnameLabel,       5, 0, 1, 1);
    m_mainLayout->addWidget(m_monitorHostnameComboBox,    5, 1, 1, 1);
    m_mainLayout->addWidget(m_monitorSocketPathLabel,     6, 0, 1, 1);
    m_mainLayout->addWidget(m_monitorSocketSpinBox,       6, 1, 1, 1);

    this->setLayout(m_mainLayout);

    qDebug() << "QEMUBinariesPage created";
}

QEMUBinariesPage::~QEMUBinariesPage()
{
    qDebug() << "FirstRunWizard destroyed";
}
