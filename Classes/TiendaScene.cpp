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

#include "TiendaScene.h"
#include "AvatarScene.h" // Para transicionar a la escena de Avatar
#include "FondoScene.h"  // Para transicionar a la escena de Fondo
#include "MenuScene.h"   // Para regresar al menú principal
#include "audio/include/AudioEngine.h" // Para manejar el sonido si es necesario


USING_NS_CC;

Scene* TiendaScene::createScene()
{
    return TiendaScene::create();
}

bool TiendaScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    AudioEngine::stopAll();

    AudioEngine::play2d("MusicaTienda.mp3", true); // true para loop infinito, reproducir música

    // Fondo de la tienda
    auto spriteFondoTienda = Sprite::create("TiendaImage.png");
    if (spriteFondoTienda != nullptr) {
        spriteFondoTienda->setAnchorPoint(Vec2::ZERO);
        spriteFondoTienda->setPosition(origin);
        spriteFondoTienda->setScale(visibleSize.width / spriteFondoTienda->getContentSize().width,
            visibleSize.height / spriteFondoTienda->getContentSize().height);
        this->addChild(spriteFondoTienda, 0);
    }

    // Botón "Avatar"
    auto avatarButton = MenuItemImage::create(
        "BotonJugar1.png",  // Imagen normal
        "BotonJugar2.png", // Imagen seleccionada
        [](Ref* sender) {    // Callback para ir a la escena de Avatar
            auto avatarScene = AvatarScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, avatarScene));
        });

    if (avatarButton != nullptr) {
        avatarButton->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, visibleSize.height / 2));
        avatarButton->setScale(4);
    }

    // Etiqueta "Avatar"
    auto labelAvatar = Label::createWithTTF("Avatar", "fonts/Marker Felt.ttf", 20);
    if (labelAvatar != nullptr) {
        labelAvatar->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, visibleSize.height / 2 + 60));
        labelAvatar->setTextColor(Color4B::WHITE);
        this->addChild(labelAvatar, 1);
    }

    // Botón "Fondo"
    auto fondoButton = MenuItemImage::create(
        "BotonJugar1.png",  // Imagen normal
        "BotonJugar2.png", // Imagen seleccionada
        [](Ref* sender) {    // Callback para ir a la escena de Fondo
            auto fondoScene = FondoScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, fondoScene));
        });

    if (fondoButton != nullptr) {
        fondoButton->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, visibleSize.height / 2));
        fondoButton->setScale(4);
    }

    // Etiqueta "Fondo"
    auto labelFondo = Label::createWithTTF("Fondo", "fonts/Marker Felt.ttf", 20);
    if (labelFondo != nullptr) {
        labelFondo->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, visibleSize.height / 2 + 60));
        labelFondo->setTextColor(Color4B::WHITE);
        this->addChild(labelFondo, 1);
    }

    // Botón para regresar al menú
    auto backButton = MenuItemImage::create(
        "BotonRegresar1.png",  // Imagen normal
        "BotonRegresar2.png", // Imagen seleccionada
        [](Ref* sender) {    // Callback para regresar al menú principal
            auto menuScene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
        });

    if (backButton != nullptr) {
        backButton->setPosition(Vec2(origin.x + 50, origin.y + visibleSize.height - 50)); // Esquina superior izquierda
        backButton->setScale(2);
    }

    // Crear el menú con los botones
    auto menu = Menu::create(avatarButton, fondoButton, backButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}
    