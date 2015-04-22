# Main cmake file 

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 OLD)

# Main project name is ARLens
project(HCI571_Statemachine)


# Add all files to the configuration
file(GLOB HCI571_Statemachine_SRC
	"main.cpp"
	"MyFiniteStateMachineBuilder.h"
	"MyFiniteStateMachineBuilder.cpp*"
	"MyTestStateOperation.h"
	"MyTestStateOperation.cpp"
)

set(Statemachine_SRC
	"FiniteStateMachine.h"
	"FiniteStateMachine.cpp"
	"FiniteStateMachineBuilder.h"
	"FiniteStateMachineBuilder.cpp"
	"State.h"
	"State.cpp"
	"StateOperation.h"
	"StateOperation.cpp"
	"StateParameter.h"
	"StateParameter.cpp"
	"Transition.h"
	"Transition.cpp"
	"StateTypes.h"

)



#  SRC Groups
source_group(Statemachine FILES ${Statemachine_SRC})

# Create an executable
add_executable( HCI571_Statemachine ${HCI571_Statemachine_SRC} ${Statemachine_SRC} )


