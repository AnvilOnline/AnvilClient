#include "DatumBase.hpp"

using namespace Blam::Data;

const DatumBase DatumBase::Null = { 0xFFFF };

DatumBase::DatumBase()
	: DatumBase(DatumBase::Null.Salt)
{
}

DatumBase::DatumBase(const uint16_t p_Salt)
	: Salt(p_Salt)
{
}

bool DatumBase::operator==(const DatumBase &p_Other) const
{
	return Salt == p_Other.Salt;
}

bool DatumBase::operator!=(const DatumBase &p_Other) const
{
	return !(*this == p_Other);
}

DatumBase::operator uint16_t() const
{
	return Salt;
}

DatumBase::operator bool() const
{
	return *this != Null;
}