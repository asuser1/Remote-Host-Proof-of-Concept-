#pragma once

using namespace System;
using namespace System::Runtime::Serialization;

[Serializable]
public ref class MouseMove : ISerializable
{
public:
	MouseMove(int nx, int ny)
	{
		x = nx;
		y = ny;
	}

	MouseMove(SerializationInfo^ info, StreamingContext ctxt)
	{
		x = (int)info->GetValue("PositionX", int::typeid);
		y = (int)info->GetValue("PositionY", int::typeid);
	}

	virtual void GetObjectData(SerializationInfo^ info, StreamingContext context);

	int GetX() { return x;  }
	int GetY() { return y;  }

	void SetXY(int nx, int ny) { x = nx; y = ny; }

protected:
private:
	//Mouse positions x and y
	int x;
	int y;
};