//
//  main.cpp
//  FiniteStateMachine
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include "FiniteStateMachine.h"
#include "MyTestStateOperation.h"


using namespace std;

int main(int argc, char* atgv[] )
{

    // Create a finite state machine
    FiniteStateMachine* statemachine = new FiniteStateMachine();

    // Add three states
    statemachine->addState("StateA");
    statemachine->addState("StateB");
    statemachine->addState("StateC");

    // create the transitions between these states
    statemachine->addTransition(string("StateA"), string("StateB"), string("1"));
    statemachine->addTransition(string("StateB"), string("StateC"), string("2"));
    statemachine->addTransition(string("StateC"), string("StateA"), string("3"));

    // add a state function and specify its function type
    statemachine->setStateFunction("StateA", new MyTestStateOperation("Function State A *"));
    statemachine->setStateFunction("StateB", new MyTestStateOperation("Function State B **"),
                               StateOperationType::ON_EXIT);
    statemachine->setStateFunction("StateC", new MyTestStateOperation("Function State C ***"),
                               StateOperationType::ON_ENTRY);

    // set a start state
    statemachine->setStartState("StateA");

    std::string key;
    std::cin >> key;

    // loop till e gets pressed.
    while(key.compare("e") != 0)
    {
        statemachine->fire(key);
        std::cin >> key;
    }
}