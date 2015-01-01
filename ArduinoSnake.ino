#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

int snake_start = 0;
int snake[64];
int snake_length = 1;
int snake_direction = UP;
int next_direction = UP;
int pins[4]={7,6, 2, 3};
int start_time = 0;
int food = -1;
int get_position(int row, int col){
  return row * 8 + col;
}
int get_row(int pos){
  return pos/8;
}
int get_col(int pos){
  return pos%8;
}
void setup(){
  Serial.begin(115200);
  for(int i = 0; i < 4; ++i){
    pinMode(pins[i], INPUT);
  }
  snake[0] = get_position(4,4);
  start_time = millis();
}
void handleButtons(){
  for(int i = 0; i < 4; ++i){
    if(digitalRead(pins[i]) && abs(snake_direction - i) != 2){
      next_direction = i;
      Serial.print("direction: "); Serial.println(i);
    }
  }
}
boolean food_present(){
  return food != -1;
}
int last_time = 0;
int snake_threshold = 1000;
void handleSnake(){
  int current_time = millis();
  if(current_time - last_time >= snake_threshold){
    last_time += snake_threshold;
    snake_direction = next_direction;
    int next_pos = next_position(snake[(snake_start+snake_length-1)%64], snake_direction);
    if(next_pos == -1){
      //lost
    }else if(food == next_pos){
      snake[(snake_start+snake_length)%64] = next_pos;
      snake_length ++;
      generateFood();
    }else{
      snake[(snake_start+snake_length)%64] = next_pos;
      snake_start = (snake_start+1)%64;
    }
  }
}
void generateFood(){
  while(true){
    food = rand()%64;
    if(!collision(food)){
      return;
    }
  }
}
const int first_food_threshold = 2000;
void handleFood(){
  if(!food_present() && millis()-start_time > first_food_threshold){
    generateFood();
  }
}

void show(){
  for(int i = 0; i < 10;  ++i){
    Serial.println();
  }
  for(int i = 0; i < 8; ++i){
    for(int j = 0; j < 8; ++j){
      if(collision(get_position(i, j))){
        Serial.print("s");
      }else if(food == get_position(i, j)){
        Serial.print("f");
      }else{
        Serial.print(".");
      }
    }
    Serial.println();
  }
  delay(500);
}
boolean collision(int pos){
  for(int i = 0; i < snake_length; ++i){
    if(pos == snake[(snake_start+i)%64]){
      return true;
    }
  }
  return false;
}
int next_position(int current_position, int dir){
  //if walls are solid, uncomment 'return -1's
  switch(dir){
    case LEFT:
      if(get_col(current_position) == 0){
        //return -1;
        return current_position+7;
      }
      return current_position-1;
    case RIGHT:
      if(get_col(current_position) == 7){
        //return -1;
        return current_position-7;
      }
      return current_position+1;
    case UP:
      if(get_row(current_position) == 0){
        //return -1;
        return current_position+7*8;
      }
      return current_position-8;
    case DOWN:
      if(get_row(current_position) == 7){
        //return -1;
        return current_position-7*8;
      }
      return current_position+8;
  }
}
void loop(){
  handleButtons();
  handleSnake();
  handleFood();
  show();
}
