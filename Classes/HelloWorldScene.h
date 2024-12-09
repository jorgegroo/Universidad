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

#ifndef __HELLO_WORLD_SCENE_H__
#define __HELLO_WORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    // Crea y retorna la escena principal
    static cocos2d::Scene* createScene();

    // Método de inicialización
    virtual bool init();

    // Implementa el método "create" manualmente
    CREATE_FUNC(HelloWorld);

private:
    // Métodos privados
    void inicializarTeclado();                     // Inicializa el control del teclado
    void moverJugador(float delta);                // Movimiento continuo del jugador
    void agregarYunkes(float dt);                  // Agrega yunques a la escena
    void verificarYunkes(float dt);                // Verifica y elimina yunques fuera de pantalla
    void inicializarFisicas(cocos2d::Sprite* sprite); // Agrega físicas a un sprite
    void registrarContactos();                     // Registra las colisiones físicas
    void actualizarRecord();                       // Método para actualizar el récord si se supera

    // Variables privadas
    cocos2d::Sprite* _aventurero;                 // Referencia al sprite del aventurero
    cocos2d::Vector<cocos2d::Sprite*> _yunkes;    // Lista de yunques en la escena
    bool moviendoDerecha;                         // Estado de movimiento a la derecha
    bool moviendoIzquierda;                       // Estado de movimiento a la izquierda
    int _puntuacion;                              // Puntuación del jugador
    cocos2d::Label* _labelPuntuacion;             // Etiqueta para mostrar la puntuación
    int _recordXP;                                // Récord de XP
    cocos2d::Label* _labelRecord;                 // Etiqueta para mostrar el récord de XP

    int _vidas;                               // Vidas restantes del aventurero
    cocos2d::Label* _labelVidas;              // Etiqueta para mostrar las vidas en pantalla
    void actualizarVidas();                   // Método para actualizar la etiqueta de vida
};

#endif // __HELLO_WORLD_SCENE_H__