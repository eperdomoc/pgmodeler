/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2014 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "modelnavigationwidget.h"

ModelNavigationWidget::ModelNavigationWidget(QWidget *parent): QWidget(parent)
{
	setupUi(this);

	connect(models_cmb, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentModel()));

	connect(close_tb, &QToolButton::clicked,
					[=](){ emit s_modelCloseRequested(models_cmb->currentIndex()); });

	connect(next_tb, &QToolButton::clicked,
					[=](){ models_cmb->setCurrentIndex(models_cmb->currentIndex()+1); });

	connect(previous_tb, &QToolButton::clicked,
					[=](){ models_cmb->setCurrentIndex(models_cmb->currentIndex()-1); });
}

int ModelNavigationWidget::getCurrentIndex(void)
{
	return(models_cmb->currentIndex());
}

QString ModelNavigationWidget::getText(int idx)
{
	if(idx < 0 || idx >= models_cmb->count())
		return("");
	else
		return(models_cmb->itemText(idx));
}

void ModelNavigationWidget::addModel(ModelWidget *model)
{
	if(model)
	{
		setEnabled(true);
		models_cmb->blockSignals(true);
		models_cmb->addItem(model->getDatabaseModel()->getName(), model->getFilename());
		models_cmb->setCurrentIndex(models_cmb->count()-1);

		if(model->getFilename().isEmpty())
			models_cmb->setToolTip(trUtf8("(model not saved yet)"));
		else
			models_cmb->setToolTip(model->getFilename());

		models_cmb->blockSignals(false);
		enableNavigationButtons();
	}
}

void ModelNavigationWidget::updateModelText(int idx, const QString &text, const QString &filename)
{
	if(idx >= 0 && idx < models_cmb->count())
	{
		models_cmb->setItemText(idx, QString("%1").arg(text));
		models_cmb->setItemData(idx, filename);

		if(idx==models_cmb->currentIndex())
			models_cmb->setToolTip(filename);
	}
}

void ModelNavigationWidget::removeModel(int idx)
{
	models_cmb->blockSignals(true);
	models_cmb->removeItem(idx);
	this->setEnabled(models_cmb->count() >= 1);

	if(models_cmb->count() >= 1)
		models_cmb->setToolTip(models_cmb->currentData().toString());
	models_cmb->blockSignals(false);

	enableNavigationButtons();
	emit s_modelRemoved(idx);
}

void ModelNavigationWidget::setCurrentModel(void)
{
	models_cmb->setToolTip(models_cmb->currentData().toString());

	enableNavigationButtons();

	if(models_cmb->currentIndex() >= 0)
		emit s_currentModelChanged(models_cmb->currentIndex());
}

void ModelNavigationWidget::enableNavigationButtons(void)
{
	previous_tb->setEnabled(models_cmb->currentIndex() > 0 && models_cmb->count() > 1);
	next_tb->setEnabled(models_cmb->currentIndex() >= 0 && models_cmb->currentIndex()!=(models_cmb->count()-1));
}