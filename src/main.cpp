#include <Arduboy2.h>
Arduboy2 arduboy;

bool buttonDebounce = false;
enum GameState {
  OPENING,
  PLAYING,
  ENDING,
  GAMEOVER
};

GameState gameState = OPENING;

int mouseX = 6, mouseY = 6;
int catX = 10, catY = 2;
int frameCount = 0;
const int rows = 15;
const int cols = 15;

enum Tile {
  EMPTY,
  MOUSE,
  CAT,
  BLOCK
};

Tile gameMap[rows][cols];

void callmap(){
  Tile initMap[rows][cols] = {
    {BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK},
    {BLOCK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, CAT,   EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, BLOCK, BLOCK, EMPTY, EMPTY, EMPTY, EMPTY, BLOCK, BLOCK, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, BLOCK, BLOCK, EMPTY, MOUSE, EMPTY, EMPTY, BLOCK, BLOCK, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, EMPTY, EMPTY, EMPTY, BLOCK},    
    {BLOCK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLOCK},
    {BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK}
  };
  // setup関数内でこれを追加
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
        gameMap[r][c] = initMap[r][c];
    }
  }
}
//////////////////////////////////////////////////////////////////////
void moveMouse(){
    // 上に移動
    if (arduboy.justPressed(UP_BUTTON) && mouseY > 0 )
    {
        int y = mouseY - 1;
        // ブロックと猫があるかチェックして上にシフト
        while (y >= 0) {
          if (gameMap[y][mouseX] == BLOCK || gameMap[y][mouseX] == CAT) {
            y--;
          } else {
            break;
          }
        }
        if (y >= 0 && gameMap[y][mouseX] == EMPTY) {
          for (int i = y; i < mouseY; i++) {
            if (gameMap[i + 1][mouseX] == CAT) {
              // 猫を押し出す
              gameMap[i][mouseX] = CAT;
              // 猫の座標を更新
              catY = i;
            } else {
              // ブロックを押し出す
              gameMap[i][mouseX] = BLOCK;
            }
          }
          // ネズミを移動
          gameMap[mouseY][mouseX] = EMPTY;
          mouseY--;
          gameMap[mouseY][mouseX] = MOUSE;
        }
    }
    // 下に移動
    if (arduboy.justPressed(DOWN_BUTTON) && mouseY < rows - 1)
    {
      int y = mouseY + 1;
      // ブロックと猫があるかチェックして下にシフト
      while (y < rows) {
        if (gameMap[y][mouseX] == BLOCK || gameMap[y][mouseX] == CAT) {
          y++;
        } else {
          break;
        }
      }
      if (y < rows && gameMap[y][mouseX] == EMPTY) {
        for (int i = y; i > mouseY; i--) {
          if (gameMap[i - 1][mouseX] == CAT) {
            gameMap[i][mouseX] = CAT;
            catY = i;  // 猫の座標を更新
          } else {
            gameMap[i][mouseX] = BLOCK;
          }
        }
        gameMap[mouseY][mouseX] = EMPTY;
        mouseY++;
        gameMap[mouseY][mouseX] = MOUSE;
        }
    }
    // 左に移動
    if (arduboy.justPressed(LEFT_BUTTON) && mouseX > 0 )
    {
        int x = mouseX - 1;
        // ブロックと猫があるかチェックして左にシフト
        while (x >= 0) {
          if (gameMap[mouseY][x] == BLOCK || gameMap[mouseY][x] == CAT) {
            x--;
          } else {
            break;
          }
        }
        if (x >= 0 && gameMap[mouseY][x] == EMPTY) {
          for (int i = x; i < mouseX; i++) {
            if (gameMap[mouseY][i + 1] == CAT) {
              gameMap[mouseY][i] = CAT;
              catX = i; // 猫の座標を更新
            } else {
              gameMap[mouseY][i] = BLOCK;
            }
          }
          gameMap[mouseY][mouseX] = EMPTY;
          mouseX--;
          gameMap[mouseY][mouseX] = MOUSE;
        }
    }
    // 右に移動
    if (arduboy.justPressed(RIGHT_BUTTON) && mouseX < cols - 1)
    {
        int x = mouseX + 1;
        // ブロックと猫があるかチェックして右にシフト
        while (x < cols)
        {
          if (gameMap[mouseY][x] == BLOCK || gameMap[mouseY][x] == CAT) {
            x++;
          } else {
            break;
          }
        }
        if (x < cols && gameMap[mouseY][x] == EMPTY) 
        {
          for (int i = x; i > mouseX; i--) {
            if (gameMap[mouseY][i - 1] == CAT) {
              gameMap[mouseY][i] = CAT;
              catX = i; // 猫の座標を更新
            } else {
              gameMap[mouseY][i] = BLOCK;
            }
          }
          gameMap[mouseY][mouseX] = EMPTY;
          mouseX++;
          gameMap[mouseY][mouseX] = MOUSE;
        }
    }
}
///////////////////////////////////////////////////////////////
void moveCat(){
    int diffX = mouseX - catX;
    int diffY = mouseY - catY;
    int targetX = catX;
    int targetY = catY;
    if (frameCount % 40 == 0) 
    {
      if (abs(diffX) <= 3 && abs(diffY) <= 3)
      {
          // ネズミが近くにいる場合、ネズミの方向に移動する
          if (abs(diffX) > abs(diffY))
          {
            // X方向の差の方が大きい場合、X方向に1マス移動
            targetX += (diffX > 0) ? 1 : -1;

            // Y方向にも差がある場合、Y方向にも1マス移動（斜め移動）
            if (diffY != 0) {
              targetY += (diffY > 0) ? 1 : -1;
            }
            } else if (abs(diffX) < abs(diffY))
            {
            // Y方向の差の方が大きい場合、Y方向に1マス移動
            targetY += (diffY > 0) ? 1 : -1;

            // X方向にも差がある場合、X方向にも1マス移動（斜め移動）
            if (diffX != 0) {
              targetX += (diffX > 0) ? 1 : -1;
            }
            }
            else {
            // X方向とY方向の差が等しい場合、斜め方向に1マス移動
            targetX += (diffX > 0) ? 1 : -1;
            targetY += (diffY > 0) ? 1 : -1;
            }
        }
        else {
        // ランダムに移動
        int directions[][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        int randomIndex = random(8); // 0-7の乱数
        targetX = catX + directions[randomIndex][0];
        targetY = catY + directions[randomIndex][1];
        }

      // 移動先がゲームボードの範囲内か、壁や他のブロックにぶつからないか確認
      if (targetX >= 0 && targetY >= 0 && targetX < cols && targetY < rows && gameMap[targetY][targetX] != BLOCK) {
        gameMap[catY][catX] = EMPTY;
        catX = targetX;
        catY = targetY;
        gameMap[catY][catX] = CAT;
      }
  }
}
//////////////////////////////////////////////////////////////
void drawGameMap(){
      for (int y = 0; y < rows; ++y) {
      for (int x = 0; x < cols; ++x) {
        int cellValue = gameMap[y][x];
        int drawX = x * 5;  // 5x5 grid
        int drawY = y * 5;  // 5x5 grid
        switch(cellValue) {
          case EMPTY:
            break;
          case MOUSE:
            arduboy.drawCircle(drawX + 2, drawY + 2, 2);
            break;
          case CAT:
            arduboy.drawCircle(drawX + 2, drawY + 2, 2);  // サークルで猫を描画
            arduboy.drawPixel(drawX + 2, drawY + 2);  // 猫のサークルの中に点を描画
            break;
          case BLOCK:
            arduboy.drawRect(drawX, drawY, 5, 5);
            break;
        }
      }
    }
  }


///////////////////////////////////////////////////////////////
void setup()
{
  arduboy.begin();
   // Initialize gameMap with EMPTY
  callmap();
  gameState = PLAYING;
  // Place mouse, cat, and blocks
  // Place mouse, cat, and blocks with initial values
}
//////////////////////////////////////////////////////////////
void loop() 
{
  if (!arduboy.nextFrame()) {
    return;
  }
  arduboy.clear();
  arduboy.pollButtons(); // 追加
  if(gameState == PLAYING)
  {
    //ネズミの移動
    moveMouse();
    //クリア判定
    if(gameMap[catY-1][catX] == BLOCK &&
      gameMap[catY+1][catX] == BLOCK &&
      gameMap[catY ][catX -1] == BLOCK && 
      gameMap[catY][catX+1] == BLOCK )
      {
          gameState = ENDING;
      } 
    // 猫が60フレームごとに移動
    // ネズミとの距離を計算
    
    moveCat();
      
    // ネズミと猫が同じ座標にいる場合、ゲームオーバー
    if (mouseX == catX && mouseY == catY) {
          gameState = GAMEOVER;
    }  
    
    //ゲームオーバじゃなければフレームカウントを増やす
    frameCount++;
    // Draw the game map
    drawGameMap();

    arduboy.setCursor(80, 10);
    arduboy.print("Surround ");
    arduboy.setCursor(80, 20);
    arduboy.print("the cats ");
    arduboy.setCursor(80, 30);
    arduboy.print("with box");
    arduboy.setCursor(80, 40);
    arduboy.print("Rodents ");
    arduboy.setCursor(80, 50);
    arduboy.print("Revenge! ");
    arduboy.display();
  }//gameplaying
  if(gameState == ENDING){
          arduboy.setCursor(20, 10);
          arduboy.print("Game Clear");
          arduboy.setCursor(20, 20);
          arduboy.print("Thanks for playing");
          arduboy.setCursor(20, 30);
          arduboy.print("coded by motoki");
          arduboy.setCursor(20, 40);
          arduboy.print("New game?");
          arduboy.setCursor(20, 50);
          arduboy.print("press A Button");
          arduboy.display();

          if (!buttonDebounce && arduboy.justPressed(A_BUTTON)) {
              mouseX = 6; mouseY = 6;
              catX = 10; catY = 2;
              callmap();
              gameState = PLAYING;
              buttonDebounce = true;
          } else if (!arduboy.pressed(A_BUTTON)) {
              buttonDebounce = false;
          }
      return;
  }
  if(gameState == GAMEOVER)
  {
          arduboy.setCursor(50, 10);
          arduboy.print("game ");
          arduboy.setCursor(50, 20);
          arduboy.print("over! ");
          arduboy.setCursor(30, 30);
          arduboy.print("New game?");
          arduboy.setCursor(30, 40);
          arduboy.print("press A Button");
          arduboy.display();

          if (!buttonDebounce && arduboy.justPressed(A_BUTTON)) {
              mouseX = 6; mouseY = 6;
              catX = 10; catY = 2;
              callmap();
              gameState = PLAYING;
              buttonDebounce = true;
          } else if (!arduboy.pressed(A_BUTTON)) {
              buttonDebounce = false;
          }
   return;
  }
}
