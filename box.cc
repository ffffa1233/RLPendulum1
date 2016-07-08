#include "box.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

b2Body* verticalBody;
b2Body* horizonBody;
b2Body* lineBody;

b2Vec2 gravity(0.0f, -10.0f);
b2World* world;

float timeStep = 1.0f / 60.0f;
int velocityIterations = 6;
int positionIterations = 2;

/* Box2D 초기화 */
void box_init(){
	double randomAngle = (rand()%160) + 10;

	world = new b2World(gravity, true);

	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;

	b2PolygonShape rectangleShape;
	rectangleShape.SetAsBox(10, 0.5);

	b2FixtureDef rectangleFixtureDef;
	rectangleFixtureDef.shape = &rectangleShape;
	rectangleFixtureDef.density = 3;

	myBodyDef.position.Set(0, 14);
	verticalBody = world->CreateBody(&myBodyDef);
	verticalBody->CreateFixture(&rectangleFixtureDef);
	verticalBody->SetTransform(b2Vec2(9.95*cos(randomAngle*DEGTORAD), 14-(9.95-9.95*sin(randomAngle*DEGTORAD))), randomAngle*DEGTORAD);

	rectangleShape.SetAsBox(6, 2);
	rectangleFixtureDef.shape = &rectangleShape;
	rectangleFixtureDef.density = 3;

	myBodyDef.position.Set(0, 2);
	horizonBody = world->CreateBody(&myBodyDef);
	horizonBody->CreateFixture(&rectangleFixtureDef);

	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(0, 0);
		
	b2PolygonShape lineShape;
	lineShape.SetAsEdge(b2Vec2(-10000,0), b2Vec2(10000,0) );

	b2FixtureDef lineFixtureDef;
	lineFixtureDef.shape = &lineShape;
	lineFixtureDef.density = 1;

	lineBody = world->CreateBody(&myBodyDef);
	lineBody->CreateFixture(&lineFixtureDef);

	b2RevoluteJointDef upperJointDef;
	upperJointDef.localAnchorA.Set(-9.95, 0);
	upperJointDef.localAnchorB.Set(0, 1.95);
	upperJointDef.bodyA = verticalBody;
	upperJointDef.bodyB = horizonBody;
		
	world->CreateJoint(&upperJointDef);

	/* 힘 초기화 */
	world->ClearForces();
}

/* bodies의 x축 위치값을 받아옵니다. */
double get_position(){
	world->Step(timeStep, velocityIterations, positionIterations);
	
	return horizonBody->GetPosition().x;
	
}

/* bodies의 x축 속도값을 받아옵니다. */
double get_velocity(){
	world->Step(timeStep, velocityIterations, positionIterations);

	return horizonBody->GetLinearVelocity().x;
}

double get_angle(){
	world->Step(timeStep, velocityIterations, positionIterations);

	return verticalBody->GetAngle()*RADTODEG;
}

double get_angleVelocity(){
	world->Step(timeStep, velocityIterations, positionIterations);

	return verticalBody->GetAngularVelocity();
}

/* bodies에 force값 만큼 힘을 가합니다. */
void apply_force(int force){
	world->Step(timeStep, velocityIterations, positionIterations);

	horizonBody->ApplyForce(b2Vec2(force*100-5*100, 0), horizonBody->GetWorldCenter() );
}

/* Box2D 해제  */
void box_cleanup(){
	delete world;
}
