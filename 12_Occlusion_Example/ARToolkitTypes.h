//
//  ARToolkitTypes.h
//  HCi571X-ARToolkit
//
//  Created by Dr.-Ing. Rafael Radkowski on 2/20/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

//////////////////////////////////////////////////////////
// This struct describes the ARToolkit input data
typedef struct
{
    int patt_id;
    std::string name;
    int size;
    
}PatternInfo;

// Vector that keeps all pattern
typedef std::vector<PatternInfo> PatternInfoVec;



//////////////////////////////////////////////////////////
// This struct describes the ARToolkit - OSG interface
typedef struct
{
    osg::Matrixd        matrix;
    double                 cf;
    
}ObjectInfo;
typedef std::vector<ObjectInfo> ObjectInfoVec;
