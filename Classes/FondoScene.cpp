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

#include "FondoScene.h"
#include "TiendaScene.h"
#include "audio/include/AudioEngine.h"
#include "HelloWorldScene.h" // Para obtener el récord de XP almacenado

USING_NS_CC;

int fondoEquipado = 1; // Fondo actualmente equipado
bool fondosComprados[4] = { true, false, false, false }; // Fondo 1 está comprado por defecto

Scene* FondoScene::createScene()
{
    return FondoScene::create();
}

bool FondoScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    AudioEngine::stopAll();
    AudioEngine::play2d("MusicaTienda.mp3", true);

    // Fondo de la escena
    auto spriteFondo = Sprite::create("TiendaImage.png");
    if (spriteFondo != nullptr) {
        spriteFondo->setAnchorPoint(Vec2::ZERO);
        spriteFondo->setPosition(origin);
        spriteFondo->setScale(visibleSize.width / spriteFondo->getContentSize().width,
            visibleSize.height / spriteFondo->getContentSize().height);
        this->addChild(spriteFondo, 0);
    }

    // Cargar el récord de XP
    int recordXP = UserDefault::getInstance()->getIntegerForKey("recordXP", 0);

    // Crear los fondos con sus labels y botones
    for (int i = 0; i < 4; i++) {
        std::string nombreFondo = "fondo" + std::to_string(i + 1) + ".png";
        auto fondoSprite = Sprite::create(nombreFondo);

        if (fondoSprite) {
            // Posición del fondo
            fondoSprite->setScale(0.5);
            fondoSprite->setPosition(Vec2(origin.x + (visibleSize.width / 5) * (i + 1), visibleSize.height * 0.6));
            this->addChild(fondoSprite, 1);

            // Label para mostrar el estado del fondo
            std::string estado;
            if (i == 0) {
                estado = "Equipado"; // Fondo 1 por defecto está equipado
            }
            else if (!fondosComprados[i]) {
                if (i == 1) estado = "Record > 10 XP";
                if (i == 2) estado = "Record > 50 XP";
                if (i == 3) estado = "Record > 100 XP";
            }
            else {
                estado = "Comprado";
            }

            auto labelEstado = Label::createWithTTF(estado, "fonts/Marker Felt.ttf", 14);
            labelEstado->setPosition(Vec2(fondoSprite->getPositionX(), fondoSprite->getPositionY() - 60));
            this->addChild(labelEstado, 1);

            // Botón para comprar/seleccionar el fondo
            auto button = MenuItemImage::create(
                "BotonJugar1.png", "BotonJugar2.png",
                [=](Ref* sender) {
                    if (!fondosComprados[i]) { // Si el fondo no está comprado
                        if ((i == 1 && recordXP >= 10) || (i == 2 && recordXP >= 50) || (i == 3 && recordXP >= 100)) {
                            fondosComprados[i] = true; // Comprar fondo
                            labelEstado->setString("Comprado");
                        }
                        else {
                            AudioEngine::play2d("error_sound.mp3"); // Reproduce un sonido de error
                        }
                    }
                    else { // Si el fondo ya está comprado, seleccionarlo
                        fondoEquipado = i + 1;
                        labelEstado->setString("Equipado");
                        actualizarLabels(i);
                    }
                });

            button->setScale(3);
            button->setPosition(Vec2(fondoSprite->getPositionX(), fondoSprite->getPositionY() - 100));
            auto menu = Menu::create(button, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 1);
        }
    }

    // Botón para regresar a la tienda
    auto backButton = MenuItemImage::create(
        "BotonRegresar1.png", "BotonRegresar2.png",
        [](Ref* sender) {
            auto tiendaScene = TiendaScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, tiendaScene));
        });

    if (backButton != nullptr) {
        backButton->setPosition(Vec2(origin.x + 50, origin.y + visibleSize.height - 50));
        backButton->setScale(2);
        auto menuBack = Menu::create(backButton, nullptr);
        menuBack->setPosition(Vec2::ZERO);
        this->addChild(menuBack, 1);
    }

    return true;
}

// Método para actualizar los labels de los fondos
void FondoScene::actualizarLabels(int fondoSeleccionado)
{
    for (int i = 0; i < 4; i++) {
        if (i == fondoSeleccionado) continue;

        // Buscar el label de cada fondo y actualizarlo
        auto labelEstado = dynamic_cast<Label*>(this->getChildByTag(100 + i));
        if (labelEstado) {
            if (fondosComprados[i]) {
                labelEstado->setString("Comprado");
            }
        }
    }
}