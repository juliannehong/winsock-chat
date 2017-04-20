#pragma once

#include "Win32.h"
#include "Types.h"
#include <vector>
using namespace std;

namespace PanelContainer
{
	struct CPanelContainerGlobals
	{
		HCURSOR arrow;
		HCURSOR hsize;
		HCURSOR vsize;
		HBRUSH ButtonFace;
		HBRUSH ButtonHilight;
		HBRUSH ButtonShadow;
		HBRUSH WindowFrame;
		/* Debug brushes */
		HBRUSH DbgButtonFace;
		HBRUSH DbgButtonHilight;
		HBRUSH DbgButtonShadow;
		HBRUSH DbgWindowFrame;
		HBRUSH HalftoneBrush;
		int cxVScroll;
		int cyHScroll;

		bool isdbg;

		CPanelContainerGlobals();
		~CPanelContainerGlobals();

		void SwapToDebug();
		void SwapToNormal();
	};

	enum ObjectType : U8
	{
		Object_None,
		Object_Panel,
		Object_VerticalSplit,
		Object_HorizontalSplit,
	};

	struct ObjectID
	{
	private:
		U32 Id;
	public:
		ObjectID() : Id(0)
		{
		}
		ObjectID(ObjectType Type, U32 Index = 0) : Id(((U8)(Type) << 24) | (Index & 0xFFFFFF))
		{
		}

		FORCEINLINE U8 GetObjectType()
		{
			return (U8)(((Id) >> 24) & 0xFF);
		}
		FORCEINLINE U32 GetObjectIndex()
		{
			return (U32)(((Id) & 0xFF'FFFF));
		}
		FORCEINLINE void SetObjectType(ObjectType t)
		{
			Id &= 0x00FF'FFFF;
			Id |= ((U8)t) << 24;

		}
		FORCEINLINE void SetObjectIndex(U32 Index)
		{
			Id &= 0xFF00'0000;
			Id |= (Index & 0x00FF'FFFF);
		}
		operator U32()
		{
			return Id;
		}
	};

	struct DrawingParams
	{
		U8 BorderX;
		U8 BorderY;
		U8 BorderSharedX;
		U8 BorderSharedY;
		U8 SplitterX;
		U8 SplitterY;
		U8 SplitterGapX;
		U8 SplitterGapY;
		DrawingParams() :
			BorderX(2),
			BorderY(2),
			BorderSharedX(0),
			BorderSharedY(0),
			SplitterX(7),
			SplitterY(7),
			SplitterGapX(7),
			SplitterGapY(7)
		{
		}
		~DrawingParams()
		{
		}
	};

	enum NodeFlags
	{
		NodeFlag_VerticalSeparator = 0x00,
		NodeFlag_HorizontalSeparator = 0x01,
		NodeFlag_LeftIsNode = 0x02,
		NodeFlag_RightIsNode = 0x04,
		NodeFlag_IsRightChild = 0x00,
		NodeFlag_IsLeftChild = 0x08,
	};

	const U32 InvalidNodeIndex = (-1);

	struct Node
	{
		U32 Flags;
		float RelativePosition;
		U32 Parent;
		U32 Left;
		U32 Right;

		Node() : Flags(0), RelativePosition(0), Left(InvalidNodeIndex), Right(InvalidNodeIndex)
		{
		}
		~Node()
		{
		}
	};

};

