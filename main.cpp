#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdc++.h>
using namespace sf;
using namespace std;

class Player { 
public:
    float x, y, w, h, dx, dy, speed; 
    int dir;
    FloatRect getRect() 
    {
        return FloatRect(x, y, w, h);
    }
    bool isShoot = false;
    int health = 100;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;
    Player(String F, float X, float Y, float W, float H) {
        dx = 0; dy = 0; speed = 0; dir = 0;
        File = F;
        w = W; h = H;
        image.loadFromFile("images/" + File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
    void update(float time)
    {
        switch (dir)
        {
        case 0: dx = speed; dy = 0; break;
        case 1: dx = -speed; dy = 0; break;
        }

        x += dx * time;
        y += dy * time;

        speed = 0;
        sprite.setPosition(x, y);
    }
};

class Marine {
public:
    float x, y, w, h, dx, dy, speed, moveTimer;
    bool Side;
    float shootx = 0;
    FloatRect getRect() {
        return FloatRect(x, y, w, h);
    }
    int dir = 1, health = 100;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;
    String name;
    Marine(String F, float X, float Y, float W, float H) {
        dx = 0; dy = 0; speed = 0; dir = 0;
        moveTimer = 0;
        Side = true;
        name = "Marine";
        File = F;
        w = W; h = H;
        image.loadFromFile("images/" + File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        shootx = X;
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
    void update(float time)
    {
        if (Side == true) { dx = 0.02; dy = 0; }
        if (Side == false) { dx = -0.02; dy = 0; }
        moveTimer += time;
        if (moveTimer > 28700) { y += 42; if (Side == true) { Side = false; } else if (Side == false) { Side = true; } moveTimer = 0; }

        x += dx * time;
        y += dy * time;

        shootx += abs(dx * time);

        speed = 0;
        sprite.setPosition(x, y); 
    }
};

class Bullet {
public:
    float x, y, w, h, dx, dy, speed;
    int health = 100;
    FloatRect getRect() {
        return FloatRect(x, y, w, h);
    }
    bool dir;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;
    String name;
    Bullet(float X, float Y, bool d) {
        dx = 0; dy = 0; speed = 0; dir = 0;
        name = "Bullet";
        File = "Bullet.png";
        w = 6;
        h = 20;
        dir = d;
        image.loadFromFile("images/" + File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X; y = Y;
        sprite.setTextureRect(IntRect(0, 0, w, h));
    }
    void update(float time)
    {
        dx = 0; dy = 0.2;
        if (dir == true) y -= dy * time;
        if (dir == false) y += dy * time;
        x += dx * time;

        if (y <= 0) { health = 0; }
        if (y >= 600) { health = 0; }
        sprite.setPosition(x, y); 
    }
};


bool startGame() 
{
    RenderWindow window(VideoMode(900, 600), "Pirate Invaders");

    Player p("Player.png", 450, 520, 32, 32);

    list<Marine*> enemies1;
    list<Marine*>::iterator it;

    for (int i = 0; i <= 6; i++) {
        int k = 0, j = 0;
        enemies1.push_back(new Marine("Marine.png", -5 + (i * 50), 100, 32, 32));
    }

    list<Marine*> enemies2;
    list<Marine*>::iterator it2;

    
    for (int i = 0; i <= 6; i++) 
    {
        int k = 0, j = 0;
        enemies2.push_back(new Marine("Marine.png", -5 + (i * 50), 142, 32, 32));
    }

    list<Marine*> enemies3;
    list<Marine*>::iterator it3;

    for (int i = 0; i <= 6; i++) {
        int k = 0, j = 0;
        enemies3.push_back(new Marine("Marine.png", -5 + (i * 50), 184, 32, 32));
    }

    list<Bullet*> bullets;
    list<Bullet*>::iterator it4;

    list<Bullet*> ebullets;
    list<Bullet*>::iterator it5;

    float CurrentFrame = 0;
    Clock clock;
    float shootTimer = 0;
    float Randomtimer = 0;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        Event event;
        shootTimer += time;
        Randomtimer += time;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (p.isShoot == true) { p.isShoot = false; bullets.push_back(new Bullet(p.x, p.y, true)); }

            for (it = enemies1.begin(); it != enemies1.end(); it++)
            {
                if ((*it)->shootx > 26) { (*it)->shootx = 0; ebullets.push_back(new Bullet((*it)->x + 14, (*it)->y, false)); }
            }

            for (it2 = enemies2.begin(); it2 != enemies2.end(); it2++)
            {
                if ((*it2)->shootx > 45) { (*it2)->shootx = 0; ebullets.push_back(new Bullet((*it2)->x + 14, (*it2)->y, false)); }
            }

            for (it3 = enemies3.begin(); it3 != enemies3.end(); it3++)
            {
                if ((*it3)->shootx > 70) { (*it3)->shootx = 0; ebullets.push_back(new Bullet((*it3)->x + 14, (*it3)->y, false)); }
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }
        if ((Keyboard::isKeyPressed(Keyboard::A)) || (Keyboard::isKeyPressed(Keyboard::Left))) 
        {
            p.dir = 1; p.speed = 0.25;
            if (p.x < 0) p.x = 0;
        }
        if ((Keyboard::isKeyPressed(Keyboard::D)) || (Keyboard::isKeyPressed(Keyboard::Right)))
        {
            p.dir = 1; p.speed = -0.25;
            if (p.x > 868) p.x = 868;
        }
        if ((Keyboard::isKeyPressed(Keyboard::Space)))
        {
            p.isShoot = true;
        }
        p.update(time);

        for (it = enemies1.begin(); it != enemies1.end();)
        {
            Marine* b = *it;
            b->update(time);
            if (b->health <= 0) { it = enemies1.erase(it); delete b; }
            else it++;
        }
        for (it2 = enemies2.begin(); it2 != enemies2.end();)
        {
            Marine* b = *it2;
            b->update(time);
            if (b->health <= 0) { it2 = enemies2.erase(it2); delete b; }
            else it2++;
        }
        for (it3 = enemies3.begin(); it3 != enemies3.end();)
        {
            Marine* b = *it3;
            b->update(time);
            if (b->health <= 0) { it3 = enemies3.erase(it3); delete b; }
            else it3++;
        }

        for (it4 = bullets.begin(); it4 != bullets.end();)
        {
            Bullet* b = *it4;
            b->update(time);
            if (b->health <= 0) { it4 = bullets.erase(it4); delete b; }
            else it4++;
        }

        for (it5 = ebullets.begin(); it5 != ebullets.end();)
        {
            Bullet* b = *it5;
            b->update(time);
            if (b->health <= 0) { it5 = ebullets.erase(it5); delete b; }
            else it5++;
        }

        for (it = enemies1.begin(); it != enemies1.end(); it++)
        {
            if ((*it)->getRect().intersects(p.getRect()))
            {
                p.health = 0;
                window.close();
            }
            for (it4 = bullets.begin(); it4 != bullets.end(); it4++)
            {
                if ((*it)->getRect().intersects((*it4)->getRect()))
                {
                    (*it)->health = 0;
                    (*it4)->health = 0;
                }
            }
        }

        for (it2 = enemies2.begin(); it2 != enemies2.end(); it2++)
        {
            if ((*it2)->getRect().intersects(p.getRect()))
            {
                p.health = 0;
                window.close();
            }
            for (it4 = bullets.begin(); it4 != bullets.end(); it4++)
            {
                if ((*it2)->getRect().intersects((*it4)->getRect()))
                {
                    (*it2)->health = 0;
                    (*it4)->health = 0;
                }
            }
        }

        for (it3 = enemies3.begin(); it3 != enemies3.end(); it3++)
        {
            if ((*it3)->getRect().intersects(p.getRect()))
            {
                p.health = 0;
                window.close();
            }
            for (it4 = bullets.begin(); it4 != bullets.end(); it4++)
            {
                if ((*it3)->getRect().intersects((*it4)->getRect()))
                {
                    (*it3)->health = 0;
                    (*it4)->health = 0;
                }
            }
        }
        for (it5 = ebullets.begin(); it5 != ebullets.end(); it5++)
        {
            if ((*it5)->getRect().intersects(p.getRect()))
            {
                p.health = 0;
                window.close();
            }
        }

        for (it = enemies1.begin(); it != enemies1.end(); it++) { (*it)->update(time); }
        for (it2 = enemies2.begin(); it2 != enemies2.end(); it2++) { (*it2)->update(time); }
        for (it3 = enemies3.begin(); it3 != enemies3.end(); it3++) { (*it3)->update(time); }

        for (it4 = bullets.begin(); it4 != bullets.end(); it4++) { (*it4)->update(time); }

        for (it5 = ebullets.begin(); it5 != ebullets.end(); it5++) { (*it5)->update(time); }
        window.clear();

        window.draw(p.sprite);

        for (it = enemies1.begin(); it != enemies1.end(); it++) { window.draw((*it)->sprite); }
        for (it2 = enemies2.begin(); it2 != enemies2.end(); it2++) { window.draw((*it2)->sprite); }
        for (it3 = enemies3.begin(); it3 != enemies3.end(); it3++) { window.draw((*it3)->sprite); }

        for (it4 = bullets.begin(); it4 != bullets.end(); it4++) { window.draw((*it4)->sprite); }

        for (it5 = ebullets.begin(); it5 != ebullets.end(); it5++) { window.draw((*it5)->sprite); }

        window.display();
    }
}

void gameRunning() {
    if (startGame()) { gameRunning(); }
}

int main()
{
    gameRunning();
    return 0;
}