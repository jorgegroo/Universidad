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
#include "MenuScene.h"
#include "audio/include/AudioEngine.h" // necesario para el sonido

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    // layer is an autorealise object
    auto layer = HelloWorld::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); // dibujar el c�rculo de las f�sicas // modificacion 

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
        yunke->getPhysicsBody()->setVelocity(Vec2(0 , (CCRANDOM_0_1() + 0.2f) * -250)); // velocidad de ca�da de las yunkes
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

        // Verificar si el yunke est� fuera del �rea visible
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
    auto cuerpo = PhysicsBody::createCircle(sprite->getContentSize().width / 2); // tama�o del c�rculo de las f�sicas
    cuerpo->setContactTestBitmask(true);
    cuerpo->setDynamic(true);
    sprite->setPhysicsBody(cuerpo);
}

void HelloWorld::registrarContactos()
{
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [=](PhysicsContact& contact) {
        auto bodyA = contact.getShapeA()->getBody();
        auto bodyB = contact.getShapeB()->getBody();

        if (bodyA == _aventurero->getPhysicsBody() || bodyB == _aventurero->getPhysicsBody()) {
            _vidas--;
            actualizarVidas();

            // Diferir el cambio del cuerpo f�sico
            this->scheduleOnce([=](float) {
                // Desactivar el movimiento din�mico y reposicionar al aventurero
                _aventurero->getPhysicsBody()->setDynamic(false);
                auto visibleSize = Director::getInstance()->getVisibleSize();
                _aventurero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.2f));

                // Reactivar el movimiento din�mico y corregir propiedades f�sicas
                this->scheduleOnce([=](float) {
                    _aventurero->getPhysicsBody()->setDynamic(true);
                    _aventurero->getPhysicsBody()->setGravityEnable(false); // Desactivar gravedad
                    _aventurero->getPhysicsBody()->setVelocity(Vec2::ZERO); // Detener cualquier movimiento
                    }, 0.1f, "ReactivarCuerpoDinamico");

                }, 0.01f, "DetenerCuerpo");

            // Verificar si las vidas llegaron a 0
            if (_vidas <= 0) {
                auto menuScene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
            }

            return true; // Procesar el contacto
        }

        return false; // No procesar el contacto
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void HelloWorld::actualizarVidas() {
    _labelVidas->setString("Vidas: " + std::to_string(_vidas));
}

void HelloWorld::actualizarRecord()
{
    if (_puntuacion > _recordXP) {
        _recordXP = _puntuacion;

        // Guarda el nuevo r�cord de XP en UserDefault
        UserDefault::getInstance()->setIntegerForKey("recordXP", _recordXP);
        UserDefault::getInstance()->flush();

        // Actualiza la etiqueta del r�cord en pantalla
        _labelRecord->setString("Record: " + std::to_string(_recordXP));
    }
}

bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    AudioEngine::stopAll();

    AudioEngine::play2d("playing_music.mp3", true); // true para loop infinito, reproducir m�sica

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _puntuacion = 0; // Inicializa la puntuaci�n 
    _recordXP = UserDefault::getInstance()->getIntegerForKey("recordXP", 0); // Carga el r�cord de XP guardado
    _vidas = 3;      // Inicializar las vidas del jugador

    // Fondo
    auto spriteFondo = Sprite::create("fondo1.png"); // modificacion
    spriteFondo->setAnchorPoint(Vec2::ZERO); // Vec:: ZERO
    spriteFondo->setPosition(0, 0); // 35, 0
    spriteFondo->setScale(2); // 1.73
    addChild(spriteFondo, 0); // spriteFondo, 0

    // Aventurero
    _aventurero = Sprite::create("aventurero.png");
    _aventurero->setPosition(Point((visibleSize.width / 2), visibleSize.height * 0.20));
    _aventurero->setScale(3);
    inicializarFisicas(_aventurero);
    addChild(_aventurero, 1);

    // Etiqueta para las vidas
    _labelVidas = Label::createWithTTF("Vidas: 3", "fonts/Marker Felt.ttf", 14);
    _labelVidas->setPosition((origin.x + visibleSize.width / 2), origin.y + visibleSize.height - 50); // Arriba al centro
    this->addChild(_labelVidas, 1);

    inicializarTeclado();
    registrarContactos(); // Registra el listener para contactos
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::agregarYunkes), 5.0f);
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::verificarYunkes), 0.1f); // Verifica los yunkes cada 0.1 segundos

    _labelPuntuacion = Label::createWithTTF("XP: 0", "fonts/Marker Felt.ttf", 14);
    _labelPuntuacion->setPosition((origin.x + visibleSize.width / 2), 300); // Posici�n arriba al centro
    this->addChild(_labelPuntuacion, 1);

    // Etiqueta para mostrar el r�cord de XP
    _labelRecord = Label::createWithTTF("Record: " + std::to_string(_recordXP), "fonts/Marker Felt.ttf", 14);
    _labelRecord->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height - 80));
    this->addChild(_labelRecord, 1);

    // Programa un m�todo para aumentar la puntuaci�n constantemente
    this->schedule([=](float dt) {
        _puntuacion++;
        _labelPuntuacion->setString("XP: " + std::to_string(_puntuacion));
        actualizarRecord();
        }, 1.0f, "ActualizarPuntuacion");

    return true;
}