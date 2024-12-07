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

#include "MenuScene.h"
#include "HelloWorldScene.h"
#include "gameOverScene.h"
#include "audio/include/AudioEngine.h" // necesario para el sonido

USING_NS_CC;

Scene* gameOverScene::createScene()
{
    auto scene = Scene::create();
    

    // layer is an autorealise object
    auto layer = gameOverScene::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); // dibujar el círculo de las físicas // modificacion 

    // add layer as child to scene
    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in gameOverScene.cpp\n");
}

// on "init" you need to initialize your instance
bool gameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    AudioEngine::play2d("playing_music.mp3", true); // true para loop infinito, reproducir música

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto spriteFondoMenu = Sprite::create("MenuImage.jpg"); // modificacion
    spriteFondoMenu->setAnchorPoint(Vec2::ZERO); // Vec:: ZERO
    spriteFondoMenu->setPosition(0, 0); // 35, 0
    spriteFondoMenu->setScaleX(3.5); // 1.73
    spriteFondoMenu->setScaleY(4);
    addChild(spriteFondoMenu, 0); // spriteFondo, 0

    auto spriteTexto = Sprite::create("Nube.png"); // Reemplaza con el nombre de tu archivo de imagen
    if (spriteTexto == nullptr)
    {
        problemLoading("'Nube.png'");
    }
    else
    {
        // Configura la posición del sprite en el centro de la pantalla
        spriteTexto->setPosition(origin.x + visibleSize.width / 2, 275);

        // Escalar el sprite si es necesario (opcional)
        spriteTexto->setScale(1.5); // Ajusta el valor según necesites

        // Añadir el sprite como hijo de la escena
        this->addChild(spriteTexto, 1);
    }

    // Crear un botón usando un sprite
    auto closeButton = MenuItemImage::create(
        "BotonCerrar1.png",  // Imagen para el estado normal
        "BotonCerrar2.png", // Imagen para el estado seleccionado
        [](Ref* sender) {    // Callback para la acción del botón
            Director::getInstance()->end(); // Cierra la aplicación
        });

    if (closeButton == nullptr)
    {
        problemLoading("'BotonCerrar1.png' or 'BotonCerrar2.png'");
    }
    else
    {
        closeButton->setPosition((origin.x + visibleSize.width / 2) - 30, 125);
        closeButton->setScale(4);

        // Crear el menú y añadir el botón
        auto menu = Menu::create(closeButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1); // Añadir el menú a la escena
    }

    auto labelSalir = Label::createWithTTF("Salir", "fonts/Marker Felt.ttf", 20); // Texto, fuente, tamaño
    if (labelSalir == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // Posición del texto en el centro de la pantalla
        labelSalir->setPosition((origin.x + visibleSize.width / 2) - 30, 150);

        // Color del texto
        labelSalir->setTextColor(Color4B::WHITE);

        // Añade el texto como hijo del menú
        this->addChild(labelSalir, 1);
    }

    // Crear un botón usando un sprite
    auto playButton = MenuItemImage::create(
        "BotonJugar1.png",  // Imagen para el estado normal
        "BotonJugar2.png", // Imagen para el estado seleccionado
        [](Ref* sender) {    // Callback para la acción del botón
            auto newScene = HelloWorld::createScene(); // Crea la nueva escena 
            Director::getInstance()->replaceScene(TransitionFade::create(1.0, newScene)); // Transición opcional
        });

    if (playButton == nullptr)
    {
        problemLoading("'BotonJugar1.png' or 'BotonJugar2.png'");
    }
    else
    {
        playButton->setPosition((origin.x + visibleSize.width / 2) + 30, 125);
        playButton->setScale(4);

        // Crear el menú y añadir el botón
        auto menu = Menu::create(playButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1); // Añadir el menú a la escena
    }

    auto labelJugar = Label::createWithTTF("Jugar", "fonts/Marker Felt.ttf", 20); // Texto, fuente, tamaño
    if (labelJugar == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // Posición del texto en el centro de la pantalla
        labelJugar->setPosition((origin.x + visibleSize.width / 2) + 30, 150);

        // Color del texto
        labelJugar->setTextColor(Color4B::WHITE);

        // Añade el texto como hijo del menú
        this->addChild(labelJugar, 1);
    }

    return true;
}