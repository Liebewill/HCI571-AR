/*
 *  StateTypes.h
 *  Untitled
 *
 *  Created by Rafael Radkowski on 22.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#pragma once

#include <string>
#include <list>

namespace mystatemachine
{

typedef enum{
	Value_String, 
	Value_Equal,
    Value_Any
}StateType;


typedef enum 
{
	ON_ENTRY,
	ON_EXIT,
	STATE_OPERATION
}StateOperationType;


typedef struct {
	std::string name;
	std::string xmi_id;
	std::string visibility;
	std::string xmi_type;
	std::string	value;
}StateFunctionAttribute;

typedef std::list<StateFunctionAttribute> StateFunctionAttributeList;

}