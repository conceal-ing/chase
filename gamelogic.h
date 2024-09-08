#define SCREENWIDTH 640
#define SCREENHEIGHT 480
#define PLAYERSIZE 30
#define PLAYERSPEED 2
#define MAXENEMIES 30

// TODO: Clean up.

typedef struct {
  SDL_Rect pos;
  int speed;
  int health;
} entity;

entity player = { .pos = {10, 10, PLAYERSIZE, PLAYERSIZE}, 0, 100 };
entity cursor = { .pos = {10, 10, PLAYERSIZE, PLAYERSIZE}, 0, 999 };

SDL_Rect healthBar = {10, 10, SCREENWIDTH-10, 25};

entity zombies[MAXENEMIES];
int addedZombies = 0;

void generateZombie(entity *entities, int *counter) {
  if (*counter < MAXENEMIES) {
    int r = rand()%11; // 0-10
    entity newZombie = { .pos = {SCREENWIDTH/2, SCREENHEIGHT/2, r+PLAYERSIZE, r+PLAYERSIZE}, 0, 999};
    entities[*counter] = newZombie;
    *counter+=1;
  }
}

void unCollideWall(entity *ent) {
  int diff = 0;

  diff = ent->pos.x+ent->pos.w-SCREENWIDTH;
  if (diff > 0) { ent->pos.x -= diff; }
  diff = ent->pos.y+ent->pos.h-SCREENHEIGHT;
  if (diff > 0) { ent->pos.y -= diff; }

  if (ent->pos.x < 0) { ent->pos.x = 0; }
  if (ent->pos.y < 0) { ent->pos.y = 0; }
}

// it's 1am and i'm tired
// TODO: Fix this... Or don't, makes the zombies move around funny.
void unCollideEntities(entity *ent1, entity ent2) {  
  if ( !((ent1->pos.x+ent1->pos.w > ent2.pos.x && ent1->pos.x < ent2.pos.x+ent2.pos.w) &&
  (ent1->pos.y+ent1->pos.h > ent2.pos.y && ent1->pos.y < ent2.pos.y+ent2.pos.h)) ) {
  return; }

  if (ent1->health < 101) { ent1->health-=1; return; }
  ent1->pos.x = rand()%(SCREENWIDTH-ent1->pos.w);
  ent1->pos.y = rand()%(SCREENHEIGHT-ent1->pos.h);
}

void follow(entity *ent1, entity target) {
  int speed = 0;
  
  if (ent1->health < 101) {
    speed = PLAYERSPEED;    
  } else {
    speed = rand()%2;
  }

  int targetMiddleHorizontal = target.pos.x+(target.pos.w<<1);
  int targetMiddleVertical   = target.pos.y+(target.pos.h<<1);
  int entMiddleHorizontal    = ent1->pos.x+(ent1->pos.w<<1);
  int entMiddleVertical      = ent1->pos.y+(ent1->pos.h<<1);

  if (targetMiddleHorizontal < entMiddleHorizontal) { ent1->pos.x -= speed; }
  if (targetMiddleHorizontal > entMiddleHorizontal) { ent1->pos.x += speed; }
  if (targetMiddleVertical < entMiddleVertical) { ent1->pos.y -= speed; }
  if (targetMiddleVertical > entMiddleVertical) { ent1->pos.y += speed; }
}

void chase() {
  for (int i = 0; i < addedZombies; i++) {
    follow(&zombies[i], player);
    unCollideWall(&zombies[i]);
    unCollideEntities(&zombies[i], zombies[(i+1)*(i<addedZombies)]);
    unCollideEntities(&player, zombies[i]);
  }
}


void draw(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
  SDL_RenderFillRect(renderer, &healthBar);
  SDL_RenderFillRect(renderer, &cursor.pos); 
  SDL_RenderFillRect(renderer, &player.pos);

  for (int i = 0; i < addedZombies; i++) {
    SDL_RenderFillRect(renderer, &zombies[i].pos);
  } 
}

void onMouseDown(SDL_MouseButtonEvent click, int x, int y) {
  cursor.pos.x = x;
  cursor.pos.y = y;

  generateZombie(zombies, &addedZombies);
}

