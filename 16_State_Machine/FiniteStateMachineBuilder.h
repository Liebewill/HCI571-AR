/*
 *  FiniteStateMachineBuilder.h
 *  Untitled
 *
 *  Created by Rafael Radkowski on 23.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */
#pragma once

#include "FiniteStateMachine.h"
#include "StateOperation.h"
#include <string>
#include <iostream>
#include <fstream>
#include "StateTypes.h"
#include <list>

using namespace std;

class FiniteStateMachineBuilder
{
protected:
	FiniteStateMachine* _destination;
	
private:
	
	typedef enum{
		LEVEL_0,
		LEVEL_1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4
	}Level;
	
	typedef struct{
		std::string xmi_type;
		std::string xmi_id;
		std::string name;
	}Package;
	
	typedef struct{
		std::string xmi_type;
		std::string xmi_id;
		std::string name;
		std::string toSubvertex;
	}EntryFunction;
	
	typedef struct{
		std::string xmi_type;
		std::string xmi_id;
		std::string name;
		std::string toSubvertex;
	}ExitFunction;
	
	typedef struct{
		std::string xmi_type;
		std::string xmi_id;
		std::string name;
		std::string toSubvertex;
	}DoFunction;
	
	typedef struct{
		std::string annotatedElement; // ID des Subvertex, den dieser Kommentar zugeordnet ist
		std::string xmi_id;
		std::string body; 
	}OwnedComment;
	
	
	typedef struct Subvertex{
		std::string xmi_type;
		std::string xmi_id;
		std::string name;
		std::string visibility;
		std::string outgoing;
		std::string incoming;
	};

	//  <trigger xmi:id="_14_0_888026c_1232699038635_228333_708" name="" visibility="public" event="_14_0_888026c_1232699246834_791001_724"/>
	typedef struct{
		std::string xmi_id;
		std::string name;
		std::string event;
	}Trigger;
	
	
	typedef struct{
		std::string xmi_id;
		std::string name;
		std::string target;
		std::string source;
		Trigger		trigger;
	}Transitions;
	
	//  <changeExpression xmi:type="uml:OpaqueExpression" xmi:id="_14_0_888026c_1232699251505_856729_725" name="">
	typedef struct{
		std::string xmi_type;
		std::string xmi_id;
		std::string name;
		std::string body;
		std::string	package; // Gibt an, zu welchem Packet dieser Ausdruck gehört
	}ChangeExpression;
	
	typedef struct{
		std::string xmi_type;
		std::string xmi_id;
		std::string name;
	}AnyChangeExpression;
	
	typedef struct {
		std::string name; // ID des Subvertex, den dieser Kommentar zugeordnet ist
		std::string xmi_id;
		std::string visibility;
		std::string xmi_type;
		std::string	value;
		std::string	ownerFunction; // Function, dem das Attribug zugeordet ist. 
	}OwnedAttribute;
	
	

	std::list<Package>			_packages;
	std::list<Subvertex>		_states;
	std::list<Transitions>		_transitions;
	std::list<ChangeExpression>	_expressions;
	std::list<EntryFunction>	_entryfunctions;
	std::list<ExitFunction>		_exitfunctions;
	std::list<OwnedComment>		_owndedComments;
	std::list<DoFunction>		_doFunctions;
	std::list<OwnedAttribute>	_attributes;
	std::list<AnyChangeExpression>	_anyExpressions;
	
	std::string				_currentPackageID;
	std::string				_currentSubvertexID;
	std::string				_currentFunctionID;
	
	Level					_currentLevel;
	
	///////////////////////////////////////////////////////////////////////
	// XML-Kram
	int findPackage(std::string inStr, std::string diagrammName);

	int getElement(std::string inStr);
	
	bool getSubvertex(std::string inStr);
	
	
	bool getTransition(std::string inStr);
	
	int getSubElement(std::string inStr);
	
	int getSubSubElement(std::string inStr);
	
	bool getTrigger(std::string inStr);
	
	bool getEntry(std::string inStr);
	
	bool getExit(std::string inStr);
	
	bool getDoActivity(std::string inStr);
	
	bool getOwnedComment(std::string inStr);
	
	bool getPackage(std::string inStr);
	
	bool getChangeExpression(std::string inStr);
	
	bool getOwnedAttribute(std::string inStr);
	
	bool getDefaultValue(std::string inStr);
	
	bool getAnyReceiveEvent(std::string inStr);
	
	// Gibt zurück, ob ein Element beendet ist oder nicht
	bool isEndOfElement(std::string inStr);
	
	/* Liefert die Attribute eines Elementes zurück
	 @param inStr - die aus der Datei gelesene Zeile
	 @param attribute - Bezeichnung des Attributes, nachdem gesucht wird
	 @return - den Wert des Attributes als String
	 */
	std::string getAttribute(std::string inStr, std::string attribute);
	
	
	/////////////////////////////////////////////////////////////////////////
	// Zu erstelen des Zustandsautomaten
	bool createAutomat(FiniteStateMachine* destination);
	
	std::string getStateType(std::string id);
	
	std::string getState(std::string id);
	
	/*
	 Gibt die Entry Function zu einem Zustand State zurück. 
	 Ist keine vorhanden kommt eine Leerer String zurück
	 */
	std::string getEntryFunction(std::string stateID);
	
	
	/*
	 Gibt die ID der Entry Function zu einem Zustand State zurück. 
	 Ist keine ID vorhanden kommt eine Leerer String zurück
	 */
	std::string getEntryFunctionID(std::string stateID);
	
	/*
	 Gibt die Exit Function zu einem Zustand State zurück. 
	 Ist keine vorhanden kommt eine Leerer String zurück
	 */
	std::string getExitFunction(std::string stateID);
	
	/*
	 Gibt die ID der Exit Function zu einem Zustand State zurück. 
	 Ist keine vorhanden kommt eine Leerer String zurück
	 */
	std::string getExitFunctionID(std::string stateID);
	
	/*
	 Gibt die Do Function zu einem Zustand State zurück. 
	 Ist keine vorhanden kommt eine Leerer String zurück
	 */
	std::string getDoFunction(std::string stateID);
	
	/*
	 Gibt die ID einer Do Function zu einem Zustand State zurück. 
	 Ist keine vorhanden kommt eine Leerer String zurück
	 */
	std::string getDoFunctionID(std::string stateID);
	
	/*
	 Gibt eine Liste mit allen Attributen einer Funktion zurück
	 */
	StateFunctionAttributeList getAttributesOfFunction(std::string functionID);
	
	/*
	 Gibt den Kommentar zu einem Zustand zurück.
	 Ein leeren String, falls kein Kommentar vorhanden ist. 
	 */
	std::string getComment(std::string stateID);
	
	/*
	 Liefert die Transitionen zu einem Zustand
	 @param string id - String mit der ID des Zustands, zu dem die Transitionen gesucht werden. 
	 @param String mit dem ID des Zustandes, zudem die Transition erfolgt
	 */
	std::string getTransitionOperation(std::string id);
	

	
	
	/////////////////////////////////////////////////////////////////////////
	// Diese Funktion verbindet die Funktionen mit den Zustandsautomenten
	virtual void setFunction(std::string stateName, std::string functionname, std::string parameter1, StateFunctionAttributeList attribute_list, StateOperationType type);
	
public:
	FiniteStateMachineBuilder();
	
	
	/*
	 Erstellt eine Zustandsmaschine auf Basis eines UML 2.0 Zustandsdiagramms. 
	 @param umlfile - string mit dem Filenamen des UML 2.0 Files
	 @param diagrammname - Name des UML 2.0 Diagramms, indem das Zustandsmodell enthalten ist
	 @param destination - Poiner einer FiniteStateMachine, in der das Diagramm erstellt werden soll
	*/
	bool build(std::string umlfile, std::string diagrammname, FiniteStateMachine* destination);

};