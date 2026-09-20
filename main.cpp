#include "raylib.h"
#include <cmath>
#include <iostream>

struct DroneState
{
    float x;
    float y;
    float z;
    float speed;
    float vx;
    float vy;
    float vz;
};

struct Tree
{
    float x;
    float z;
    float height;
};

struct Building
{
    float x;
    float z;
    float width;
    float depth;
    float height;
    Color color;
};

struct Cloud {
    // add some cloudy charm!! blue skies are boring.
    float x;
    float y;
    float z;
    float scale;
};

// Helper functions that place objects in the world!
void placeTree(float x, float z, float height)
{
    DrawCylinder({x, 0.0f, z}, 0.2f, 0.2f, height, 6, BROWN);
    DrawCylinder({x, height, z}, 0.0f, 1.1f, 1.2f, 8, DARKGREEN);
    DrawCylinder({x, height + 0.7f, z}, 0.0f, 0.85f, 1.0f, 8, GREEN);
    DrawCylinder({x, height + 1.3f, z}, 0.0f, 0.5f, 0.8f, 8, LIME);
}
void placeWaterTower(float x, float z)
{
    float legH = 6.0f;
    float spread = 1.2f;

    DrawCylinder({x - spread, 0.0f, z - spread}, 0.15f, 0.15f, legH, 6, GRAY);
    DrawCylinder({x + spread, 0.0f, z - spread}, 0.15f, 0.15f, legH, 6, GRAY);
    DrawCylinder({x - spread, 0.0f, z + spread}, 0.15f, 0.15f, legH, 6, GRAY);
    DrawCylinder({x + spread, 0.0f, z + spread}, 0.15f, 0.15f, legH, 6, GRAY);
    DrawCylinder({x, legH, z}, 2.0f, 1.6f, 3.0f, 12, BLUE);
    DrawCylinder({x, legH + 3.0f, z}, 1.6f, 0.0f, 1.0f, 12, DARKBLUE);
}
void placeRadioTower(float x, float z)
{
    DrawCylinder({x, 0.0f, z}, 0.3f, 0.05f, 18.0f, 6, LIGHTGRAY);
    for (float h = 2.0f; h < 16.0f; h += 3.0f)
    {
        float r = 0.25f * (1.0f - h / 18.0f) + 0.05f;
        DrawCylinder({x, h, z}, r + 0.8f, r, 0.15f, 6, GRAY);
    }
    DrawSphere({x, 18.2f, z}, 0.2f, RED);
}
void placeBarn(float x, float z)
{
    DrawCube({x, 1.5f, z}, 6.0f, 3.0f, 4.0f, RED);
    DrawCube({x, 3.2f, z}, 6.2f, 1.2f, 4.2f, DARKBROWN);
    // doors
    DrawCube({x, 0.75f, z + 2.01f}, 1.5f, 1.5f, 0.1f, BROWN);
}
void placeHouse(float x, float z, Color wallColor)
{
    DrawCube({x, 1.0f, z}, 3.0f, 2.0f, 3.0f, wallColor);
    DrawCube({x, 2.3f, z}, 3.2f, 0.8f, 3.2f, BROWN);
    DrawCube({x, 0.5f, z + 1.51f}, 0.8f, 1.0f, 0.1f, DARKBROWN);
}
void placeCloud(float x, float y, float z, float scale)
{
    Color cloudColor = {255, 255, 255, 200}; // slightly transparent white
    DrawSphere({x,            y,            z           }, 2.0f * scale, cloudColor);
    DrawSphere({x + 2.2f*scale, y,          z           }, 1.6f * scale, cloudColor);
    DrawSphere({x - 2.0f*scale, y,          z           }, 1.5f * scale, cloudColor);
    DrawSphere({x + 1.0f*scale, y + 1.0f*scale, z       }, 1.4f * scale, cloudColor);
    DrawSphere({x - 0.8f*scale, y + 0.9f*scale, z       }, 1.3f * scale, cloudColor);
    DrawSphere({x,              y,           z + 1.5f*scale}, 1.4f * scale, cloudColor);
    DrawSphere({x,              y,           z - 1.4f*scale}, 1.3f * scale, cloudColor);
}

int main()
{
    InitWindow(800, 600, "Drone Simulator 3D");
    SetTargetFPS(60);

    DroneState drone{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    Model droneModel = LoadModel("drone.obj"); // Load a 3D model for the drone if you'd like!

    // Camera setup
    Camera3D camera = {};
    camera.position = {0.0f, 5.0f, 10.0f};
    camera.target = {drone.x, drone.y, drone.z};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;


    // Defining some trees. These are hardcoded for now.
    Tree trees[] = {
        // These trees have scattered coordinates, so they'll appear across the map.
        {8, 6, 2.5f},{9, 8, 2.0f},{10, 5, 3.0f},{7, 10, 2.2f},{-8, 6, 2.8f},
        {-9, 8, 2.3f},{-10, 5, 2.0f},{-7, 10, 2.6f},{15, 30, 3.0f},{18, 28, 2.5f},
        {20, 32, 3.2f},{13, 35, 2.8f},{22, 27, 2.0f},{16, 33, 3.5f},{19, 36, 2.2f},
        {12, 29, 2.7f},{25, 31, 3.1f},{23, 34, 2.4f},{17, 38, 2.9f},{21, 40, 3.0f},
        {35, 10, 2.8f},{38, 8, 3.0f},{40, 12, 2.5f},{36, 15, 3.2f},{42, 7, 2.2f},
        {39, 14, 2.7f},{37, 18, 3.1f},{41, 20, 2.4f},{-35, 10, 2.6f},{-38, 8, 3.0f},
        {-40, 12, 2.8f},{-36, 15, 2.3f},{-42, 7, 3.2f},{-39, 14, 2.5f},{-37, 18, 2.0f},
        {-41, 20, 2.9f},{10, -20, 2.4f},{-10, -22, 2.8f},{5, -30, 3.0f},{-15, -18, 2.2f},
        {20, -25, 2.6f},{-20, -28, 3.1f},{-2, 15, 2.0f},{2, 18, 2.0f},{-2, 21, 2.0f},
        {2, 24, 2.0f},{-2, 27, 2.0f},{2, 30, 2.0f},
    };
    int treeCount = sizeof(trees) / sizeof(trees[0]);

    Cloud clouds[] = {
    { 10.0f, 18.0f,  5.0f,  1.0f},{-15.0f, 22.0f, 10.0f,  1.3f},{ 30.0f, 20.0f, -8.0f,  0.9f},
    {-25.0f, 17.0f, 20.0f,  1.1f},{  5.0f, 25.0f,-20.0f,  1.4f},{ 20.0f, 19.0f, 30.0f,  1.0f},
    {-10.0f, 23.0f,-30.0f,  1.2f},{-40.0f, 21.0f,  0.0f,  1.5f},{ 45.0f, 18.0f, 15.0f,  0.8f},{  0.0f, 26.0f, 40.0f,  1.3f},
    };
    int cloudCount = sizeof(clouds) / sizeof(clouds[0]);

    // Operation loop
    while (!WindowShouldClose())
    {
        float thrust = 25.0f;
        float drag = 0.95f; // Drag force
        float dt = GetFrameTime();
        float camDistance = 10.0f;
        static float camAngleX = 0.0f;
        static float camAngleY = 0.0f;

        float forwardX = -sinf(camAngleX);
        float forwardZ = -cosf(camAngleX);
        float rightX = cosf(camAngleX);
        float rightZ = -sinf(camAngleX);

        // Rotation logic, using WASD because gamer
        if (IsKeyDown(KEY_A))
        {
            // Boost while holding shift!
            if(IsKeyDown(KEY_LEFT_SHIFT))
            {
                drone.vx -= rightX * thrust * 1.5f * dt;
                drone.vz -= rightZ * thrust * 1.5f * dt;
            }
            drone.vx -= rightX * thrust * dt;
            drone.vz -= rightZ * thrust * dt;
        }
        if (IsKeyDown(KEY_D))
        {
            if(IsKeyDown(KEY_LEFT_SHIFT))
            {
                drone.vx += rightX * thrust * 1.5f * dt;
                drone.vz += rightZ * thrust * 1.5f * dt;
            }
            drone.vx += rightX * thrust * dt;
            drone.vz += rightZ * thrust * dt;
        }
        if (IsKeyDown(KEY_LEFT_CONTROL))
        {
            drone.vy -= thrust * dt;
        }
        if (IsKeyDown(KEY_SPACE))
        {
            drone.vy += thrust * dt;
        }
        if (IsKeyDown(KEY_S))
        {
            if(IsKeyDown(KEY_LEFT_SHIFT))
            {
                drone.vx -= forwardX * thrust * 1.5f * dt;
                drone.vz -= forwardZ * thrust * 1.5f * dt;
            }
            drone.vx -= forwardX * thrust * dt;
            drone.vz -= forwardZ * thrust * dt;
        }
        if (IsKeyDown(KEY_W))
        {
            if(IsKeyDown(KEY_LEFT_SHIFT))
            {
                drone.vx += forwardX * thrust * 1.5f * dt;
                drone.vz += forwardZ * thrust * 1.5f * dt;
            }
            drone.vx += forwardX * thrust * dt;
            drone.vz += forwardZ * thrust * dt;
        } 

        // Update position based on variables
        drone.vx *= drag;
        drone.vy *= drag;
        drone.vz *= drag;
        drone.x += drone.vx * dt;
        drone.y += drone.vy * dt;
        drone.z += drone.vz * dt;

        if (drone.y < 0.0f)
        {
            drone.y = 0.0f;
            drone.vy = 0.0f; // Kill vertical velocity so it doesn't keep pushing down
        }

        // Update Camera to follow drone
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouseDelta = GetMouseDelta();
            camAngleX -= mouseDelta.x * 0.003f;
            camAngleY -= mouseDelta.y * 0.003f;
            if (camAngleY > 1.5f)
                camAngleY = 1.5f;
            if (camAngleY < -1.5f)
                camAngleY = -1.5f;
        }

        camera.position = {
            drone.x + camDistance * sinf(camAngleX) * cosf(camAngleY),
            drone.y + camDistance * sinf(camAngleY),
            drone.z + camDistance * cosf(camAngleX) * cosf(camAngleY)};
        camera.target = {drone.x, drone.y, drone.z};

        BeginDrawing();
        ClearBackground({135, 206, 235, 255}); // Sky blue background

        BeginMode3D(camera);

        // Draw landscape
        DrawPlane({0.0f, 0.0f, 0.0f}, {500.0f, 500.0f}, {60, 120, 40, 255});
        DrawGrid(100, 2.0f);

        // Clouds
        for (int i = 0; i < cloudCount; i++)
            placeCloud(clouds[i].x, clouds[i].y, clouds[i].z, clouds[i].scale);

        // Trees
        for (int i = 0; i < treeCount; i++)
            placeTree(trees[i].x, trees[i].z, trees[i].height);

        // It should be noted that these buildings are AI generated. I'm not super good with 3D modeling.
        // I'll work on my modeling skills another time lol. For now, enjoy the blocky charm of these structures.

        // Village
        placeHouse(18.0f, -5.0f, {220, 200, 170, 255});
        placeHouse(22.0f, -2.0f, {200, 180, 160, 255});
        placeHouse(25.0f, -7.0f, {210, 195, 175, 255});
        placeHouse(20.0f, -9.0f, {190, 170, 150, 255});
        placeHouse(28.0f, -4.0f, {215, 205, 180, 255});

        // Barn
        placeBarn(-25.0f, -15.0f);

        // Water tower
        placeWaterTower(0.0f, 25.0f);

        // Radio tower
        placeRadioTower(40.0f, -5.0f);

        // Dirt road
        DrawCube({0.0f, 0.01f, 12.0f}, 3.0f, 0.02f, 24.0f, {139, 115, 85, 255});

        // Drone
        DrawSphere({drone.x, drone.y, drone.z}, 0.5f, BLACK);

        EndMode3D();

        // HUD
        DrawText("WASD: Move  |  Space/Ctrl: Up/Down | Shift: Boost |  Left Mouse Button: Rotate Camera", 10, 10, 18, BLACK);
        DrawFPS(10, 570);

        EndDrawing();
    }
    UnloadModel(droneModel);
    CloseWindow();
    return 0;
}