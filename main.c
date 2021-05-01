#include "raylib.h"
#include <time.h>
const int screenWidth = 800;
const int screenHeight = 450;
bool gameOver=false;
int SnakeLen=1;
int key_active=0;
int Score=0;
int secs=0;
int mins=0;
int hours=0;
int fps_call=0;
struct Snake{
    int posX,posY,width,height;
    Color BodyColor; 
    Rectangle rect;
}Snake[1024];


struct Food{
    int posX,posY,radius,alpha;
    Color FoodColor;
}Food;
void InitGame(void);
void UpdateGame(void);
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "body-Game");
    InitGame();
    SetTargetFPS(10);          // Set our game to run at 10 frames-per-`
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if(gameOver==false){
            if(fps_call==10){
                secs+=1;
            };
            if(secs==60){
                mins+=1;
                secs=0;
            };
            if(mins==60){
                hours+=1;
                mins=0;
            };
            
                
        };
        
        UpdateGame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
void InitGame(void){
    gameOver=false;
    for(int i=0;i<SnakeLen;i++){
        Snake[i].posX=100;
        Snake[i].posY=200;
        Snake[i].width=20;
        Snake[i].height=20;
        Snake[i].BodyColor=BLUE;
        if(i==0){
            Snake[0].BodyColor=GREEN;
        };
        if(i%2!=0){ 
        Snake[i].BodyColor=BLACK;
        };
    }
    Food.posX= GetRandomValue(30,screenWidth-30);
    Food.posY= GetRandomValue(30,screenHeight-30);
    Food.radius=5;
    Food.alpha=255;
    Food.FoodColor=(Color){255,0,0,Food.alpha};
}
void Grow(){
    for(int i=SnakeLen-1;i>0;i--){
        Snake[i].posX=Snake[i-1].posX;
        Snake[i].posY=Snake[i-1].posY;
        Snake[i].width=20;
        Snake[i].height=20;
        Snake[i].BodyColor=BLUE;
        if(i%2!=0){ 
        Snake[i].BodyColor=BLACK;
        };
    }
    Score++;
}

int blink_secs=0;
void UpdateDraw(){
    if(gameOver==false){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ///snake:
        for(int i=0;i<SnakeLen;i++){
            Snake[i].rect=(Rectangle){Snake[i].posX,Snake[i].posY,Snake[i].width,Snake[i].height};
            DrawRectangleRec(Snake[i].rect,Snake[i].BodyColor);
        };
        if(Snake[0].posX>=screenWidth && key_active==0){
            Snake[0].posX=0;
        };
        if(Snake[0].posX<=0 && key_active==1){
            Snake[0].posX=screenWidth;
        };
        if(Snake[0].posY<=0 && key_active==2){
            Snake[0].posY=screenHeight;
        };
        if(Snake[0].posY>=screenHeight && key_active==3){
            Snake[0].posY=0;
        };
        
        //////food:
        DrawCircle(Food.posX,Food.posY,Food.radius,Food.FoodColor);
        if(Food.alpha==255 && blink_secs==8){
            Food.alpha=0;
            Food.FoodColor=(Color){255,0,0,Food.alpha};
            blink_secs=0;
        }
        else{
            Food.alpha=255;
            Food.FoodColor=(Color){255,0,0,Food.alpha};
            blink_secs+=1;
        }
        
        ///collision:
        bool Food_ate=CheckCollisionCircleRec((Vector2){Food.posX,Food.posY},Food.radius,Snake[0].rect);
        if(Food_ate){
            Food.posX= GetRandomValue(30,screenWidth-30);
            Food.posY= GetRandomValue(30,screenHeight-30);
            SnakeLen+=1;
            Grow();
        };
        for(int j=0;j<SnakeLen;j++){
            bool is_dead=CheckCollisionRecs(Snake[0].rect,Snake[j].rect);   
            if(is_dead && j!=0){
                gameOver=true;
            };
        };
        
        DrawText(TextFormat("Time Elapsed: %01i:%i:%i",hours,mins,secs),20,0,20,PURPLE);
        DrawText(TextFormat("Score: %01i", Score),screenWidth-150,0,20,PURPLE);
        
    };
    if(gameOver==true){
        ClearBackground(RAYWHITE);
        BeginDrawing();
        DrawText(TextFormat("Time Elapsed: %01i:%i:%i",hours,mins,secs),screenWidth*0.3,screenHeight*0.4,50,PURPLE);
        DrawText(TextFormat("Score: %01i", Score),screenWidth*0.3,screenHeight*0.6,50,ORANGE);
        EndDrawing();
        TakeScreenshot("capture.png");//bmp tga    
    };
    EndDrawing();
}

void UpdateInput(void){

    for(int i=SnakeLen-1;i>0;i--){
            Snake[i].posX=Snake[i-1].posX;  
            Snake[i].posY=Snake[i-1].posY;      
    };
   
    if(IsKeyPressed(KEY_RIGHT)){
        key_active=0;
    };
    if(IsKeyPressed(KEY_LEFT)){
       key_active=1;
    };
    if(IsKeyPressed(KEY_UP)){
        key_active=2;
    };
    if(IsKeyPressed(KEY_DOWN)){
        key_active=3;
    };
   
    if(key_active==0){
        Snake[0].posX+=20;
    };
   
    if(key_active==1){
        Snake[0].posX-=20;
    };
    if(key_active==2){
        Snake[0].posY-=20;
    };
    if(key_active==3){
        Snake[0].posY+=20;
    };

}

void UpdateGame(void){
    UpdateInput();
    UpdateDraw(); 
    fps_call+=1;
    if(fps_call>10){
       fps_call=0;
    };
}