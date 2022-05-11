#include "ObjectProjectData.h"

ObjectProjectData::ObjectProjectData()
{
}

ObjectProjectData::~ObjectProjectData()
{
}

QVariant ObjectProjectData::getMark() const
{
	return data[0];
}

QVariant ObjectProjectData::getName() const
{
	return data[1];
}

QVariant ObjectProjectData::getDescription() const
{
	return data[2];
}

QVariant ObjectProjectData::getObjectType() const
{
	return data[3];
}

QVariant ObjectProjectData::getDigital() const//добавили для цифры
{
	return data[4];
}

QVariant ObjectProjectData::getSignature() const
{
	return data[5];
}

QVariant ObjectProjectData::getController() const
{
	return data[6];
}

QVariant ObjectProjectData::getPlcAdress() const//добавили для цифры
{
	return data[7];
}

QVariant ObjectProjectData::getPlcVarname() const//добавили для цифры
{
	return data[8];
}

QVariant ObjectProjectData::getResource() const
{
	return data[9];
}

QVariant ObjectProjectData::getEventGroup() const
{
	return data[10];
}

QVariant ObjectProjectData::getKKS() const
{
	return data[11];
}

QVariant ObjectProjectData::getObjectTemplate() const
{
	return data[12];
}

QVariant ObjectProjectData::getMnemonicFrameName() const
{
	return data[13];
}

QVariant ObjectProjectData::getMnemonicFrameTemplate() const
{
	return data[14];
}

QVariant ObjectProjectData::getMnemonicFrameParent() const
{
	return data[15];
}

QVariant ObjectProjectData::getTechnicalProgramName() const
{
	return data[16];
}

QVariant ObjectProjectData::getTechnicalProgramParent() const
{
	return data[17];
}



void ObjectProjectData::setData(int numParam, QVariant value)
{
		switch (numParam) {
	case 0:
		data[0] = value; break;
	case 1:
		data[1] = value; break;
	case 2:
		data[2] = value; break;
	case 3:
		data[3] = value; break;
	case 4:
		data[4] = value; break;
	case 5:
		data[5] = value; break;
	case 6:
		data[6] = value; break;
	case 7:
		data[7] = value; break;
	case 8:
		data[8] = value; break;
	case 9:
		data[9] = value; break;
	case 10:
		data[10] = value; break;
	case 11:
		data[11] = value; break;
	case 12:
		data[12] = value; break;
	case 13:
		data[13] = value; break;
	case 14:
		data[14] = value; break;
	case 15:
		data[15] = value; break;
	case 16:
		data[16] = value; break;
	case 17:
		data[17] = value; break;

//	default:
		
	}
}

