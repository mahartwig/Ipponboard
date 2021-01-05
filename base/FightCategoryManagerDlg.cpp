// Copyright 2018 Florian Muecke. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.txt file.

#include "FightCategoryManagerDlg.h"
#include "ui_FightCategoryManagerDlg.h"
#include "..\util\path_helpers.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QPlainTextEdit>

using namespace Ipponboard;

//---------------------------------------------------------
FightCategoryManagerDlg::FightCategoryManagerDlg(
	Ipponboard::FightCategoryMgr::Ptr pMgr,
	QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::FightCategoryManagerDlg)
	, m_pClassMgr(pMgr)
//---------------------------------------------------------
{
	ui->setupUi(this);

	// NOTE: This is nasty workaround for the standard buttons not
	// beeing translated (separate translator would be required)
	//ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
	//ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

	Q_ASSERT(m_pClassMgr);

	load_values();
}

//---------------------------------------------------------
FightCategoryManagerDlg::~FightCategoryManagerDlg()
//---------------------------------------------------------
{
	delete ui;
}

//---------------------------------------------------------
void FightCategoryManagerDlg::changeEvent(QEvent* e)
//---------------------------------------------------------
{
	QDialog::changeEvent(e);

	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;

	default:
		break;
	}
}

//---------------------------------------------------------
void FightCategoryManagerDlg::on_pushButton_add_pressed()
//---------------------------------------------------------
{
	bool ok(false);
	QString name = QInputDialog::getText(this,
										 tr("Add new category"),
										 tr("Enter the name of the new category"),
										 QLineEdit::Normal,
										 QString(),
										 &ok);

	while (ok && m_pClassMgr->HasCategory(name))
	{
		QMessageBox::critical(this,
							  tr(""),
							  tr("This category already exists. Please choose a different name."));

		name = QInputDialog::getText(this,
									 tr("Add new category"),
									 tr("Enter the name of the new category"),
									 QLineEdit::Normal,
									 name,
									 &ok);
	}

	if (ok)
	{
		m_pClassMgr->AddCategory(name);
		isModifiedData = true;

		// update combobox
		QStringList contents;
		contents.append(name);
		contents.append("");
		contents.append("");
		contents.append("");
		QTreeWidgetItem* pItem =
			new QTreeWidgetItem(contents, QTreeWidgetItem::UserType);
		pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		ui->treeWidget_classes->addTopLevelItem(pItem);
		pItem->setText(eColumn_Time, "...");
		pItem->setText(eColumn_GS, "...");
		pItem->setText(eColumn_Weights, "...");
	}
}

//---------------------------------------------------------
void FightCategoryManagerDlg::load_values()
//---------------------------------------------------------
{
	for (int i(0); i < m_pClassMgr->CategoryCount(); ++i)
	{
		Ipponboard::FightCategory classItem;
		m_pClassMgr->GetCategory(i, classItem);

		QStringList contents;
		contents.append(classItem.ToString());
		contents.append(classItem.GetRoundTimeStr());
		contents.append(classItem.GetGoldenScoreTimeStr());
		contents.append(classItem.GetWeights());

		QTreeWidgetItem* pItem =
			new QTreeWidgetItem(contents, QTreeWidgetItem::UserType);
		pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		ui->treeWidget_classes->addTopLevelItem(pItem);
	}
}

//---------------------------------------------------------
void FightCategoryManagerDlg::on_pushButton_remove_pressed()
//---------------------------------------------------------
{
	QTreeWidgetItem* pItem = ui->treeWidget_classes->currentItem();

	if (pItem)
	{
		ui->treeWidget_classes->takeTopLevelItem(
			ui->treeWidget_classes->indexOfTopLevelItem(pItem));

		m_pClassMgr->RemoveCategory(pItem->text(eColumn_Name));
		isModifiedData = true;

		delete pItem;
	}
}

//---------------------------------------------------------
void FightCategoryManagerDlg::on_treeWidget_classes_itemChanged(
	QTreeWidgetItem* pItem, int column)
//---------------------------------------------------------
{
	bool matches(false);

	FightCategory cat(pItem->text(eColumn_Name));
	cat.SetRoundTime(pItem->text(eColumn_Time));
	cat.SetGoldenScoreTime(pItem->text(eColumn_GS));
	cat.SetWeights(pItem->text(eColumn_Weights));

	if (eColumn_Name == column)
	{
		// get original data
		FightCategory old;

		for (int i(0); i < ui->treeWidget_classes->topLevelItemCount(); ++i)
		{
			const QTreeWidgetItem* pCheckItem =
				ui->treeWidget_classes->topLevelItem(i);

			if (pCheckItem == pItem)
			{
				m_pClassMgr->GetCategory(i, old);

				// Resetting the name to the old one (below) will trigger
				// itemChanged!
				if (old.ToString() == cat.ToString())
					return;

				break;
			}
		}

		// check if we do have more than one class with that name
		for (int i(0); i < ui->treeWidget_classes->topLevelItemCount(); ++i)
		{
			const QTreeWidgetItem* pCheckItem =
				ui->treeWidget_classes->topLevelItem(i);

			if (pCheckItem != pItem &&
					pCheckItem->text(eColumn_Name) == pItem->text(eColumn_Name))
			{
				QMessageBox::critical(
					this,
					QCoreApplication::applicationName(),
					tr("This name is already taken!"));

				// set previous text
				pItem->setText(eColumn_Name, old.ToString());

				return;
			}
		}

		m_pClassMgr->RenameCategory(old.ToString(), cat.ToString());
		isModifiedData = true;
		return;
	}
	else if (eColumn_Time == column)
	{
		QRegExp regex("[1-6]{0,1}[0-9][:][0-5][0-9]");
		matches = regex.exactMatch(pItem->text(column));
	}
	else if (eColumn_GS == column)
	{
		QRegExp regex("[1-6]{0,1}[0-9][:][0-5][0-9]");
		matches = regex.exactMatch(pItem->text(column));
	}
	else if (column == eColumn_Weights)
	{
		QRegExp regex("([-+]{0,1}[0-9]{1,3}[;])*[-+]{0,1}[0-9]{1,3}");
		matches = regex.exactMatch(pItem->text(eColumn_Weights));
	}

	QBrush brush(pItem->foreground(column));

	if (matches)
	{
		brush.setColor(Qt::black);
		m_pClassMgr->UpdateCategory(cat);
		isModifiedData = true;
	}
	else
	{
		brush.setColor(Qt::red);
	}

	pItem->setForeground(column, brush);
}

void FightCategoryManagerDlg::close_dialog()
{
	if (isModifiedData) QDialog::accept();
	else QDialog::reject();
}

void FightCategoryManagerDlg::reject()
{
	close_dialog();
}

void FightCategoryManagerDlg::accept()
{
	close_dialog();
}
