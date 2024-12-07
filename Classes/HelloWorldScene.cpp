/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "audio/include/AudioEngine.h" // necesario para el sonido

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    // layer is an autorealise object
    auto layer = HelloWorld::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); // dibujar el círculo de las físicas // modificacion 

    // add layer as child to scene
    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void HelloWorld::inicializarTeclado()
{ // modificacion
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed = [=](EventKeyboard::KeyCode KeyCode, Event* event) {
        if (KeyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
            moviendoDerecha = true; // Inicia el movimiento en derecha
            //correrDerecha();
        }
        else if (KeyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
            moviendoIzquierda = true; // Inicia el movimiento a la izquierda
        }
        else if (KeyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
            if (!_aventurero->getActionByTag(1)) {
                auto saltar = JumpBy::create(0.5f, Vec2(0, 0), 100, 1);
                saltar->setTag(1);
                _aventurero->runAction(saltar);
            }
        }
        };

    listener->onKeyReleased = [=](EventKeyboard::KeyCode KeyCode, Event* event) {
        if (KeyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
            moviendoDerecha = false; // Detiene el movimiento a la derecha
        }
        else if (KeyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
            moviendoIzquierda = false; // Detiene el movimiento a la izquierda
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // inicializar las variables en false
    moviendoDerecha = false;
    moviendoIzquierda = false;

    // Programa una funcion para mover el jugador cada frame
    schedule(CC_SCHEDULE_SELECTOR(HelloWorld::moverJugador));
}

void HelloWorld::moverJugador(float delta)
{ // modificacion
    float velocidad = 300.0f; // velocidad ajustable

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    float limiteIzquierda = origin.x;
    float limiteDerecha = origin.x + visibleSize.width;

    if (moviendoDerecha) {
        if (_aventurero->getPositionX() + _aventurero->getContentSize().width / 2 < limiteDerecha) {
            _aventurero->setPositionX(_aventurero->getPositionX() + velocidad * delta);
        }
    }

    if (moviendoIzquierda) {
        if (_aventurero->getPositionX() - _aventurero->getContentSize().width / 2 > limiteIzquierda) {
            _aventurero->setPositionX(_aventurero->getPositionX() - velocidad * delta);
        }
    }
}

void HelloWorld::agregarYunkes(float dt) 
{
    auto director = Director::getInstance();
    auto tamanio = director->getWinSize();
    Sprite* yunke = nullptr;

    for (int i = 0; i < 10; ++i) { // cantidad de yunkes
        yunke = Sprite::create("yunke.png");
        yunke->setScale(0.2);
        yunke->setAnchorPoint(Vec2::ZERO);
        yunke->setPosition(CCRANDOM_0_1() * tamanio.width, tamanio.height);
        inicializarFisicas(yunke);
        yunke->getPhysicsBody()->setVelocity(Vec2(0 , (CCRANDOM_0_1() + 0.2f) * -250)); // velocidad de caída de las yunkes
        _yunkes.pushBack(yunke);
        this->addChild(yunke, 1);
    }
}

void HelloWorld::verificarYunkes(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    for (auto it = _yunkes.begin(); it != _yunkes.end();)
    {
        Sprite* yunke = *it;

        // Verificar si el yunke está fuera del área visible
        if (yunke->getPositionY() + yunke->getContentSize().height < origin.y)
        {
            // Eliminar el sprite del nodo y del vector
            this->removeChild(yunke);
            it = _yunkes.erase(it); // Elimina el yunke del vector y avanza el iterador
        }
        else
        {
            ++it; // Avanza el iterador si no se elimina el yunke
        }
    }
}


void HelloWorld::inicializarFisicas(cocos2d::Sprite* sprite) {
    auto cuerpo = PhysicsBody::createCircle(sprite->getContentSize().width / 2); // tamaño del círculo de las físicas
    cuerpo->setContactTestBitmask(true);
    cuerpo->setDynamic(true);
    sprite->setPhysicsBody(cuerpo);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _puntuacion = 0; // Inicializa la puntuación 
    auto spriteFondo = Sprite::create("Fondo.png"); // modificacion
    spriteFondo->setAnchorPoint(Vec2::ZERO); // Vec:: ZERO
    spriteFondo->setPosition(0, 0); // 35, 0
    spriteFondo->setScale(2); // 1.73
    addChild(spriteFondo, 0); // spriteFondo, 0

    _aventurero = Sprite::create("aventurero.png");
    _aventurero->setPosition(Point((visibleSize.width / 2), visibleSize.height * 0.20));
    _aventurero->setScale(3);
    inicializarFisicas(_aventurero);
    addChild(_aventurero, 1);

    inicializarTeclado();
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::agregarYunkes), 5.0f);
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::verificarYunkes), 0.1f); // Verifica los yunkes cada 0.1 segundos


    return true;
}