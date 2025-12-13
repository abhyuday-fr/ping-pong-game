#include <iostream>
#include <raylib.h>

int player_score = 0;
int cpu_score = 0;

Color red = Color{210,70,90,255};
Color blue = Color{70,120,220,255};
Color purple = Color{250,240,230,255};
Color Yellow = Color{245,205,80,255};

class Ball{
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw(){
        DrawCircle(x,y,radius,Yellow);
    }

    void Update(){
        x += speed_x;
        y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <=0){
            speed_y *=-1;
        }
        if(x + radius >=GetScreenWidth()){
            cpu_score++;
            ResetBall();
        }
        if(x - radius <=0 ){
            player_score++;
            ResetBall();
        }
    }

    void ResetBall(){
        x=GetScreenWidth()/2;
        y=GetScreenHeight()/2;

        int speed_choices[2]={-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle{
    protected:
    void LimitMovement(){
        if(y <=0 ){
            y=0;
        }
        if(y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }

    public:
    float x,y;
    float width, height;
    int speed;

    void Draw(){
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_UP)){
            y = y - speed;
        }
        else if(IsKeyDown(KEY_DOWN)){
            y = y + speed;
        }

        LimitMovement();
    }
};

class CpuPaddle: public Paddle{
    public:
    void Update(){
        if(IsKeyDown(KEY_W)){
            y = y - speed;
        }
        else if(IsKeyDown(KEY_S)){
            y = y + speed;
        }

        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main(){
    std::cout << "Starting the game\n";

    const int screen_width=1280;
    const int screen_height=800;

    InitWindow(screen_width,screen_height,"Ping Pong!");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 10;
    ball.speed_y = 10;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width -10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 15;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed=15;


    while(WindowShouldClose() == false){
        bool gamePaused = (cpu_score == 10 || player_score == 10);
        BeginDrawing();


        //Updating
        if(!gamePaused){
            ball.Update();
            player.Update();
            cpu.Update();
        }

        //Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *=-1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *=-1;
        }

        //Drawing
        ClearBackground(blue);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, red);
        DrawCircle(screen_width/2, screen_height/2, 150, purple);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i",cpu_score), screen_width/4-20 , 20, 80, WHITE);
        DrawText(TextFormat("%i",player_score), 3 * screen_width/4-20 , 20, 80, WHITE);

        if(IsKeyPressed(KEY_SPACE) && gamePaused){
            player_score = 0;
            cpu_score = 0;
            ball.ResetBall();
            gamePaused = false;
        }

        if(cpu_score == 10){
            DrawText("Blue Wins!!", screen_width/2 - 180, 60, 80, WHITE);
            DrawText("SPACE to restart", screen_width/2-300, screen_height/2 + 40, 70, BLACK);
        }
        else if(player_score == 10){
            DrawText("Red Wins!!", screen_width/2 - 160, 60, 80, WHITE);
            DrawText("SPACE to restart", screen_width/2-300, screen_height/2 + 40, 70, BLACK);
        }
        
        EndDrawing();
    }

    std::cout << "Game Over, Thanks for playing :)\n";

    CloseWindow();
    return 0;
}