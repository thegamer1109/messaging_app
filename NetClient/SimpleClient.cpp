#include <iostream>
#include <olc_net.h>

enum class CustomMsgTypes : uint32_t {
    FireBullet,
    MovePlayer
};

int main() {
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::FireBullet;

    int a = 1;
    bool b = true;
    float c = 3.14159f;

    struct {
        float x;
        float y;
    } d[5];

    msg << a << b << c << d;

    a = 99;
    b = false;
    c = 100.01f;

    msg >> d >> c >> b >> a;

    return 0;
}