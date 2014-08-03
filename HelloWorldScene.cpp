#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
void HelloWorld::onEnter()
{
    Layer::onEnter();
    
    auto scene = getScene();
    auto world = scene->getPhysicsWorld();
    world->setGravity(Vec2::ZERO);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(HelloWorld::onContactSeperate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    auto wall = Node::create();
    wall->setPhysicsBody(PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.1f, 1, 0.0f)));
    wall->setPosition(Vec2(visibleSize/2));
    this->addChild(wall);
    
    // create five balls with random velocity and direction
    for (int i = 0; i < 5; ++i)
    {
        Size size(10 + CCRANDOM_0_1()*10, 10 + CCRANDOM_0_1()*10);
        Size winSize = visibleSize;
        Vec2 position = Vec2(winSize.width, winSize.height) - Vec2(size.width, size.height);
        position.x = position.x * CCRANDOM_0_1();
        position.y = position.y * CCRANDOM_0_1();
        position = origin + position + Vec2(size.width/2, size.height/2);
        Vect velocity((CCRANDOM_0_1() - 0.5)*400, (CCRANDOM_0_1() - 0.5)*400);
        auto ball = Sprite::create("bang.png");
        ball->setPosition(position);
        ball->setPhysicsBody(PhysicsBody::createCircle(ball->getContentSize().width/2,PhysicsMaterial(0.1f, 1, 0.0f)));
        ball->getPhysicsBody()->setContactTestBitmask(UINT_MAX);
        ball->getPhysicsBody()->setVelocity(velocity);
        this->addChild(ball);
    }

    
    return;
}

bool HelloWorld::onContactBegin(PhysicsContact& contact)
{
    auto a = contact.getShapeA()->getBody();
    auto b = contact.getShapeB()->getBody();
    
    // save the velocity, ignore the direction of velocity, only save the length
    float* v = new float[2];
    v[0] = a->getVelocity().length();
    v[1] = b->getVelocity().length();
    
    contact.setData(v);
    
    return true;
}

void HelloWorld::onContactSeperate(PhysicsContact& contact)
{
    auto a = contact.getShapeA()->getBody();
    auto b = contact.getShapeB()->getBody();
    
    // restore the velocity, keep the direction of the velocity.
    float* v = (float*)contact.getData();
    auto va = a->getVelocity();
    auto vb = b->getVelocity();
    va.normalize();
    vb.normalize();
    a->setVelocity(va * v[0]);
    b->setVelocity(vb * v[1]);
    
    delete v;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
