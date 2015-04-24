/*
 *  FiniteStateMachineBuilder.cpp
 *  Untitled
 *
 *  Created by Rafael Radkowski on 23.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#include "FiniteStateMachineBuilder.h"

#define VERBOSE

using namespace mystatemachine;

FiniteStateMachineBuilder::FiniteStateMachineBuilder()
{
	
	
}


/********************************************************************************************************************
 Erstellt eine Zustandsmaschine auf Basis eines UML 2.0 Zustandsdiagramms. 
 @param umlfile - string mit dem Filenamen des UML 2.0 Files
 @param diagrammname - Name des UML 2.0 Diagramms, indem das Zustandsmodell enthalten ist
 @param destination - Poiner einer FiniteStateMachine, in der das Diagramm erstellt werden soll
 *********************************************************************************************************************/
bool FiniteStateMachineBuilder::build(std::string umlfile, std::string diagrammname, FiniteStateMachine* destination)
{
	_currentPackageID = "";
	_currentSubvertexID = "";
	_currentFunctionID = "";
	
	_currentLevel = LEVEL_0;
	
	if(destination == NULL) return false;
	
	if(umlfile.length() == 0) return false;
	
	if(diagrammname.length() == 0) return false;
	
	std::ifstream in(umlfile.c_str());

	
	std::string inStr;
	
	if(in.is_open())
	{
		while(!in.eof())
		{
			//in >> inStr;
			getline (in,  inStr );
			switch(_currentLevel)
			{
				case LEVEL_0:
					_currentLevel = (Level) findPackage(inStr, diagrammname);
					break;
				case LEVEL_1:
					_currentLevel =  (Level)getElement(inStr);
					break;
				case LEVEL_2:
					_currentLevel =  (Level)getSubElement(inStr);
					break;
				case LEVEL_3:
					_currentLevel =  (Level)getSubSubElement(inStr);
					break;
	

			}
			
		}
	}
	
	
	createAutomat(destination);
	
	return true;
	
}



int FiniteStateMachineBuilder::findPackage(std::string inStr, std::string diagrammName)
{
	
	// LEVEL_0
	
	int index = inStr.find("packagedElement");
	
	if(index == -1) return 0;
	
	int index2 = inStr.find("uml:StateMachine");
	if(index2 != -1)
	{
	
	
		int indexName = inStr.find(diagrammName);
	
		if(indexName == -1) return 0;

		// Wenn das Programm hier angekommen ist, dann ist das richtige Packet gefunden;
	
		getPackage( inStr);
	
		std::cout << inStr << std::endl;

		bool ret =  isEndOfElement(inStr);;
	
		if(ret == true)return LEVEL_0;
		return LEVEL_1;
	}
	
	index2 = inStr.find("uml:ChangeEvent");
	if(index2 != -1)
	{
		getPackage( inStr);
		
		bool ret =  isEndOfElement(inStr);;
		
		if(ret == true)return LEVEL_0;
		return LEVEL_1;
	}
	
	
	index2 = inStr.find("uml:AnyReceiveEvent");
	if(index2 != -1)
	{
		bool ret = getAnyReceiveEvent( inStr);
		
		if(ret == true)
		{
			_currentPackageID = "";
			return LEVEL_0;
		}
		return LEVEL_1;
	}
	
	return LEVEL_0;
	
}


bool FiniteStateMachineBuilder::getChangeExpression(std::string inStr)
{
	ChangeExpression e;
	
	e.xmi_type = getAttribute(inStr, "xmi:type");
	e.xmi_id = getAttribute(inStr, "xmi:id");
	e.name  = getAttribute(inStr, "name");

	e.package = this->_currentPackageID;
	
	_expressions.push_front(e);
#ifdef VERBOSE	
	std::cout << "ChangeExpression: " << e.xmi_id << std::endl;
#endif
	return  isEndOfElement(inStr);
}


int FiniteStateMachineBuilder::getElement(std::string inStr)
{
	
	//LEVEL_1
	
	
	int index = inStr.find("<subvertex");
	if(index != -1)
	{
		bool ret = getSubvertex(inStr);
		if(ret==true)
		{
			_currentSubvertexID ="";
			return LEVEL_1;
		}
		else
		{ 
			return LEVEL_2; // Wenn das Element noch nicht zu Ende ist, dann gehte ein Level tiefer
		}
	}
	
	index = inStr.find("<transition");
	if(index != -1)
	{
		bool ret = getTransition(inStr);
		if(ret==true)return LEVEL_1;
		else return LEVEL_2;
	}
	
	index = inStr.find("<changeExpression");
	if(index != -1)
	{
		bool ret = getChangeExpression(inStr);
		if(ret==true)return LEVEL_1;
		else return LEVEL_2;
	}
	
	index = inStr.find("</changeExpression>");
	if(index != -1)
	{
		return LEVEL_1;
	}
	
	index = inStr.find("</packagedElement>");
	if(index != -1)
	{
		_currentPackageID = "";
		return LEVEL_0;
	}
	
/*	
	index = inStr.find("</subvertex>");
	if(index != -1)
	{
		_currentSubvertexID = "";
		return LEVEL_1;
	}
*/
	return 1;
	
}

bool FiniteStateMachineBuilder::getPackage(std::string inStr)
{
	Package p;
	
	p.xmi_type = getAttribute(inStr, "xmi:type");
	p.xmi_id = getAttribute(inStr, "xmi:id");
	p.name = getAttribute(inStr, "name");
	
	this->_packages.push_back(p);
	
	_currentPackageID = p.xmi_id;
	
	return isEndOfElement(inStr);
}

bool FiniteStateMachineBuilder::getAnyReceiveEvent(std::string inStr)
{
	AnyChangeExpression p;
	
	p.xmi_type = getAttribute(inStr, "xmi:type");
	p.xmi_id = getAttribute(inStr, "xmi:id");
	p.name = getAttribute(inStr, "name");
	
	this->_anyExpressions.push_back(p);
	
	
	return isEndOfElement(inStr);
}

bool FiniteStateMachineBuilder::getSubvertex(std::string inStr)
{
	
	Subvertex v;
	
	v.xmi_type = getAttribute(inStr, "xmi:type");
	v.xmi_id = getAttribute(inStr, "xmi:id");
	v.name = getAttribute(inStr, "name");
	v.visibility = getAttribute(inStr, "visibility");
	v.outgoing = getAttribute(inStr, "outgoing");
	v.incoming = getAttribute(inStr, "incoming");
	
	_currentSubvertexID = v.xmi_id ;
	
	this->_states.push_back(v);
#ifdef VERBOSE	
	std::cout << "Subvertex: " << v.xmi_id << ", Name : "<< v.name << std::endl;
#endif	
	return isEndOfElement(inStr);
		
}

bool FiniteStateMachineBuilder::getTransition(std::string inStr)
{
	Transitions t;
	
	t.xmi_id = getAttribute(inStr, "xmi:id");
	t.name = getAttribute(inStr, "name");
	t.target = getAttribute(inStr, "target");
	t.source = getAttribute(inStr, "source");
	
	this->_transitions.push_front(t);
#ifdef VERBOSE	
	std::cout << "Transition: " << t.xmi_id << ", Name : "<< t.name<< std::endl;
#endif	
	return  isEndOfElement(inStr);
}

int FiniteStateMachineBuilder::getSubElement(std::string inStr) // Level 2
{
	int index = inStr.find("<trigger");
	if(index != -1)
	{
		bool ret = getTrigger(inStr);
		if(ret==true)return LEVEL_1;// Zurück zu Level 1
		else return LEVEL_2;
	}
	
	index = inStr.find("<body>");
	if(index != -1)
	{
		int i1 = inStr.find_first_of(">");
		int i2 = inStr.find_first_of("<", i1+1);
		std::string ret = inStr.substr(i1+1, i2-i1-1);
		
		(*this->_expressions.begin()).body = ret;
#ifdef VERBOSE		
		std::cout << "Body: " << ret << std::endl;
#endif		
		return LEVEL_1;
	}
	
	index = inStr.find("<entry");
	if(index != -1)
	{
		bool ret = getEntry(inStr);
		if(ret==true)
		{
			_currentFunctionID ="";
			return LEVEL_2;
		}
		else return LEVEL_3;
	}
	
	index = inStr.find("<exit");
	if(index != -1)
	{
		bool ret = getExit(inStr);
		if(ret==true)
		{
			_currentFunctionID ="";
			return LEVEL_2;
		}
		else return LEVEL_3;
	}
	
	
	index = inStr.find("<ownedComment");
	if(index != -1)
	{
		bool ret = getOwnedComment(inStr);
		if(ret==true)return LEVEL_2;
		//	else return LEVEL_2;
	}
	
	index = inStr.find("<doActivity");
	if(index != -1)
	{
		bool ret = getDoActivity(inStr);
		if(ret==true)
		{
			_currentFunctionID ="";
			return LEVEL_2;
		}
		else return LEVEL_3;
	}
	
	
	index = inStr.find("</subvertex>");
	if(index != -1)
	{
		_currentSubvertexID = "";
		return LEVEL_1;
	}
	

	return 2;
}

bool FiniteStateMachineBuilder::getDoActivity(std::string inStr)
{
	DoFunction d;
	
	d.xmi_type = getAttribute(inStr, "xmi:type");
	d.xmi_id =  getAttribute(inStr, "xmi:id");
	d.name =  getAttribute(inStr, "name");
	d.toSubvertex = _currentSubvertexID;
	
	_currentFunctionID = d.xmi_id;
	
#ifdef VERBOSE		
	std::cout << "DoFunction: " << d.name << std::endl;
#endif		
	_doFunctions.push_back(d);
	
	return  isEndOfElement(inStr);
}


bool FiniteStateMachineBuilder::getOwnedComment(std::string inStr)
{
	OwnedComment c;
	
	c.annotatedElement = getAttribute(inStr, "annotatedElement");
	c.xmi_id = getAttribute(inStr, "xmi:id");
	c.body = getAttribute(inStr, "body");
		
	_owndedComments.push_back(c);

	return  isEndOfElement(inStr);
}

bool FiniteStateMachineBuilder::getEntry(std::string inStr)
{
	EntryFunction f;
	
	f.xmi_type = getAttribute(inStr, "xmi:type");
	f.xmi_id =  getAttribute(inStr, "xmi:id");
	f.name =  getAttribute(inStr, "name");
	f.toSubvertex = _currentSubvertexID;
	
	_currentFunctionID = f.xmi_id;
	
#ifdef VERBOSE		
	std::cout << "EntryFunction: " << f.name << std::endl;
#endif		
	_entryfunctions.push_back(f);
	
	return  isEndOfElement(inStr);
}

bool FiniteStateMachineBuilder::getExit(std::string inStr)
{
	ExitFunction f;
	
	f.xmi_type = getAttribute(inStr, "xmi:type");
	f.xmi_id =  getAttribute(inStr, "xmi:id");
	f.name =  getAttribute(inStr, "name");
	f.toSubvertex = _currentSubvertexID;
	
	_currentFunctionID = f.xmi_id;
	
#ifdef VERBOSE		
	std::cout << "ExitFunction: " << f.name << std::endl;
#endif		
	_exitfunctions.push_back(f);
	
	return  isEndOfElement(inStr);
}

bool FiniteStateMachineBuilder::getTrigger(std::string inStr)
{
	Trigger t;
	
	t.xmi_id = getAttribute(inStr, "xmi:id");
	t.name = getAttribute(inStr, "name");
	t.event = getAttribute(inStr, "event");
	
//	this->_triggers.push_back(t);
	(*this->_transitions.begin()).trigger = t;
#ifdef VERBOSE	
	std::cout << "Trigger: " << t.xmi_id << std::endl;
#endif	
	return  isEndOfElement(inStr);
}

std::string FiniteStateMachineBuilder::getAttribute(std::string inStr, std::string attribute)
{
	int index = -1;
	
	index = inStr.find(attribute);
	
	if(index != -1)
	{
		std::string sub = inStr.substr(index, inStr.length());
		int i1, i2;
		
		i1 = sub.find_first_of("\"");
		i2 = sub.find_first_of("\"", i1+1);
		
		std::string ret = sub.substr(i1+1, i2-i1-1);
		return ret;
		
	}
	return "";
	
}

// Gibt zurück, ob ein Element beendet ist oder nicht
bool FiniteStateMachineBuilder::isEndOfElement(std::string inStr)
{
	int index = -1;
	
	index = inStr.find("/");
	
	if(index != -1)
	{
		return true;
	}
	return false;
}


int FiniteStateMachineBuilder::getSubSubElement(std::string inStr)
{ // LEVEL_3
	int index = -1;
	
	
	index = inStr.find("<ownedAttribute");
	if(index != -1)
	{
		bool ret = getOwnedAttribute(inStr);
		if(ret==true)return LEVEL_3;// Er beleibt auf dem Level.

	}
	
	
	index = inStr.find("<defaultValue");
	if(index != -1)
	{
		bool ret = getDefaultValue(inStr);
		if(ret==true)return LEVEL_3;// Er beleibt auf dem Level. 
		
	}
	
	
	
	
	////////////////////////////////////////////////////////////////
	// Ab hier geht es wieder einen Level nach oben
	
	index = inStr.find("</exit>");
	if(index != -1)
	{
		_currentFunctionID = "";
		return LEVEL_2;
	}
	
	index = inStr.find("</entry>");
	if(index != -1)
	{
		_currentFunctionID = "";
		return LEVEL_2;
	}
	
	index = inStr.find("</doActivity>");
	if(index != -1)
	{
		_currentFunctionID = "";
		return LEVEL_2;
	}
	
	return 3;

}

bool FiniteStateMachineBuilder::getOwnedAttribute(std::string inStr)
{
	OwnedAttribute att;
	
	att.xmi_id = getAttribute(inStr, "xmi:id");
	att.name = getAttribute(inStr, "name");
	att.visibility = getAttribute(inStr, "visibility");
	
	
	att.ownerFunction = _currentFunctionID;
	
	//	this->_triggers.push_back(t);
	this->_attributes.push_front(att);
	
#ifdef VERBOSE	
	std::cout << "Attribute: " << att.xmi_id << " , Name: " << att.name ;
#endif	
	return  isEndOfElement(inStr);

}

bool FiniteStateMachineBuilder::getDefaultValue(std::string inStr)
{
	std::string value =  getAttribute(inStr, "value");
	(*this->_attributes.begin()).value = value;
	(*this->_attributes.begin()).xmi_type = "String";
	
#ifdef VERBOSE	
	std::cout << " , Value: " << value << std::endl;
#endif	
	
	return  isEndOfElement(inStr);
}


bool FiniteStateMachineBuilder::createAutomat(FiniteStateMachine* destination)
{
	// Zustände erstellen
	if(destination == NULL) return false;
	
	_destination = destination;
	
	std::list<Subvertex>::iterator sItr = _states.begin();
	
	while(sItr != this->_states.end())
	{
		if( (*sItr).xmi_type.compare("uml:State") == 0)
		{
			std::string stateName = (*sItr).name;
			
			destination->addState(stateName);
			
			// Kommentar, 
			std::string comment =  getComment((*sItr).xmi_id);
			
			
			// Entry Funktion
			std::string entryfunctionname = getEntryFunction((*sItr).xmi_id);
			if(entryfunctionname.length() != 0)
			{
				// Attribute der Entry-Function werden gesammelt
				StateFunctionAttributeList	entry_attributes = getAttributesOfFunction(getEntryFunctionID((*sItr).xmi_id));
				setFunction(stateName,  entryfunctionname, comment, entry_attributes, ON_ENTRY);
			}
			
			// Exit Funktion
			std::string exitfunctionname = getExitFunction((*sItr).xmi_id);
			if(exitfunctionname.length() != 0)
			{
				// Attribute der Exit-Function werden gesammelt
				StateFunctionAttributeList	exit_attributes = getAttributesOfFunction(getExitFunctionID((*sItr).xmi_id));
				setFunction(stateName,  exitfunctionname, comment, exit_attributes,  ON_EXIT);
			}
			
			// Do Function
			std::string dofunctionname = getDoFunction((*sItr).xmi_id);
			if(dofunctionname.length() != 0)
			{
				// Attribute der Do-Function werden gesammelt
				StateFunctionAttributeList	do_attributes = getAttributesOfFunction(getDoFunctionID((*sItr).xmi_id));
				setFunction(stateName,  dofunctionname, comment, do_attributes,  STATE_OPERATION);
			}
		}
		
		sItr++;
	}
	
	
	// Transition erstellen
	std::list<Transitions>::iterator tItr =	_transitions.begin();
	
	while(tItr != _transitions.end())
	{
		std::string target = getState((*tItr).target);
		std::string source = getState((*tItr).source);
		std::string targetType = getStateType((*tItr).target);
		std::string sourceType = getStateType((*tItr).source);
	
		std::string eventStr = (*tItr).trigger.event;
		std::string ops =getTransitionOperation(eventStr);
		
	
		
		if(sourceType.compare("uml:Pseudostate") == 0)// Das ist der Startbutton
		{
			destination->setStartState(target);
		}
		else
		{
			if(ops.compare("uml:ANY_OPERATION") != 0)
			{
				destination->addTransition(source, target, ops);
			}
			else
			{
				// ToDo: Hier muss die Any-Operation rein
				destination->addTransition(source, target, "ANY", Value_Any );
			}
			
			
		}
		
		
		tItr++;
	}
	
	_destination = NULL;
	
	return true;
	
}

std::string FiniteStateMachineBuilder::getEntryFunction(std::string stateID)
{
	std::string ret = "";
	
	std::list<EntryFunction>::iterator itr = _entryfunctions.begin();
	
	while(itr != _entryfunctions.end())
	{
		std::string subvertexID = (*itr).toSubvertex;
		
		if(subvertexID.compare(stateID) == 0)
		{
			ret = (*itr).name;

#ifdef VERBOSE
			std::cout << "FiniteStateMachineBuilder: EntryFunction " << (*itr).name << " zu Zustand" << getState(stateID) << " hinzugefügt" << std::endl;
#endif
			return ret;
		}
		
		
		itr++;
	}
	return ret;
	
}

std::string FiniteStateMachineBuilder::getEntryFunctionID(std::string stateID)
{
	std::string ret = "";
	
	std::list<EntryFunction>::iterator itr = _entryfunctions.begin();
	
	while(itr != _entryfunctions.end())
	{
		std::string subvertexID = (*itr).toSubvertex;
		
		if(subvertexID.compare(stateID) == 0)
		{
			ret = (*itr).xmi_id;
			
#ifdef VERBOSE
			std::cout << "FiniteStateMachineBuilder: EntryFunctionID " << ret << " zu Zustand" << getState(stateID) << " geladen" << std::endl;
#endif
			return ret;
		}
		
		
		itr++;
	}
	return ret;
	
}



std::string FiniteStateMachineBuilder::getExitFunction(std::string stateID)
{
	std::string ret = "";
	
	std::list<ExitFunction>::iterator itr = _exitfunctions.begin();
	
	while(itr != _exitfunctions.end())
	{
		std::string subvertexID = (*itr).toSubvertex;
		
		if(subvertexID.compare(stateID) == 0)
		{
			ret = (*itr).name;
			
#ifdef VERBOSE
			std::cout << "FiniteStateMachineBuilder: ExitFunction " << (*itr).name << " zu Zustand" << getState(stateID) << " hinzugefügt" << std::endl;
#endif
			return ret;
		}
		
		
		itr++;
	}
	return ret;
	
}

std::string FiniteStateMachineBuilder::getExitFunctionID(std::string stateID)
{
	std::string ret = "";
	
	std::list<ExitFunction>::iterator itr = _exitfunctions.begin();
	
	while(itr != _exitfunctions.end())
	{
		std::string subvertexID = (*itr).toSubvertex;
		
		if(subvertexID.compare(stateID) == 0)
		{
			ret = (*itr).xmi_id;
			
#ifdef VERBOSE
			std::cout << "FiniteStateMachineBuilder: ExitFunctionID " << ret << " zu Zustand" << getState(stateID) << " gelesen" << std::endl;
#endif
			return ret;
		}
		
		
		itr++;
	}
	return ret;
	
}


/*
 Gibt die Do Function zu einem Zustand State zurück. 
 Ist keine vorhanden kommt eine Leerer String zurück
 */
std::string FiniteStateMachineBuilder::getDoFunction(std::string stateID)
{
	std::string ret = "";
	
	std::list<DoFunction>::iterator itr = _doFunctions.begin();
	
	while(itr != _doFunctions.end())
	{
		std::string subvertexID = (*itr).toSubvertex;
		
		if(subvertexID.compare(stateID) == 0)
		{
			ret = (*itr).name;
			
#ifdef VERBOSE
			std::cout << "FiniteStateMachineBuilder: DoFunction " << (*itr).name << " zu Zustand" << getState(stateID) << " hinzugefügt" << std::endl;
#endif
			return ret;
		}
		
		
		itr++;
	}
	return ret;
	
}

/*
 Gibt die ID zur Do Function zu einem Zustand State zurück. 
 Ist keine vorhanden kommt eine Leerer String zurück
 */
std::string FiniteStateMachineBuilder::getDoFunctionID(std::string stateID)
{
	std::string ret = "";
	
	std::list<DoFunction>::iterator itr = _doFunctions.begin();
	
	while(itr != _doFunctions.end())
	{
		std::string subvertexID = (*itr).toSubvertex;
		
		if(subvertexID.compare(stateID) == 0)
		{
			ret = (*itr).xmi_id;
			
#ifdef VERBOSE
			std::cout << "FiniteStateMachineBuilder: DoFunctionID " << ret << " zu Zustand" << getState(stateID) << " geladen" << std::endl;
#endif
			return ret;
		}
		
		
		itr++;
	}
	return ret;
	
}


std::string FiniteStateMachineBuilder::getState(std::string id)
{
	
	std::list<Subvertex>::iterator sItr = _states.begin();
	
	while(sItr != this->_states.end())
	{
		if( (*sItr).xmi_id.compare(id) == 0)
		{
			return (*sItr).name;
		}
		
		sItr++;
	}
		
	return "";
}

std::string FiniteStateMachineBuilder::getStateType(std::string id)
{
	
	std::list<Subvertex>::iterator sItr = _states.begin();
	
	while(sItr != this->_states.end())
	{
		if( (*sItr).xmi_id.compare(id) == 0)
		{
			return (*sItr).xmi_type;
		}
		
		sItr++;
	}
	
	return "";
}


std::string FiniteStateMachineBuilder::getTransitionOperation(std::string id)
{
	std::list<ChangeExpression>::iterator tItr =_expressions.begin();
	
	std::string result = "";
	
	while(tItr != _expressions.end())
	{
		
		std::string xmi_id = (*tItr).package;
		
		if(xmi_id.compare(id) == 0)
		{
			result = (*tItr).body;
			return result;
		}

		tItr++;
	}
	
	if(result.compare("") == 0)
	{
		std::list<AnyChangeExpression>::iterator tItr =_anyExpressions.begin();
		
		while(tItr != _anyExpressions.end())
		{
			
			std::string xmi_id = (*tItr).xmi_id;
			
			if(xmi_id.compare(id) == 0)
			{
				return "uml:ANY_OPERATION";
			}
			
			tItr++;
		}
		
	
	}
	
	return result;
	
}



/*
 Gibt den Kommentar zu einem Zustand zurück.
 Ein leeren String, falls kein Kommentar vorhanden ist. 
 */
std::string FiniteStateMachineBuilder::getComment(std::string stateID)
{
	std::list<OwnedComment>::iterator itr = _owndedComments.begin();
	
	while(itr != _owndedComments.end())
	{
		
		std::string sid = (*itr).annotatedElement;
		
		if(sid.compare(stateID) == 0)
		{
			return (*itr).body;
		}
		
		itr++;
	}
	
	return "";
	
}

/*
 Gibt eine Liste mit allen Attributen einer Funktion zurück
 */
StateFunctionAttributeList FiniteStateMachineBuilder::getAttributesOfFunction(std::string functionID)
{
	StateFunctionAttributeList temp;
	
	int size = _attributes.size();
	std::list<OwnedAttribute>::iterator itr = _attributes.begin();
	
	while(itr != _attributes.end())
	{
		
		std::string sid = (*itr).ownerFunction;
		
		if(sid.compare(functionID) == 0)
		{
			StateFunctionAttribute s;
			s.name = (*itr).name;
			s.visibility = (*itr).visibility;
			s.xmi_type = (*itr).xmi_type;
			s.value = (*itr).value;
			
			temp.push_front(s);
		}
		
		itr++;
	}
	
	
	return temp;
	
	
}


void FiniteStateMachineBuilder::setFunction(std::string stateName, std::string functionname, std::string parameter1, StateFunctionAttributeList attribute_list, StateOperationType type)
{
	
}
