/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#pragma once
#include "Common.h"

BEGIN_NAMESPACE
class Node
{
private:
	int m_Index;

public:
	Node(const int Index);

	~Node(void);

	const int GetIndex() const;

	void Block();

	void UnBlock();

	const bool Blocked() const;
};


END_NAMESPACE