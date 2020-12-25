#pragma once

#include <QString>
#include <QDate> 
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <QtGui>


class ObjectProjectData//����� ����� ������
{
public:
	ObjectProjectData();
	~ObjectProjectData();

	QVariant getMark() const;
	QVariant getName() const;
	QVariant getDescription() const;
	QVariant getObjectType() const;
	QVariant getDigital() const; //�������� ��� �����
	QVariant getSignature() const;
	QVariant getController() const;
	QVariant getPlcAdress() const;//�������� ��� �����
	QVariant getPlcVarname() const;//�������� ��� �����
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

