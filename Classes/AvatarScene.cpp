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

#include "AvatarScene.h"
#include "TiendaScene.h" // Para regresar a la tienda
#include "audio/include/AudioEngine.h" // Para manejar el sonido si es necesario


USING_NS_CC;

Scene* AvatarScene::createScene()
{
    return AvatarScene::create();
}

bool AvatarScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    AudioEngine::stopAll();

    AudioEngine::play2d("MusicaTienda.mp3", true); // true para loop infinito, reproducir música

    // Fondo de la escena
    auto spriteFondo = Sprite::create("TiendaImage.png"); // Cambia por tu imagen de fondo
    if (spriteFondo != nullptr) {
        spriteFondo->setAnchorPoint(Vec2::ZERO);
        spriteFondo->setPosition(origin);
        spriteFondo->setScale(visibleSize.width / spriteFondo->getContentSize().width,
            visibleSize.height / spriteFondo->getContentSize().height);
        this->addChild(spriteFondo, 0);
    }

    // Botón para regresar a la tienda
    auto backButton = MenuItemImage::create(
        "BotonRegresar1.png",  // Imagen normal
        "BotonRegresar2.png", // Imagen seleccionada
        [](Ref* sender) {    // Callback para regresar a la tienda
            auto tiendaScene = TiendaScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, tiendaScene));
        });

    if (backButton != nullptr) {
        backButton->setPosition(Vec2(origin.x + 50, origin.y + visibleSize.height - 50)); // Esquina superior izquierda
        backButton->setScale(2); // Ajusta la escala
    }

    auto menu = Menu::create(backButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}