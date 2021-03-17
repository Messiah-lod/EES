#include "TableLinkData.h"


TableLinkData::TableLinkData()
{
}

TableLinkData::~TableLinkData()
{
}

QVariant TableLinkData::getSourceControl() const
{
	return data[0];
}

QVariant TableLinkData::getSourceRes() const
{
	return data[1];
}

QVariant TableLinkData::getSourceMark() const
{
	return data[2];
}

QVariant TableLinkData::getSourceChanel() const
{
	return data[3];
}

QVariant TableLinkData::getReceiverControl() const
{
	return data[4];
}

QVariant TableLinkData::getReceiverRes() const
{
	return data[5];
}

QVariant TableLinkData::getReceiverMark() const
{
	return data[6];
}

QVariant TableLinkData::getReceiverChanel() const
{
	return data[7];
}

void TableLinkData::setData(int numParam, QVariant value)
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
	}
}