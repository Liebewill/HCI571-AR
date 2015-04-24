//
//  ARPaddleTypes.h
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/3/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once


typedef enum
{
    IDLE,
    CLOSE_IN,
    CLOSE_OUT,
}ARPaddleTypes;


typedef enum
{
    DIR_IDLE,
    NORTH,
    NORTH_EAST ,
    EAST , //2
    SOUTH_EAST ,
    SOUTH ,
    SOUTH_WEST ,
    WEST ,//7
    NORTH_WEST 
    
}PattDirection;