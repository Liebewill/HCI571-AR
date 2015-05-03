//
//  ARPaddleCallback.cpp
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "ARPaddleCallback.h"
#include <iostream>
#include <strstream>
#include <string>
#include "StateTypes.h"

extern int IntersectTraversalMask;


std::string g_paddlelocation = "";
std::string g_modelname = "";
std::string g_direction = "";
std::string g_direction2 = "";
std::string g_speed = "";


bool start = false;
int cc = 0;
const int max_length = 50;


ARPaddleCallback::ARPaddleCallback(osg::Group* intersection_group, ARTagNode* marker):
	_intersection_group(intersection_group), _marker_node(marker)
{
    _current_Position = _marker_node->getMatrix().getTrans();
    _new_Position = osg::Vec3(0.0, 0.0, 0.0);
    _final_Position = osg::Vec3(0.0, 0.0, 0.0);
    _final_Up = osg::Vec3(0.0, 0.0, 1.0);
    
    _moveable_object_matrix.identity();
    
    
    _boundingSphere = _marker_node->getBound();
    
    
    _distVisitor = new DistanceNodeVisitor();
    
    
    _worldReferenceSystem = NULL;
    
    for(int i=0;i<8; i++)
        _zPos.push_back(0.0);
    _lastPos = 0.0;
    _pattAngle = 0.0;
    
    _t0  = t1 = (double)clock()/CLOCKS_PER_SEC;
    
    _ts0 = _ts1 = (double)clock()/CLOCKS_PER_SEC;
    _lastLocation = osg::Vec3(0.0, 0.0, 0.0);
    
    _withARBook = false;
    _swipeCount = -1;
    _swipeInit = false;

	_collided = false;
    
    createStateMachine();
}

void ARPaddleCallback::createStateMachine(void)
{
    
    ARPaddleOperation* operation = new ARPaddleOperation();
    
    _fsm = new FiniteStateMachine();
    
    _fsm->addState("IDLE");
    _fsm->addState("Pick");
    _fsm->addState("Drag");
    _fsm->addState("Drop");
    _fsm->addState("Select");
    _fsm->addState("Push");
    _fsm->addState("MagicBookPick");
    _fsm->addState("Swipe");
    
    _fsm->setStateFunction("IDLE", operation, ON_ENTRY);
    _fsm->setStateFunction("Pick", operation, ON_ENTRY);
    _fsm->setStateFunction("Drag", operation, ON_ENTRY);
    _fsm->setStateFunction("Drop", operation, ON_ENTRY);
    _fsm->setStateFunction("Select", operation, ON_ENTRY);
    _fsm->setStateFunction("Push", operation, ON_ENTRY);
    _fsm->setStateFunction("MagicBookPick", operation, ON_ENTRY);
    _fsm->setStateFunction("Swipe", operation, ON_ENTRY);
    
    _fsm->addTransition("IDLE", "Pick", "close_in");
    _fsm->addTransition("Pick", "IDLE", "close_out");
    _fsm->addTransition("Pick", "Drag", "take");
    _fsm->addTransition("Drag", "Drop", "drop");
    _fsm->addTransition("Drop", "IDLE", "ready");
    _fsm->addTransition("IDLE", "Select", "touch");
    _fsm->addTransition("Select", "IDLE", "relase");
    _fsm->addTransition("IDLE", "Push", "push");
    _fsm->addTransition("Push", "IDLE", "ready");
    _fsm->addTransition("IDLE", "MagicBookPick", "arb_in");
    _fsm->addTransition("MagicBookPick", "Drop", "drop");
    _fsm->addTransition("IDLE", "Swipe", "swipe");
    _fsm->addTransition("Swipe", "IDLE", "ready");
    _fsm->setStartState("IDLE");

}


/*
 Set the world reference system
 @param: the osg::MatrixTransform node that acts as the reference coordinate system for this paddle.
 */
void ARPaddleCallback::setWorldCoordinateRefSystem( ARTagNode*  worldReferenceSystemNode)
{
	_worldReferenceSystemNode =worldReferenceSystemNode;
	_worldReferenceSystem = &_worldReferenceSystemNode->getMatrix();
}


bool ARPaddleCallback::intersect(const osg::Vec3d& start,
                                       const osg::Vec3d& end,
                                       osg::Vec3d& intersection,
                                       osg::Vec3d& normal,
                                       std::string& modelname) const
{
    // This creates a line segment intersector
    osg::ref_ptr<osgUtil::LineSegmentIntersector> lsi = new osgUtil::LineSegmentIntersector(start,end);
    
    // The traversal mask visitor. It defines which objects should be traversed
    osgUtil::IntersectionVisitor iv(lsi.get());
    //iv.setTraversalMode(osgUtil::IntersectionVisitor::TRAVERSE_ALL_CHILDREN);
    iv.setTraversalMask(IntersectTraversalMask);
    
    // This starts the traversl / ray intersection test.
    _intersection_group->accept(iv);
    
    // this returns the first contact.
    if (lsi->containsIntersections())
    {
        
        intersection = lsi->getIntersections().begin()->getWorldIntersectPoint();
        normal = lsi->getIntersections().begin()->getWorldIntersectNormal();
        osg::NodePath path = lsi->getIntersections().begin()->nodePath;
        modelname = path[path.size()-2]->getName();
		
        return true;
    }
    return false;
}


/*
 This function computes the distance to all other objects in the intersection group
 */
bool ARPaddleCallback::computeDistance(std::string& result, std::string& object)
{
    _intersection_group->accept(*_distVisitor);
    
  
    double min_distance = 100000000.0;
    std::string min_object = "";
    double app_angle = 0;
    double app_speed_angle = 0;
    
    int num = _distVisitor->size();
    for ( int i=0; i<num; i++)
    {
        osg::Vec3 loc1 = _distVisitor->getLocation(i);
        std::string n1 = _distVisitor->getName(i);
        
        
        osg::Vec3d pad = _worldRefLocation;
        pad.z() = pad.z() - 30.0;
        
        // Calculates the distance and writes them into the map
        double dist = calcEuclidDistance(pad, loc1);
        _objectDistance[n1] = dist;
        
        double approachingAngle = 0.0;
        double approachingSpeedAngle = 0.0;
        
        // Calculate the approaching direction
        calcObjectAngle(loc1, pad, &approachingAngle, &approachingSpeedAngle);
        
        
        // Look for the closest object
        if(dist < min_distance)
        {
            min_distance = dist;
            min_object = n1;
            app_angle = approachingAngle;
            app_speed_angle = approachingSpeedAngle;
        }
    }
    
    
    
   //std::cout << "Closest objet is " << min_object << " at " << min_distance <<  std::endl;
    
    if(min_distance < 40.0f)
    {
        // Check the approaching direction
        if(app_angle > 60 && app_angle < 130 )
        {
            g_modelname = min_object;
            object = min_object;
            result = "close_in";
            return true;
        }
    }
    else if(min_distance < 50.0f)
    {
        g_modelname = "-";
        object = "-";
        result = "close_out";
        return "true";
    }
    else if(min_distance < 60 && app_angle < 35 )
    {
        g_modelname = min_object;
        object = min_object;
        result = "push";
        return true;
    }
    else
        result = "-";
    return false;

}



/*!
 This function calculates the distance to all ARBook objects.
 @param result: a state machine message
 @param object: the object that has been detected;
 */
bool ARPaddleCallback::computeDistanceARBook(std::string& result, std::string& object)
{
    this->_ARBookGroup->accept(*_arbookvisitor);
    
    // Invert the matrix in world reference coordinates
    osg::Matrixd wInv = osg::Matrix::inverse(*_worldReferenceSystem);
    
    
    double min_distance = 100000000.0;
    std::string min_object = "";
    double app_angle = 0;
    double app_speed_angle = 0;
    
    static int num = _arbookvisitor->size();
    for ( int i=0; i<num; i++)
    {
        osg::Matrixd loc1 = _arbookvisitor->getWorldMatrix(i);
        std::string n1 = _arbookvisitor->getName(i);
        
        
        // Multiply the position of the Paddle's tracking marker with the inverted matrix.
        osg::Matrixd mat1 =  loc1 * wInv;
        
        osg::Vec3d trans = mat1.getTrans();
        
        // Calculates the distance and writes them into the map
        double dist = calcEuclidDistance(_worldRefLocation, trans);
        _objectDistance[n1] = dist;
        
        double approachingAngle = 0.0;
        double approachingSpeedAngle = 0.0;
        
        // Calculate the approaching direction
       // calcObjectAngle(trans, _worldRefLocation, &approachingAngle, &approachingSpeedAngle);
        
        
        
        // Look for the closest object
        if(dist < min_distance)
        {
            min_distance = dist;
            min_object = n1;
            app_angle = approachingAngle;
            app_speed_angle = approachingSpeedAngle;
        }
         
    }

    //std::cout << "Closest objet is " << min_object << " at " << min_distance <<  std::endl;
    
    
    if(min_distance < 60.0f)
    {

        //g_modelname = min_object;
        object = min_object;
        result = "arb_in";
        return true;
        
    }
    else if(min_distance < 80.0f)
    {
        //g_modelname = "-";
        object = "-";
        result = "arb_out";
        return true;
    }
    else
    {
        object = "-";
        result = "-";
    }
    return false;
    
    return true;
}



void ARPaddleCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    std::string object;
    std::string  result;

	_worldReferenceSystem = &_worldReferenceSystemNode->getMatrix();
    
    // Fetch the current position of the 3D object
    _current_Position = _marker_node->getMatrix().getTrans();
    
    // Calc the location of this paddle with respect to the world
    // coordinate system
    calcPaddleWorldLocation();
    
    
    // Compute the leviation of that paddle
    computeLevitation(_worldRefLocation, result);
    _fsm->fire(result, new StateParameter(object));
    
    // Compute intersections for the select function
	computeIntersections();

    
    // Compute distances for drag & drop
    computeDistance(result, object);
    _fsm->fire(result, new ARPaddleStateParameters(object, _worldRefLocation, _worldRefDirection) );
    _fsm->fire("ready");
    

    // Check the angle
    // The angle calculation is part of the
    // function calcPaddleWorldLocation
    if(_pattAngle > 25)
    {
        _fsm->fire("drop", new ARPaddleStateParameters( "", _worldRefLocation, _worldRefDirection ));
        _fsm->fire("ready");
    }
    
    
    if(_withARBook)
    {
        computeDistanceARBook(result, object);
        _fsm->fire(result, new ARPaddleStateParameters(object, _worldRefLocation, _worldRefDirection) );
        _fsm->fire("ready");
    }
    
    
    
    computeSwipe(_worldRefLocation, _worldRefDirection, result);
    _fsm->fire(result, new ARPaddleStateParameters( "", _worldRefLocation, _worldRefDirection ));
    _fsm->fire("ready");

    
    nv->traverse(*node);
}





/*
 This function calculates the next step
 */
bool ARPaddleCallback::computeIntersections(void)
{
    _collision_down = false;
    

    
    //////////////////////////////////////////////////////////
    // At this time, the current position is also the new position.
    // We want to change the z-coordinate.
    // Later, after we introduce movement, the new position is the
    // location the user intends to have.
    _new_Position.x() = _current_Position.x();
    _new_Position.y() = _current_Position.y();
    _new_Position.z() = _current_Position.z();
    
	// Test the collision to the underground
    osg::Vec3d start = _new_Position + osg::Vec3(0.0, 0.0, 30.0);
	osg::Vec3d end = _new_Position + osg::Vec3(00.0, 1000.0, 30.0);
	
    osg::Vec3d ip, np;
    std::string modelname;
	if(intersect(start, end, ip, np, modelname))
	{
		//std::cout << "intersect with " << modelname << " : " << ip.x() << " : " << ip.y() << " : " << ip.z()  << std::endl;

		if(!_collided)
		{
			_collided = true;
			if(modelname == "Kick")
				PlaySound(TEXT("Kick-Drum-1.wav"), NULL, SND_ASYNC);
			else if(modelname == "Snare")
				PlaySound(TEXT("Hip-Hop-Snare-1.wav"), NULL, SND_ASYNC);
			else if(modelname == "HiHat")
				PlaySound(TEXT("Closed-Hi-Hat-1.wav"), NULL, SND_ASYNC);
		}

	}
    else
    {
        g_modelname = "";
		_collided = false;

    }
    
   
    return true;
    
}


double ARPaddleCallback::calcEuclidDistance(osg::Vec3d distA, osg::Vec3d distB)
{
    
    double diffx = pow( (double)(distA.x() - distB.x()), 2.0);
    double diffy = pow( (double)(distA.y() - distB.y()), 2.0);
    double diffz = pow( (double)(distA.z() - distB.z()), 2.0);
    
    return sqrt (diffx + diffy + diffz);
    
    
}



/*!
 This calculates the angle between the object and the paddle
 @param objectLoc: the location of the object in world coordinate or world reference coordinates
 @param paddleLoc: the location of the paddle in world coordinate or world reference corrdinates.
 @param paddleDir: this is the desired dir of the paddle in world coordinate or world reference corrdinates.
 @param angle: the output angle between the paddle and the
 */
bool ARPaddleCallback::calcObjectAngle(osg::Vec3 objectLoc, osg::Vec3 paddleLoc, double* angle, double* speed)
{
    // get the direction vector of the paddle with respect to the world coordinate system
    _worldRefDirection = osg::Vec3d(_world_paddle_relation(1,0),_world_paddle_relation(1,1), _world_paddle_relation(1,2)) ;
    
    // calculate the direction between paddle and object
    osg::Vec3d vecPaddleToObject = objectLoc - paddleLoc;
    vecPaddleToObject.normalize();
    
    double dp = dot_product(_worldRefDirection, vecPaddleToObject);
    double dirAngle = acos(dp)/3.14159 * 180.0;
    (*angle) = dirAngle;
    
    osg::Vec3d dirAccelerationVector = (_worldRefLocation - _worldRefLocation_t1 );
    osg::Vec2d dir = osg::Vec2d(dirAccelerationVector.x(), dirAccelerationVector.y());
    dir.normalize();
    dir.length();
    double length = dirAccelerationVector.length();
    dirAccelerationVector.normalize();
    
    double dp2 = dot_product(dirAccelerationVector, vecPaddleToObject);
    double dirAccAngle = acos(dp2)/3.14159 * 180.0;
    (*speed) = length;
    
    
 //  std::cout << "X: " << _worldRefDirection.x() << "\tY: " << _worldRefDirection.y() << "\tZ: " << _worldRefDirection.z() <<
 //   "\tA: " << dirAngle  << "\tSA: " << length << std::endl;

    _worldRefDirection_t1 = _worldRefDirection;
    _worldRefLocation_t1 = _worldRefLocation;
    
    return true;
}



/*
 Traverse the colider scene graph and check for bounding sphere intersections
 */
bool ARPaddleCallback::computeCollisions(osg::Group* node)
{
    
    osg::BoundingSphere bs = node->getBound();
    
    // Check for collisions
    if(_boundingSphere.intersects(bs) == true)
    {
        
        std::cout << "Bounding Sphere Intersection found: " << node->getName()   << std::endl;
        
        return true;
    }
    
    // Countinue traversing
    int num = node->getNumChildren();
    for(int i=0; i< num; i++)
    {
        return computeCollisions((osg::Group*)node->getChild(i));
    }
    return false;
    
}


/*
 This function calculates the location of this paddle with respect to the
 world reference system.
 */
bool ARPaddleCallback::calcPaddleWorldLocation(void)
{
    if(_worldReferenceSystem == NULL) return false;
    
    // Invert the matrix in world reference coordinates
    osg::Matrixd wInv = osg::Matrix::inverse(*_worldReferenceSystem);
    
    // Multiply the position of the Paddle's tracking marker with the inverted matrix.
	_world_paddle_relation =   _marker_node->getMatrix() * wInv;
    
    // Get the location with respect to the world coordinate system
    _worldRefLocation = _world_paddle_relation.getTrans();
    
    // Get the orientation (the up vector) with respect to the world coordinate system
    osg::Vec3d up = osg::Vec3d(_world_paddle_relation(2,0), _world_paddle_relation(2,1), _world_paddle_relation(2,2));
        
    // Create a normal up vector
    const osg::Vec3d  up_n = osg::Vec3d(0.0, 0.0, 1.0) ;
    
    double dp = dot_product(up, up_n);
    _pattAngle = acos(dp)/3.14159 * 180.0;

    
   // std::cout << "X: " << location.x() << "\tY: " << location.y() << "\tZ: " << location.z() << "\tA: " << angle << std::endl;
   
    
    std::strstream conv;
    std::string out;
    
    g_paddlelocation = std::string("X: ");
    conv << _worldRefLocation.x();
    conv >> out;
    g_paddlelocation.append(out);
    g_paddlelocation.append("    Y: ");
    conv.clear();
    conv << _worldRefLocation.y();
    conv >> out;
    g_paddlelocation.append(out);
    g_paddlelocation.append("    Z: ");
    conv.clear();
    conv << _worldRefLocation.z();
    conv >> out;
    g_paddlelocation.append(out);
    g_paddlelocation.append("    A: ");
    conv.clear();
    conv << _pattAngle;
    conv >> out;
    g_paddlelocation.append(out);

	return true;
}


void ARPaddleCallback::getEulerFromQuat(osg::Quat q, double& heading, double& attitude, double& bank)
{
    
    double limit = 0.499999;
    
    double sqx = q.x()*q.x();
    double sqy = q.y()*q.y();
    double sqz = q.z()*q.z();
    
    double t = q.x()*q.y() + q.z()*q.w();
    
    if (t>limit) // gimbal lock ?
    {
        heading = 2 * atan2(q.x(),q.w());
        attitude = osg::PI_2;
        bank = 0;
    }
    
    else if (t<-limit)
    {
        heading = -2 * atan2(q.x(),q.w());
        attitude = - osg::PI_2;
        bank = 0;
    }
    else
    {
        heading = atan2(2*q.y()*q.w()-2*q.x()*q.z() , 1 - 2*sqy - 2*sqz);
        attitude = asin(2*t);
        bank = atan2(2*q.x()*q.w()-2*q.y()*q.z() , 1 - 2*sqx - 2*sqz);
    }
    
}


double ARPaddleCallback::dot_product(osg::Vec3d a,osg::Vec3d b )
{
    double dp = 0.0f;
    for (int i=0;i<3;i++)
        dp += a[i] * b[i];
    return dp;
}


/*
 Computes the leviation of that paddle.
 */
bool  ARPaddleCallback::computeLevitation(osg::Vec3 worldRefLoc, std::string& result)
{

    _zPos.insert(_zPos.begin(), worldRefLoc[2] - _lastPos);
    _zPos.pop_back();
    
    double max = 0.0;
    for (int i=0; i<8; i++)
    {
        max += _zPos[i];
    }
   // std::cout << max << std::endl;
    
    _lastPos =  worldRefLoc[2];
    
    
    if(max > 20.0)
    {
        result = "take";
        return true;
    }
    
    
    result = "-";
    return false;

}

/*!
 This adds the ARBook to this NodeCallback.
 @param ARBookGroup  -pointer to the subscene graph that keeps the arbook objects
 */
void ARPaddleCallback::setARBookGroup( osg::Group* ARBookGroup)
{
    _ARBookGroup = ARBookGroup;
    _withARBook = true;
    
    _arbookvisitor = new ARBookVisitor(_worldReferenceSystem);
    
}


/*!
 Calcuates the cross product between two vectors
 */
osg::Vec3 ARPaddleCallback::cross_product(osg::Vec3d a,osg::Vec3d b )
{
    osg::Vec3d r;
    r[0] = a[1]*b[2]-a[2]*b[1];
    r[1] = a[2]*b[0]-a[0]*b[2];
    r[2] = a[0]*b[1]-a[1]*b[0];
    
    return r;
}


double ARPaddleCallback::signed_angle(osg::Vec3d a, osg::Vec3d b)
{

    double magnitude = acos(dot_product(a, b));
    
    osg::Vec3d axis = cross_product(a, b);
    axis.normalize();
    
    //double dir = dot_product(b, cross_product(axis, a));
    double  dir = dot_product( osg::Vec3d(0.0,0.0,1.0), axis);
    
    if( dir < 0 )
    {
       // std::cout << "------" << std::endl;
        magnitude = -magnitude;
    }
    else
    {
        //std::cout << "+++++" << std::endl;
    }
    return magnitude;
    
}


/*
 Calculate a symbol that represents the current direction of a paddle
 */
PattDirection ARPaddleCallback::codeDirection(osg::Quat dir)
{
    double heading;
    double attitude;
    double bank;
    
    getEulerFromQuat(dir, heading,  attitude,  bank );
    
    if(heading < 0)
    {
        heading = 2.0 * 3.14 + heading;
    }
    
    
    // std::cout << heading<< " : " << attitude << " : " << bank << std::endl;
    
    
    
    
    const double divider0 = 0.3925;
    const double divider1 = 3. * 0.3925;
    const double divider2 = 5. * 0.3925;
    const double divider3 = 7. * 0.3925;
    const double divider4 = 9. * 0.3925;
    const double divider5 = 11. * 0.3925;
    const double divider6 = 13. * 0.3925;
    const double divider7 = 15. * 0.3925;
    
    
    if(heading < divider0)
    {
        g_direction = "Dir: NORTH";
        return NORTH;
    }
    else if(heading < divider1)
    {
        g_direction = "Dir: NORTH_EAST";
        return NORTH_EAST;
    }
    else if(heading < divider2)
    {
        g_direction = "Dir: EAST";
        return EAST;
    }
    else if(heading < divider3)
    {
        g_direction = "Dir: SOUTH_EAST";
        return SOUTH_EAST;
    }
    else if(heading < divider4)
    {
        g_direction = "Dir: SOUTH";
        return SOUTH;
    }
    else if(heading < divider5)
    {
        g_direction = "Dir:SOUTH_WEST";
        return SOUTH_WEST;
    }
    else if(heading < divider6)
    {
        g_direction = "Dir: WEST";
        return WEST;
    }
    else if(heading < divider7)
    {
        g_direction = "Dir: NORTH_WEST";
        return NORTH_WEST;
    }
    else
    {
        g_direction = "Dir: NORTH";
        return NORTH;
    }
    
}


int ARPaddleCallback::codeMovementDirection(osg::Vec3 location)
{
    
    osg::Vec3d dir = location - _lastLocation;
    dir.normalize();
    osg::Vec3d normal =  osg::Vec3d(0.0,1.0,0.0);
    
    double dp2 =  signed_angle(dir, normal);
    double angle = dp2/3.14159 * 180.0;
    
    
    if(angle < -135.0)
    {
        g_direction2 = "Dir: SOUTH";
        return SOUTH;
    }
    else if(angle < -45.0)
    {
        g_direction2 = "Dir: WEST"; //1
        return WEST;
    }
    else if(angle < 45.0)
    {
        g_direction2 = "Dir: NORTH";
        return NORTH;
    }
    else if(angle < 135.0)
    {
        g_direction2 = "Dir: EAST";
        return EAST;
    }
    else if(angle < 180.0)
    {
        g_direction2 = "Dir: SOUTH";
        return SOUTH;
    }
	
	return -1;
}


bool speedInit =false;
/*
 This encodes the speed of the AR Paddle
 */
int ARPaddleCallback::codeSpeed(osg::Vec3 location)
{
    // 1. Calculated movement distance
    double distance = calcEuclidDistance(location, _lastLocation);
 
    _ts0 = (double)clock()/CLOCKS_PER_SEC;
    
    double delta = _ts0 - _ts1;
    
    double velocity = distance / delta;
    velocity = velocity/1000.0;
    
    
    osg::Vec3d dir = location - _lastLocation;
    dir.normalize();
    osg::Vec3d normal =  osg::Vec3d(0.0,1.0,0.0);
    
    if(dir.length() != 0 )
    {
      //  double dp = dot_product(dir, normal);
     //   double pattMovementAngle = acos(dp)/3.14159 * 180.0;
    
    
        double dp2 =  signed_angle(dir, normal);
        double pattMovementAngle = dp2/3.14159 * 180.0;
    
        
        _lastLocation = location;
        _ts1 = _ts0;
       // std::cout << "Velocity: " << velocity << " \tDist: " << distance <<  "\tAngle: " << pattMovementAngle <<std::endl;
        if(speedInit == false)
        {
            speedInit = true;
            return 1;
        }
        if(velocity < 0.10)
        {
            g_speed = "Velocity: STOP";
            _lastVel = 1;
            return 1;
        }
        else if(velocity < 0.8)
        {
            g_speed = "Velocity: SLOW";
            _lastVel = 2;
            return 2;
        }
        else
        {
            g_speed = "Velocity: FAST";
            _lastVel = 3;
            return 3;
        }
    }
    return _lastVel;
    
}


unsigned ARPaddleCallback::hamdist(unsigned x, unsigned y)
{
    unsigned dist = 0, val = x ^ y;
    
    // Count the number of set bits
    while(val)
    {
        ++dist;
        val &= val - 1;
    }
    
    return dist;
}


unsigned ARPaddleCallback::hamdistVector(std::vector<int> a, std::vector<int> b)
{

    const int size = a.size();
    unsigned int distA = 0;
    
    for(int i=0; i< size; i++)
    {
        distA += hamdist(a[i], b[i]);
    }
    return distA;
}

/*
 Compute the swipe.
 */
bool ARPaddleCallback::computeSwipe(osg::Vec3  worldRefLoc, osg::Vec3  worldRefDir, std::string& result)
{
    result = ""; 
    
    // 1. the direction of the object is calculated
    PattDirection dir = codeDirection(_marker_node->getMatrix().getRotate());
    
    int dir2 = codeMovementDirection(_marker_node->getMatrix().getTrans());
    
    // 2. compute the speed of an object
    int velocity = codeSpeed(_marker_node->getMatrix().getTrans());
    
    
    
    // 3. store the data only if the swipe gots init
    if(!_swipeInit)
    {
        _swipeInit = true;
        _lastVel = 0.0;
        for(int i=0;i<max_length; i++)
        {
            _speedVec.push_back(0.0);
            _dirVec.push_back(0.0);
            _speedVecRef.push_back(3);
            _dirVecRef.push_back(EAST);
        }
        
      
        
        return false;
    }
     if(velocity == 3) start = true;
    
    if(start == false) return false;
    
    
    _speedVec.insert(_speedVec.begin(), velocity);
    _speedVec.pop_back();
    
    _dirVec.insert(_dirVec.begin(), dir2);
    _dirVec.pop_back();
    
    // 4. Compare the vectors
    int distDir = hamdistVector(_dirVecRef, _dirVec);
    int distSpeed = hamdistVector(_speedVecRef, _speedVec);
   
/*
    std::cout << "Dir: ";
    for(int i=0; i<max;i++)
    { 
        std::cout << _dirVec[i] << ": " ;
    }
     std::cout << "-----" << distDir<<  std::endl;
    */
   //std::cout << "Vel: ";
    for(int i=0; i<max_length;i++)
    {
       // std::cout << _speedVec[i] << ": ";
    }
   //  std::cout << "-----"<< distSpeed << std::endl;
    
  
    
    if(cc == max_length)
    {
        //std::cout << "TRIGGER DIR" << distDir<<  std::endl;
        _dirVecRef = _dirVec;
        _speedVecRef = _speedVec;
        std::cout << "TRIGGER RECORDED " << std::endl;
    }
    cc++;
    
    if(distDir < 20.0 && distSpeed < 25.0)
    {
        if(_swipeCount < 0)
        {
            std::cout << "TRIGGER DIR " << distDir << "\tSPEED " << distSpeed << std::endl;
            result = "swipe";
            _swipeCount = 60;
        }
    }
    _swipeCount--;
    if(_swipeCount == 0)
        std::cout << "RESET DIR " << std::endl;

	return false;
}






