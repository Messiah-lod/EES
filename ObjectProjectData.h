#pragma once

#include <QString>
#include <QDate> 
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <QtGui>


class ObjectProjectData//набор полей строки
{
public:
	ObjectProjectData();
	~ObjectProjectData();

	QVariant getMark() const;
	QVariant getName() const;
	QVariant getDescription() const;
	QVariant getObjectType() const;
	QVariant getDigital() const; //добавили для цифры
	QVariant getSignature() const;
	QVariant getController() const;
	QVariant getPlcAdress() const;//добавили для цифры
	QVariant getPlcVarname() const;//добавили для цифры
	QVariant getResource() const;
	QVariant getEventGroup() const;
	QVariant getKKS() const;
	QVariant getObjectTemplate() const;
	QVariant getMnemonicFrameName() const;
	QVariant getMnemonicFrameTemplate() const;
	QVariant getMnemonicFrameParent() const;
	QVariant getTechnicalProgramName() const;
	QVariant getTechnicalProgramParent() const;
	


	void setData(int numParam, QVariant value);

private:
	QVariant data[18];
};

