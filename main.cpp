#include <cstdio>
#include <cmath>
#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <imgui/imgui.h>
#include "utils.h"

int main(int argc, char const *argv[]) {
    int i;
    float x1, x2, y1, y2;
    float lastM1 = 1, lastM2 = 2, lastTau=0.001;
    float inM1 = 1, inM2 = 2, inTau = 0.001;
    Mayatnik mayatnik = Mayatnik(1, 1, 2, 0.001);
    MData Z = {1.72, 1.72, 0, 0};
    auto next_z = new MData;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Double Pendulum");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape circle1;
    sf::CircleShape circle2;
    sf::VertexArray line1(sf::LineStrip, 2);
    sf::VertexArray line2(sf::LineStrip, 2);
    line1[0].position = sf::Vector2f(400, 300);


    circle1.setRadius(10);
    circle1.setOutlineColor(sf::Color::Red);
    circle1.setOutlineThickness(3);
    circle2.setRadius(10);
    circle2.setOutlineColor(sf::Color::Blue);
    circle2.setOutlineThickness(3);


    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        mayatnik.next_Z(&Z, next_z);

        window.clear(sf::Color::Black);

        x1 = cosf(static_cast<float>(next_z->alpha1 + M_PI_2)) * 100 + 400;
        y1 = sinf(static_cast<float>(next_z->alpha1 + M_PI_2)) * 100 + 300;

        x2 = cosf(static_cast<float>(next_z->alpha2 + M_PI_2)) * 100 + x1;
        y2 = sinf(static_cast<float>(next_z->alpha2 + M_PI_2)) * 100 + y1;
        line1[1].position = sf::Vector2f(x1, y1);
        line2[0].position = sf::Vector2f(x1, y1);
        line2[1].position = sf::Vector2f(x2, y2);

        circle1.setPosition(x1-10, y1-10);
        circle2.setPosition(x2-10, y2-10);

        //printf("%lf %lf %lf %lf\n", next_z->alpha1, next_z->alpha2, next_z->p1, next_z->p2);

        if (ImGui::Begin("Params")) {
            if (ImGui::DragFloat("m1", &inM1, 0.01f)) {
                if (inM1 != lastM1) {
                    mayatnik.setM1(inM1);
                    lastM1 = inM1;
                    *next_z = {1.72, 1.72, 0, 0};
                }
            }
            if (ImGui::DragFloat("m2", &inM2, 0.01f)) {
                if (inM2 != lastM2) {
                    mayatnik.setM2(inM2);
                    lastM2 = inM2;
                    *next_z = {1.72, 1.72, 0, 0};
                }
            }
            if (ImGui::DragFloat("tau", &inTau, 0.001f)) {
                if (inTau != lastTau) {
                    mayatnik.setTau(inTau);
                    lastTau = inTau;
                    *next_z = {1.72, 1.72, 0, 0};
                }
            }
            ImGui::End();
        }


        window.draw(line1);
        window.draw(line2);
        window.draw(circle1);
        window.draw(circle2);
        setMData(next_z, &Z);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    delete next_z;
    return 0;
}